#include <string>
#include <cstdint>

namespace fcitx {
void init(const std::string &bundle, const std::string &resfile);
void focusIn();
void focusOut();
void processKey(uint32_t unicode, int32_t keyCode, bool isRelease);
} // namespace fcitx
