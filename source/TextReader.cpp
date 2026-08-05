#include <TextReader.hpp>

#include <algorithm>
#include <array>
#include <cstdio>
#include <limits>
#include <vector>

#include <Config.hpp>
#include <GbkTable.hpp>
#include <Log.hpp>
#include <Overlay.hpp>

namespace {
    constexpr u32 ReplacementCharacter = 0xFFFDU;

    struct EncodingDetection {
        TextEncoding encoding;
        long contentOffset;
        const char *name;
    };

    bool isContinuationByte(u8 value) {
        return (value & 0xC0U) == 0x80U;
    }

    bool isValidUtf8(const std::vector<u8> &bytes) {
        size_t offset = 0;
        while (offset < bytes.size()) {
            const u8 first = bytes[offset++];
            if (first < 0x80U)
                continue;

            size_t continuationCount = 0;
            u32 codepoint = 0;
            u32 minimum = 0;
            if (first >= 0xC2U && first <= 0xDFU) {
                continuationCount = 1;
                codepoint = first & 0x1FU;
                minimum = 0x80U;
            } else if (first >= 0xE0U && first <= 0xEFU) {
                continuationCount = 2;
                codepoint = first & 0x0FU;
                minimum = 0x800U;
            } else if (first >= 0xF0U && first <= 0xF4U) {
                continuationCount = 3;
                codepoint = first & 0x07U;
                minimum = 0x10000U;
            } else {
                return false;
            }

            if (offset + continuationCount > bytes.size())
                return false;
            for (size_t i = 0; i < continuationCount; ++i) {
                const u8 next = bytes[offset++];
                if (!isContinuationByte(next))
                    return false;
                codepoint = (codepoint << 6U) | (next & 0x3FU);
            }

            if (codepoint < minimum || codepoint > 0x10FFFFU ||
                (codepoint >= 0xD800U && codepoint <= 0xDFFFU))
                return false;
        }
        return true;
    }

    bool isValidGb18030(const std::vector<u8> &bytes) {
        bool sawMultibyte = false;
        size_t offset = 0;
        while (offset < bytes.size()) {
            const u8 first = bytes[offset++];
            if (first < 0x80U || first == 0x80U)
                continue;
            if (first < 0x81U || first > 0xFEU || offset >= bytes.size())
                return false;

            const u8 second = bytes[offset++];
            if ((second >= 0x40U && second <= 0x7EU) ||
                (second >= 0x80U && second <= 0xFEU)) {
                sawMultibyte = true;
                continue;
            }

            if (second < 0x30U || second > 0x39U || offset + 2 > bytes.size())
                return false;
            const u8 third = bytes[offset++];
            const u8 fourth = bytes[offset++];
            if (third < 0x81U || third > 0xFEU || fourth < 0x30U || fourth > 0x39U)
                return false;
            sawMultibyte = true;
        }
        return sawMultibyte;
    }

