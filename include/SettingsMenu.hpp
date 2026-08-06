#pragma once

#include <tesla.hpp>

class SettingsMenu : public tsl::Gui {
public:
    tsl::elm::Element* createUI() override;
};
