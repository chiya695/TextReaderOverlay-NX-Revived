#pragma once

#include <tesla.hpp>

class HelpMenu : public tsl::Gui {
public:
    tsl::elm::Element* createUI() override;

private:
    static void showKeybind(tsl::gfx::Renderer *renderer, s32 x, s32 y, const char* keys, const char* desc);
};