    EncodingDetection detectEncoding(FILE *file) {
        std::rewind(file);
        std::vector<u8> sample(4096);
        const size_t sampleSize = std::fread(sample.data(), 1, sample.size(), file);
        sample.resize(sampleSize);
        std::rewind(file);

        if (sample.size() >= 4 && sample[0] == 0x00U && sample[1] == 0x00U && sample[2] == 0xFEU && sample[3] == 0xFFU)
            return { TextEncoding::Utf32Be, 4, "UTF-32 BE" };
        if (sample.size() >= 4 && sample[0] == 0xFFU && sample[1] == 0xFEU && sample[2] == 0x00U && sample[3] == 0x00U)
            return { TextEncoding::Utf32Le, 4, "UTF-32 LE" };
        if (sample.size() >= 3 && sample[0] == 0xEFU && sample[1] == 0xBBU && sample[2] == 0xBFU)
            return { TextEncoding::Utf8, 3, "UTF-8 BOM" };
        if (sample.size() >= 2 && sample[0] == 0xFEU && sample[1] == 0xFFU)
            return { TextEncoding::Utf16Be, 2, "UTF-16 BE" };
        if (sample.size() >= 2 && sample[0] == 0xFFU && sample[1] == 0xFEU)
            return { TextEncoding::Utf16Le, 2, "UTF-16 LE" };

        size_t evenZeroes = 0;
        size_t oddZeroes = 0;
        for (size_t i = 0; i < sample.size(); ++i) {
            if (sample[i] == 0)
                (i % 2 == 0 ? evenZeroes : oddZeroes)++;
        }
        if (sample.size() >= 8 && oddZeroes > sample.size() / 4 && evenZeroes < sample.size() / 16)
            return { TextEncoding::Utf16Le, 0, "UTF-16 LE (detected)" };
        if (sample.size() >= 8 && evenZeroes > sample.size() / 4 && oddZeroes < sample.size() / 16)
            return { TextEncoding::Utf16Be, 0, "UTF-16 BE (detected)" };

        bool validUtf8 = isValidUtf8(sample);
        bool validGb18030 = isValidGb18030(sample);
        if (sampleSize == 4096) {
            for (size_t trim = 1; trim <= 3 && trim < sample.size(); ++trim) {
                std::vector<u8> prefix(sample.begin(), sample.end() - trim);
                validUtf8 = validUtf8 || isValidUtf8(prefix);
                validGb18030 = validGb18030 || isValidGb18030(prefix);
            }
        }

        if (validUtf8)
            return { TextEncoding::Utf8, 0, "UTF-8" };
        if (validGb18030)
            return { TextEncoding::Gb18030, 0, "GB18030/GBK" };
        return { TextEncoding::Windows1252, 0, "Windows-1252" };
    }

    bool readByte(FILE *file, u8 &value) {
        const int next = std::fgetc(file);
        if (next == EOF)
            return false;
        value = static_cast<u8>(next);
        return true;
    }

    bool readU16(FILE *file, bool littleEndian, u16 &value) {
        u8 first = 0;
        u8 second = 0;
        if (!readByte(file, first) || !readByte(file, second))
            return false;
        value = littleEndian
            ? static_cast<u16>(first | (static_cast<u16>(second) << 8U))
            : static_cast<u16>((static_cast<u16>(first) << 8U) | second);
        return true;
    }

    bool readU32(FILE *file, bool littleEndian, u32 &value) {
        std::array<u8, 4> bytes{};
        for (u8 &byte : bytes) {
            if (!readByte(file, byte))
                return false;
        }
        if (littleEndian) {
            value = static_cast<u32>(bytes[0]) | (static_cast<u32>(bytes[1]) << 8U) |
                    (static_cast<u32>(bytes[2]) << 16U) | (static_cast<u32>(bytes[3]) << 24U);
        } else {
            value = (static_cast<u32>(bytes[0]) << 24U) | (static_cast<u32>(bytes[1]) << 16U) |
                    (static_cast<u32>(bytes[2]) << 8U) | static_cast<u32>(bytes[3]);
        }
        return true;
    }

    u32 decodeGb18030Pointer(u32 pointer) {
        size_t low = 0;
        size_t high = std::size(text_encoding::Gb18030Ranges);
        while (low < high) {
            const size_t middle = low + (high - low) / 2;
            const auto &range = text_encoding::Gb18030Ranges[middle];
            if (pointer < range.pointerStart) {
                high = middle;
            } else if (pointer > range.pointerEnd) {
                low = middle + 1;
            } else {
                return range.codepointStart + pointer - range.pointerStart;
            }
        }
        return ReplacementCharacter;
    }

    u32 decodeWindows1252(u8 value) {
        constexpr std::array<u16, 32> extension = {
            0x20AC, 0x0081, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021,
            0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, 0x008D, 0x017D, 0x008F,
            0x0090, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,
            0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0x009D, 0x017E, 0x0178,
        };
        if (value >= 0x80U && value <= 0x9FU)
            return extension[value - 0x80U];
        return value;
    }

