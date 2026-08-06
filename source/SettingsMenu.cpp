#include <SettingsMenu.hpp>

#include <Config.hpp>
#include <Overlay.hpp>

tsl::elm::Element* SettingsMenu::createUI() {
    auto *frame = new tsl::elm::OverlayFrame("Settings", TEXT_READER_VERSION);
    auto *list = new tsl::elm::List();

    const json config = Config::read();
    const bool wordWrap = config.value("settings", json::object()).value("word_wrap", true);

    auto *wordWrapToggle = new tsl::elm::ToggleListItem("Automatic Line Wrapping", wordWrap);
    wordWrapToggle->setStateChangedListener([](bool enabled) {
        Config::update([enabled](json &config) {
            config["settings"]["word_wrap"] = enabled;
        });
    });
    list->addItem(wordWrapToggle);

    frame->setContent(list);
    return frame;
}
