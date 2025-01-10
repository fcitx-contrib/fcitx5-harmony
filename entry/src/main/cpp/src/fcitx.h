#pragma once

#include <string>
#include <cstdint>

namespace fcitx {
struct InputContextState {
    std::string commit;
    std::string preedit;
    int cursorPos;
    bool accepted;
};

void init(const std::string &bundle, const std::string &resfile);
void focusIn();
void focusOut();
InputContextState processKey(uint32_t unicode, int32_t keyCode, bool isRelease);
} // namespace fcitx