    bool readCodepoint(FILE *file, TextEncoding encoding, u32 &codepoint) {
        if (encoding == TextEncoding::Utf16Le || encoding == TextEncoding::Utf16Be) {
            u16 first = 0;
            if (!readU16(file, encoding == TextEncoding::Utf16Le, first))
                return false;
            if (first >= 0xD800U && first <= 0xDBFFU) {
                u16 second = 0;
                if (!readU16(file, encoding == TextEncoding::Utf16Le, second) ||
                    second < 0xDC00U || second > 0xDFFFU) {
                    codepoint = ReplacementCharacter;
                    return true;
                }
                codepoint = 0x10000U + ((static_cast<u32>(first) - 0xD800U) << 10U) +
                            (static_cast<u32>(second) - 0xDC00U);
                return true;
            }
            codepoint = (first >= 0xDC00U && first <= 0xDFFFU) ? ReplacementCharacter : first;
            return true;
        }

        if (encoding == TextEncoding::Utf32Le || encoding == TextEncoding::Utf32Be) {
            if (!readU32(file, encoding == TextEncoding::Utf32Le, codepoint))
                return false;
            if (codepoint > 0x10FFFFU || (codepoint >= 0xD800U && codepoint <= 0xDFFFU))
                codepoint = ReplacementCharacter;
            return true;
        }

        u8 first = 0;
        if (!readByte(file, first))
            return false;

        if (encoding == TextEncoding::Windows1252) {
            codepoint = decodeWindows1252(first);
            return true;
        }

        if (encoding == TextEncoding::Gb18030) {
            if (first < 0x80U) {
                codepoint = first;
                return true;
            }
            if (first == 0x80U) {
                codepoint = 0x20ACU;
                return true;
            }

            u8 second = 0;
            if (first < 0x81U || first > 0xFEU || !readByte(file, second)) {
                codepoint = ReplacementCharacter;
                return true;
            }
            if ((second >= 0x40U && second <= 0x7EU) ||
                (second >= 0x80U && second <= 0xFEU)) {
                const u32 trailIndex = second <= 0x7EU ? second - 0x40U : second - 0x41U;
                codepoint = text_encoding::GbkToUnicode[(first - 0x81U) * 190U + trailIndex];
                if (codepoint == 0)
                    codepoint = ReplacementCharacter;
                return true;
            }
            if (second >= 0x30U && second <= 0x39U) {
                u8 third = 0;
                u8 fourth = 0;
                if (readByte(file, third) && readByte(file, fourth) &&
                    third >= 0x81U && third <= 0xFEU && fourth >= 0x30U && fourth <= 0x39U) {
                    const u32 pointer = (((first - 0x81U) * 10U + second - 0x30U) * 126U +
                                         third - 0x81U) * 10U + fourth - 0x30U;
                    codepoint = decodeGb18030Pointer(pointer);
                    return true;
                }
            }
            codepoint = ReplacementCharacter;
            return true;
        }

        if (first < 0x80U) {
            codepoint = first;
            return true;
        }

        size_t continuationCount = 0;
        u32 minimum = 0;
        if (first >= 0xC2U && first <= 0xDFU) {
            continuationCount = 1;
            codepoint = first & 0x1FU;
            minimum = 0x80U;
        } else if (first >= 0xE0U && first <= 0xEFU) {
            continuationCount = 2;
            codepoint = first & 0x0FU;
            minimum = 0x800U;
        } else if (first >= 0xF0U && first <= 0xF4U) {
            continuationCount = 3;
            codepoint = first & 0x07U;
            minimum = 0x10000U;
        } else {
            codepoint = ReplacementCharacter;
            return true;
        }

        for (size_t i = 0; i < continuationCount; ++i) {
            u8 next = 0;
            if (!readByte(file, next) || !isContinuationByte(next)) {
                codepoint = ReplacementCharacter;
                return true;
            }
            codepoint = (codepoint << 6U) | (next & 0x3FU);
        }
        if (codepoint < minimum || codepoint > 0x10FFFFU ||
            (codepoint >= 0xD800U && codepoint <= 0xDFFFU))
            codepoint = ReplacementCharacter;
        return true;
    }

