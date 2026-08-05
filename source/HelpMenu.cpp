#include <HelpMenu.hpp>

#include <Overlay.hpp>

tsl::elm::Element* HelpMenu::createUI() {
    auto *frame = new tsl::elm::OverlayFrame("Help", TEXT_READER_VERSION);

    frame->setContent(new tsl::elm::CustomDrawer([](tsl::gfx::Renderer *renderer, s32 x, s32 y, s32 w, s32 h) {
        constexpr s32 titleSize = 25;
        constexpr s32 rowHeight = 29;
        const s32 left = x + 5;
        s32 cursorY = y + titleSize;

        renderer->drawString("File Browser Controls", false, left, cursorY, titleSize, a(0xFFFF));
        cursorY += 38;
        showKeybind(renderer, left, cursorY, "\uE0A3", "Toggle Favorite");

        cursorY += 52;
        renderer->drawString("Reader Controls", false, left, cursorY, titleSize, a(0xFFFF));
        cursorY += 38;

        const std::pair<const char*, const char*> controls[] = {
            { "\uE081", "Scroll" },
            { "\uE085 \uE081", "Scroll Faster" },
            { "\uE086 \uE081 \uE091 \uE090", "Scroll Even Faster" },
            { "\uE086 \uE081 \uE092 \uE093", "Scroll to Top / Bottom" },
            { "\uE07D", "Adjust Font Size" },
            { "\uE082", "Scroll Sideways" },
            { "\uE08B", "Beginning of Line" },
            { "\uE0A3", "Toggle Bookmark" },
            { "\uE0A4 \uE0A5", "Prev/Next Bookmark" },
            { "\uE0A2", "Hide Overlay" },
        };

        for (const auto &[keys, description] : controls) {
            showKeybind(renderer, left, cursorY, keys, description);
            cursorY += rowHeight;
        }
    }));

    return frame;
}

void HelpMenu::showKeybind(tsl::gfx::Renderer *renderer, s32 x, s32 y, const char* keys, const char* desc) {
    renderer->drawString(keys, false, x, y, 21, a(0xFFFF));
    renderer->drawString(desc, false, x + 125, y, 21, a(0xFFFF));
}
