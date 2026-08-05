#pragma once

#include <tesla.hpp>

#include <chrono>
#include <set>
#include <string>
#include <vector>

enum class TextEncoding {
    Utf8,
    Utf16Le,
    Utf16Be,
    Utf32Le,
    Utf32Be,
    Gb18030,
    Windows1252,
};

class TextReaderChunk {
public:
    static const size_t MAX_SIZE = 1000;

    TextReaderChunk(long fileOffset, TextEncoding encoding)
        : m_fileOffset(fileOffset),
          m_encoding(encoding),
          m_lines(nullptr)
    {}
    ~TextReaderChunk() { unloadText(); }

    void loadText(FILE *file);
    void unloadText();
    std::string& getLine(u32 lineOffset) const;

private:
    static std::string EMPTY_STRING;
    long m_fileOffset;
    TextEncoding m_encoding;
    std::vector<std::string> *m_lines;
};

class TextReader : public tsl::Gui {
public:
    explicit TextReader(std::string const &path);
    ~TextReader();

    tsl::elm::Element* createUI() override;
    bool handleInput(u64 keysDown, u64 keysHeld, const HidTouchState &touchInput, HidAnalogStickState leftJoyStick, HidAnalogStickState rightJoyStick) override;

    void scrollTo(u32 line);
    void scroll(s32 offset);
    void toggleBookmark();
    void previousBookmark();
    void nextBookmark();
    void close() const;

    void update() override {
        auto now = std::chrono::steady_clock::now();
        m_timeAggregate += std::chrono::duration_cast<std::chrono::milliseconds>(now - m_timePrev);
        ++m_timeTicks;
        m_timePrev = now;
        if (m_timeAggregate > 200ms) {
            m_fps = 1000 * m_timeTicks / m_timeAggregate.count();
            m_timeAggregate = 0ms;
            m_timeTicks = 0;
        }
    }

protected:
    void printLn(std::string const &text, s32 x, s32 y, u32 fontSize, tsl::gfx::Renderer *renderer) const;

private:
    void loadText(u32 chunk);
    void unloadText(u32 chunk);

    std::string m_path;
    FILE *m_file;
    TextEncoding m_encoding;
    std::string m_encodingName;
    u32 m_totalLines;
    u32 m_lineNum;
    u32 m_chunkMid;
    std::vector<TextReaderChunk> m_chunks;

    bool m_loading;
    bool m_loaded;

    u32 m_size;
    s32 m_panx;
    std::set<u32> m_bookmarks;

    std::chrono::steady_clock::time_point m_timePrev;
    std::chrono::milliseconds m_timeAggregate;
    u32 m_timeTicks;
    u32 m_fps;
    bool m_debug;
};