    void appendUtf8(std::string &output, u32 codepoint) {
        std::array<u8, 4> encoded{};
        const ssize_t size = encode_utf8(encoded.data(), codepoint);
        if (size > 0)
            output.append(reinterpret_cast<const char*>(encoded.data()), static_cast<size_t>(size));
    }

    bool readTextLine(FILE *file, TextEncoding encoding, std::string &line) {
        line.clear();
        bool readAny = false;
        u32 codepoint = 0;
        while (readCodepoint(file, encoding, codepoint)) {
            readAny = true;
            if (codepoint == '\n')
                return true;
            if (codepoint != '\r' && codepoint != 0)
                appendUtf8(line, codepoint);
        }
        return readAny;
    }
}

std::string TextReaderChunk::EMPTY_STRING;

void TextReaderChunk::loadText(FILE *file) {
    if (m_lines != nullptr)
        return;
    if (std::fseek(file, m_fileOffset, SEEK_SET) != 0) {
        Log::log("Failed to load chunk");
        return;
    }

    m_lines = new std::vector<std::string>(MAX_SIZE);
    for (u32 line = 0; line < MAX_SIZE; ++line) {
        if (!readTextLine(file, m_encoding, m_lines->at(line)))
            break;
    }
}

void TextReaderChunk::unloadText() {
    delete m_lines;
    m_lines = nullptr;
}

std::string& TextReaderChunk::getLine(u32 lineOffset) const {
    if (m_lines != nullptr && lineOffset < m_lines->size())
        return m_lines->at(lineOffset);
    return EMPTY_STRING;
}

TextReader::TextReader(std::string const &path)
    : m_path(path),
      m_file(nullptr),
      m_encoding(TextEncoding::Utf8),
      m_encodingName("Unknown"),
      m_totalLines(0),
      m_lineNum(0),
      m_chunkMid(0),
      m_loading(false),
      m_loaded(false),
      m_size(16),
      m_panx(0),
      m_timePrev(std::chrono::steady_clock::now()),
      m_timeAggregate(0ms),
      m_timeTicks(0),
      m_fps(0),
      m_debug(false)
{
    auto j = Config::read();
    auto resume = j["files"][m_path].find("resume");
    if (resume != j["files"][m_path].end())
        m_lineNum = *resume;

    auto bookmarks = j["files"][m_path].find("bookmarks");
    if (bookmarks != j["files"][m_path].end()) {
        for (auto &bookmark : *bookmarks)
            m_bookmarks.insert(static_cast<u32>(bookmark));
    }
}

TextReader::~TextReader() {
    if (m_file)
        std::fclose(m_file);
}

