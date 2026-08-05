#include <FileSelect.hpp>

#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>

#include <FileSelectEntry.hpp>
#include <CompositeElement.hpp>
#include <Config.hpp>
#include <Utils.hpp>

namespace {
    bool queryEntryType(const std::string &path, const dirent *entry, bool &isDirectory, bool &isRegularFile) {
        isDirectory = entry->d_type == DT_DIR;
        isRegularFile = entry->d_type == DT_REG;

        if (entry->d_type != DT_UNKNOWN)
            return true;

        struct stat info {};
        if (stat(path.c_str(), &info) != 0)
            return false;

        isDirectory = S_ISDIR(info.st_mode);
        isRegularFile = S_ISREG(info.st_mode);
        return true;
    }
}

FileSelect::FileSelect(std::string const &path)
    : m_path(path),
      m_failed(false)
{
    auto favorites = Config::read()["favorites"];
    if (!favorites.is_array())
        favorites = json::array();

    auto isFavorite = [&favorites](std::string const &entryPath) {
        return std::find(favorites.begin(), favorites.end(), entryPath) != favorites.end();
    };

    DIR *directory = opendir(m_path.c_str());
    if (!directory) {
        m_failed = true;
        return;
    }

    while (dirent *entry = readdir(directory)) {
        const std::string name(entry->d_name);
        if (name == "." || name == "..")
            continue;

        std::string absolutePath = m_path;
        if (!absolutePath.empty() && absolutePath.back() != '/')
            absolutePath.push_back('/');
        absolutePath.append(name);

        bool isDirectory;
        bool isRegularFile;
        if (!queryEntryType(absolutePath, entry, isDirectory, isRegularFile))
            continue;

        if (isDirectory) {
            absolutePath.push_back('/');
            m_entries.push_back(new FileSelectDirEntry(absolutePath, isFavorite(absolutePath)));
        } else if (isRegularFile && utils::endsWith(name, ".txt", true)) {
            m_entries.push_back(new FileSelectFileEntry(absolutePath, isFavorite(absolutePath)));
        }
    }

    closedir(directory);

    std::sort(
        m_entries.begin(), m_entries.end(),
        [](FileSelectEntry *a, FileSelectEntry *b) { return *a < *b; });
}

FileSelect::~FileSelect() {
    for (auto *entry : m_entries)
        delete entry;
}

tsl::elm::Element* FileSelect::createUI() {
    auto frame = new FileSelectFrame();
    auto composite = new CompositeElement();

    composite->addElement(new tsl::elm::CustomDrawer([this](tsl::gfx::Renderer* renderer, u16 x, u16 y, u16 w, u16 h) {
        renderer->drawString(m_path.c_str(), false, 20, 80, 12, a(0xFFFF));
        if (m_failed)
            renderer->drawString("Unable to open directory", false, 20, 110, 18, a(0xF00F));
    }));

    auto list = new tsl::elm::List();

    for (auto *entry : m_entries) {
        auto item = new tsl::elm::ListItem(entry->label());
        item->setClickListener([entry, item](u64 keys) {
            if (keys & HidNpadButton_A) {
                entry->select();
                return true;
            }
            if (keys & HidNpadButton_Y) {
                entry->toggleFavorite();
                item->setText(entry->label());
                return true;
            }
            return false;
        });
        list->addItem(item);
    }

    composite->addElement(list, 40, 110, tsl::cfg::FramebufferWidth - 80);
    frame->setContent(composite);
    return frame;
}

void FileSelectFrame::draw(tsl::gfx::Renderer *renderer) {
    renderer->fillScreen(a({ 0x0, 0x0, 0x0, 0xD }));
    renderer->drawString("Select File...", false, 20, 50, 30, a(0xFFFF));
    renderer->drawRect(15, 720 - 73, tsl::cfg::FramebufferWidth - 30, 1, a(0xFFFF));
    renderer->drawString("\uE0E3  Favorite", false, 30, 693, 23, a(0xFFFF));

    if (this->m_contentElement != nullptr)
        this->m_contentElement->frame(renderer);
}