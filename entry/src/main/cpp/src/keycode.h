#pragma once

#include <cstdint>
#include <fcitx-utils/key.h>

namespace fcitx {
Key ohKeyToFcitxKey(uint32_t unicode, int32_t keyCode, uint32_t states);
}