tsl::elm::Element* TextReader::createUI() {
    auto *drawer = new tsl::elm::CustomDrawer([this](tsl::gfx::Renderer *renderer, s32 x, s32 y, s32 w, s32 h) {
        renderer->fillScreen(a({ 0x0, 0x0, 0x0, 0xD }));

        if (!m_loading) {
            renderer->drawString("Loading...", false, 20, 50, 18, a(0xFFFF));
            m_loading = true;
            return;
        }
        if (!m_loaded) {
            m_file = std::fopen(m_path.c_str(), "rb");
            if (m_file) {
                const EncodingDetection detection = detectEncoding(m_file);
                m_encoding = detection.encoding;
                m_encodingName = detection.name;
                std::fseek(m_file, detection.contentOffset, SEEK_SET);
                Log::log("Opened file %s as %s", m_path.c_str(), detection.name);

                m_chunks.emplace_back(detection.contentOffset, m_encoding);
                std::string line;
                u32 lineCount = 0;
                while (readTextLine(m_file, m_encoding, line)) {
                    ++lineCount;
                    if (lineCount % TextReaderChunk::MAX_SIZE == 0) {
                        const long offset = std::ftell(m_file);
                        if (offset >= 0)
                            m_chunks.emplace_back(offset, m_encoding);
                    }
                }
                m_totalLines = std::max(1U, lineCount);
                m_lineNum = std::min(m_lineNum, m_totalLines - 1);
                scrollTo(m_lineNum);
            } else {
                Log::log("Could not open file %s", m_path.c_str());
            }
            m_loaded = true;
            return;
        }
        if (!m_file) {
            renderer->drawString("Could not open file", false, 20, 50, 18, a(0xFFFF));
            return;
        }

        s32 textTop = y + 10;
        if (m_debug) {
            const std::string status = TEXT_READER_VERSION " | " + m_encodingName + " | " +
                                       std::to_string(m_totalLines) + " lines | " +
                                       std::to_string(m_fps) + " FPS";
            renderer->drawString(status.c_str(), false, x + 10, y + 22, 13, a(0xAFFF));
            renderer->drawRect(x + 8, y + 31, w - 16, 1, a(0x6FFF));
            textTop = y + 48;
        }

        const u32 numLinesToShow = (h - (textTop - y)) / std::max(1U, m_size) + 2;
        for (u32 i = 0; i < numLinesToShow; ++i) {
            const u32 absoluteLine = m_lineNum + i;
            if (absoluteLine >= m_totalLines)
                break;

            const u32 chunk = absoluteLine / TextReaderChunk::MAX_SIZE;
            const u32 line = absoluteLine % TextReaderChunk::MAX_SIZE;
            if (chunk >= m_chunks.size())
                break;

            if (m_bookmarks.find(absoluteLine) != m_bookmarks.end())
                renderer->drawRect(x, textTop + i * m_size, w, 1, a({ 0x6, 0x1, 0x1, 0xF }));
            printLn(m_chunks[chunk].getLine(line),
                    x + 10 + m_panx * static_cast<s32>(m_size),
                    textTop + static_cast<s32>((i + 1) * m_size),
                    m_size, renderer);
        }

        const u32 progressY = m_lineNum * (tsl::cfg::FramebufferHeight - 20) / m_totalLines;
        renderer->drawRect(0, progressY, 2, 20, a({ 0x8, 0x8, 0x8, 0xF }));

    });

    drawer->setBoundaries(0, 0, tsl::cfg::FramebufferWidth, tsl::cfg::FramebufferHeight);
    return drawer;
}

void TextReader::printLn(std::string const &text, s32 x, s32 y, u32 fontSize, tsl::gfx::Renderer *renderer) const {
    if (text.empty())
        return;
    renderer->drawString(text.c_str(), false, x, y, static_cast<float>(fontSize), a(0xFFFF));
}

bool TextReader::handleInput(u64 keysDown, u64 keysHeld, const HidTouchState &touchInput, HidAnalogStickState leftJoyStick, HidAnalogStickState rightJoyStick) {
    if (keysHeld & HidNpadButton_ZR) {
        if (keysHeld & HidNpadButton_StickLUp) scrollTo(0);
        if (keysHeld & HidNpadButton_StickLDown) scrollTo(m_totalLines - 1);
        if (keysHeld & HidNpadButton_StickLLeft) scroll(-1000);
        if (keysHeld & HidNpadButton_StickLRight) scroll(1000);
    } else if (keysHeld & HidNpadButton_ZL) {
        if (keysHeld & HidNpadButton_StickLUp) scroll(-20);
        if (keysHeld & HidNpadButton_StickLDown) scroll(20);
        if (keysHeld & HidNpadButton_StickLLeft) scroll(-200);
        if (keysHeld & HidNpadButton_StickLRight) scroll(200);
    } else {
        if (keysHeld & HidNpadButton_StickLUp) scroll(-2);
        if (keysHeld & HidNpadButton_StickLDown) scroll(2);
        if (keysHeld & HidNpadButton_StickLLeft) scroll(-1);
        if (keysHeld & HidNpadButton_StickLRight) scroll(1);
    }

    if (keysHeld & HidNpadButton_StickRUp) scroll(-1);
    if (keysHeld & HidNpadButton_StickRDown) scroll(1);
    if (keysHeld & HidNpadButton_StickRLeft) m_panx++;
    if (keysHeld & HidNpadButton_StickRRight) m_panx--;
    if (keysDown & HidNpadButton_StickR) m_panx = 0;

    if ((keysDown & HidNpadButton_Up) && m_size < 48) m_size++;
    if ((keysDown & HidNpadButton_Down) && m_size > 6) m_size--;

    if (keysDown & HidNpadButton_X) tsl::Overlay::get()->hide();
    if (keysDown & HidNpadButton_Y) toggleBookmark();
    if (keysDown & HidNpadButton_L) previousBookmark();
    if (keysDown & HidNpadButton_R) nextBookmark();
    if (keysDown & HidNpadButton_B) close();
    if (keysDown & HidNpadButton_Minus) m_debug = !m_debug;

    return true;
}

