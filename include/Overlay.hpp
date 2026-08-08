#pragma once

#include <tesla.hpp>

#include <MainMenu.hpp>

#ifndef TEXT_READER_VERSION
#define TEXT_READER_VERSION "v1.9.1"
#endif

class TextReaderOverlay : public tsl::Overlay {
public:
    void initServices() override {
        m_sdMounted = R_SUCCEEDED(fsdevMountSdmc());
    }

    void exitServices() override {
        if (m_sdMounted)
            fsdevUnmountDevice("sdmc");
    }

    std::unique_ptr<tsl::Gui> loadInitialGui() override {
        return initially<MainMenu>();
    }

private:
    bool m_sdMounted = false;
};
