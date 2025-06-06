#pragma once

#include <string>
#include <cstdint>
#include "napi/native_api.h"

extern napi_threadsafe_function tsfn;
void notify_main_async(std::string str);

namespace fcitx {
struct InputContextState {
    std::string commit;
    std::string preedit;
    int cursorPos;
    bool accepted;
};

void init(const std::string &bundle, const std::string &resfile);
void focusIn(bool clientPreedit);
void focusOut();
InputContextState reset();
InputContextState processKey(uint32_t unicode, int32_t keyCode, bool isRelease);
void selectCandidate(int index);
void askCandidateAction(int index);
void activateCandidateAction(int index, int id);
void activateStatusAreaAction(int id);
void toggle();
void setCurrentInputMethod(const std::string &inputMethod);
void scroll(int start, int count);
} // namespace fcitx