void TextReader::scrollTo(u32 line) {
    scroll(static_cast<s32>(line) - static_cast<s32>(m_lineNum));
}

void TextReader::scroll(s32 offset) {
    if (m_totalLines == 0)
        return;

    const s32 lastLine = static_cast<s32>(m_totalLines - 1);
    const u32 newLineNum = static_cast<u32>(std::clamp(static_cast<s32>(m_lineNum) + offset, 0, lastLine));
    const u32 newChunk = newLineNum / TextReaderChunk::MAX_SIZE;
    const u32 newOffset = newLineNum % TextReaderChunk::MAX_SIZE;

    if (newChunk < m_chunkMid &&
        (newChunk + 1 < m_chunkMid || newOffset < TextReaderChunk::MAX_SIZE / 2)) {
        for (u32 chunk = m_chunkMid + 1; chunk > newChunk + 1; --chunk)
            unloadText(chunk);
        m_chunkMid = newChunk;
    } else if (newChunk > m_chunkMid &&
               (newChunk > m_chunkMid + 1 || newOffset > TextReaderChunk::MAX_SIZE / 2)) {
        for (u32 chunk = m_chunkMid > 0 ? m_chunkMid - 1 : 0; chunk + 1 < newChunk; ++chunk)
            unloadText(chunk);
        m_chunkMid = newChunk;
    }

    loadText(m_chunkMid);
    if (m_chunkMid > 0) loadText(m_chunkMid - 1);
    if (m_chunkMid + 1 < m_chunks.size()) loadText(m_chunkMid + 1);
    m_lineNum = newLineNum;
}

void TextReader::loadText(u32 chunk) {
    if (chunk < m_chunks.size())
        m_chunks[chunk].loadText(m_file);
}

void TextReader::unloadText(u32 chunk) {
    if (chunk < m_chunks.size())
        m_chunks[chunk].unloadText();
}

void TextReader::toggleBookmark() {
    if (m_bookmarks.find(m_lineNum) == m_bookmarks.end())
        m_bookmarks.insert(m_lineNum);
    else
        m_bookmarks.erase(m_lineNum);

    Config::update([this](json &j) { j["files"][m_path]["bookmarks"] = m_bookmarks; });
}

void TextReader::previousBookmark() {
    auto bookmark = m_bookmarks.lower_bound(m_lineNum);
    if (bookmark == m_bookmarks.begin())
        return;
    --bookmark;
    scrollTo(*bookmark);
}

void TextReader::nextBookmark() {
    const auto bookmark = m_bookmarks.upper_bound(m_lineNum);
    if (bookmark != m_bookmarks.end())
        scrollTo(*bookmark);
}

void TextReader::close() const {
    Config::update([this](json &j) { j["files"][m_path]["resume"] = m_lineNum; });
    tsl::goBack();
}
