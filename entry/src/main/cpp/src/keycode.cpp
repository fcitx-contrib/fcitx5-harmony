#include "keycode.h"
#include "ohkeycode.h"

namespace fcitx {
static struct {
    OhKeyCode ohKeyCode;
    KeySym sym;
} sym_mappings[] = {
    // alphabet
    {KEYCODE_A, FcitxKey_a},
    {KEYCODE_B, FcitxKey_b},
    {KEYCODE_C, FcitxKey_c},
    {KEYCODE_D, FcitxKey_d},
    {KEYCODE_E, FcitxKey_e},
    {KEYCODE_F, FcitxKey_f},
    {KEYCODE_G, FcitxKey_g},
    {KEYCODE_H, FcitxKey_h},
    {KEYCODE_I, FcitxKey_i},
    {KEYCODE_J, FcitxKey_j},
    {KEYCODE_K, FcitxKey_k},
    {KEYCODE_L, FcitxKey_l},
    {KEYCODE_M, FcitxKey_m},
    {KEYCODE_N, FcitxKey_n},
    {KEYCODE_O, FcitxKey_o},
    {KEYCODE_P, FcitxKey_p},
    {KEYCODE_Q, FcitxKey_q},
    {KEYCODE_R, FcitxKey_r},
    {KEYCODE_S, FcitxKey_s},
    {KEYCODE_T, FcitxKey_t},
    {KEYCODE_U, FcitxKey_u},
    {KEYCODE_V, FcitxKey_v},
    {KEYCODE_W, FcitxKey_w},
    {KEYCODE_X, FcitxKey_x},
    {KEYCODE_Y, FcitxKey_y},
    {KEYCODE_Z, FcitxKey_z},

    // number
    {KEYCODE_0, FcitxKey_0},
    {KEYCODE_1, FcitxKey_1},
    {KEYCODE_2, FcitxKey_2},
    {KEYCODE_3, FcitxKey_3},
    {KEYCODE_4, FcitxKey_4},
    {KEYCODE_5, FcitxKey_5},
    {KEYCODE_6, FcitxKey_6},
    {KEYCODE_7, FcitxKey_7},
    {KEYCODE_8, FcitxKey_8},
    {KEYCODE_9, FcitxKey_9},

    // symbol
    {KEYCODE_GRAVE, FcitxKey_grave},
    {KEYCODE_BACKSLASH, FcitxKey_backslash},
    {KEYCODE_LEFT_BRACKET, FcitxKey_bracketleft},
    {KEYCODE_RIGHT_BRACKET, FcitxKey_bracketright},
    {KEYCODE_COMMA, FcitxKey_comma},
    {KEYCODE_PERIOD, FcitxKey_period},
    {KEYCODE_EQUALS, FcitxKey_equal},
    {KEYCODE_MINUS, FcitxKey_minus},
    {KEYCODE_APOSTROPHE, FcitxKey_apostrophe},
    {KEYCODE_SEMICOLON, FcitxKey_semicolon},
    {KEYCODE_SLASH, FcitxKey_slash},

    // keypad
    {KEYCODE_NUMPAD_0, FcitxKey_KP_0},
    {KEYCODE_NUMPAD_1, FcitxKey_KP_1},
    {KEYCODE_NUMPAD_2, FcitxKey_KP_2},
    {KEYCODE_NUMPAD_3, FcitxKey_KP_3},
    {KEYCODE_NUMPAD_4, FcitxKey_KP_4},
    {KEYCODE_NUMPAD_5, FcitxKey_KP_5},
    {KEYCODE_NUMPAD_6, FcitxKey_KP_6},
    {KEYCODE_NUMPAD_7, FcitxKey_KP_7},
    {KEYCODE_NUMPAD_8, FcitxKey_KP_8},
    {KEYCODE_NUMPAD_9, FcitxKey_KP_9},
    {KEYCODE_NUMPAD_COMMA, FcitxKey_KP_Separator},
    {KEYCODE_NUMPAD_DOT, FcitxKey_KP_Decimal},
    {KEYCODE_NUMPAD_EQUALS, FcitxKey_KP_Equal},
    {KEYCODE_NUMPAD_SUBTRACT, FcitxKey_KP_Subtract},
    {KEYCODE_NUMPAD_MULTIPLY, FcitxKey_KP_Multiply},
    {KEYCODE_NUMPAD_ADD, FcitxKey_KP_Add},
    {KEYCODE_NUMPAD_DIVIDE, FcitxKey_KP_Divide},

    // special
    {KEYCODE_DEL, FcitxKey_BackSpace},
    {KEYCODE_NUMPAD_ENTER, FcitxKey_KP_Enter},
    {KEYCODE_ESCAPE, FcitxKey_Escape},
    {KEYCODE_FORWARD_DEL, FcitxKey_Delete},
    {KEYCODE_ENTER, FcitxKey_Return},
    {KEYCODE_SPACE, FcitxKey_space},
    {KEYCODE_TAB, FcitxKey_Tab},
    {KEYCODE_SCROLL_LOCK, FcitxKey_Scroll_Lock},
    {KEYCODE_INSERT, FcitxKey_Insert},

    // function
    {KEYCODE_F1, FcitxKey_F1},
    {KEYCODE_F2, FcitxKey_F2},
    {KEYCODE_F3, FcitxKey_F3},
    {KEYCODE_F4, FcitxKey_F4},
    {KEYCODE_F5, FcitxKey_F5},
    {KEYCODE_F6, FcitxKey_F6},
    {KEYCODE_F7, FcitxKey_F7},
    {KEYCODE_F8, FcitxKey_F8},
    {KEYCODE_F9, FcitxKey_F9},
    {KEYCODE_F10, FcitxKey_F10},
    {KEYCODE_F11, FcitxKey_F11},
    {KEYCODE_F12, FcitxKey_F12},

    // cursor
    {KEYCODE_DPAD_UP, FcitxKey_Up},
    {KEYCODE_DPAD_DOWN, FcitxKey_Down},
    {KEYCODE_DPAD_LEFT, FcitxKey_Left},
    {KEYCODE_DPAD_RIGHT, FcitxKey_Right},

    {KEYCODE_PAGE_UP, FcitxKey_Page_Up},
    {KEYCODE_PAGE_DOWN, FcitxKey_Page_Down},
    {KEYCODE_MOVE_HOME, FcitxKey_Home},
    {KEYCODE_MOVE_END, FcitxKey_End},

    // modifiers
    {KEYCODE_CAPS_LOCK, FcitxKey_Caps_Lock},
    {KEYCODE_META_LEFT, FcitxKey_Meta_L},
    {KEYCODE_META_RIGHT, FcitxKey_Meta_R},
    {KEYCODE_CTRL_LEFT, FcitxKey_Control_L},
    {KEYCODE_CTRL_RIGHT, FcitxKey_Control_R},
    {KEYCODE_FN, FcitxKey_function},
    {KEYCODE_ALT_LEFT, FcitxKey_Alt_L},
    {KEYCODE_ALT_RIGHT, FcitxKey_Alt_R},
    {KEYCODE_SHIFT_LEFT, FcitxKey_Shift_L},
    {KEYCODE_SHIFT_RIGHT, FcitxKey_Shift_R},
};

static struct {
    OhKeyCode ohKeyCode;
    uint16_t linuxKeyCode;
} code_mappings[] = {
    // alphabet
    {KEYCODE_A, 30},
    {KEYCODE_B, 48},
    {KEYCODE_C, 46},
    {KEYCODE_D, 32},
    {KEYCODE_E, 18},
    {KEYCODE_F, 33},
    {KEYCODE_G, 34},
    {KEYCODE_H, 35},
    {KEYCODE_I, 23},
    {KEYCODE_J, 36},
    {KEYCODE_K, 37},
    {KEYCODE_L, 38},
    {KEYCODE_M, 50},
    {KEYCODE_N, 49},
    {KEYCODE_O, 24},
    {KEYCODE_P, 25},
    {KEYCODE_Q, 16},
    {KEYCODE_R, 19},
    {KEYCODE_S, 31},
    {KEYCODE_T, 20},
    {KEYCODE_U, 22},
    {KEYCODE_V, 47},
    {KEYCODE_W, 17},
    {KEYCODE_X, 45},
    {KEYCODE_Y, 21},
    {KEYCODE_Z, 44},

    // number
    {KEYCODE_0, 11},
    {KEYCODE_1, 2},
    {KEYCODE_2, 3},
    {KEYCODE_3, 4},
    {KEYCODE_4, 5},
    {KEYCODE_5, 6},
    {KEYCODE_6, 7},
    {KEYCODE_7, 8},
    {KEYCODE_8, 9},
    {KEYCODE_9, 10},

    // symbol
    {KEYCODE_GRAVE, 41},
    {KEYCODE_BACKSLASH, 43},
    {KEYCODE_LEFT_BRACKET, 26},
    {KEYCODE_RIGHT_BRACKET, 27},
    {KEYCODE_COMMA, 51},
    {KEYCODE_PERIOD, 52},
    {KEYCODE_EQUALS, 13},
    {KEYCODE_MINUS, 12},
    {KEYCODE_APOSTROPHE, 40},
    {KEYCODE_SEMICOLON, 39},
    {KEYCODE_SLASH, 53},

    // keypad
    {KEYCODE_NUMPAD_0, 82},
    {KEYCODE_NUMPAD_1, 79},
    {KEYCODE_NUMPAD_2, 80},
    {KEYCODE_NUMPAD_3, 81},
    {KEYCODE_NUMPAD_4, 75},
    {KEYCODE_NUMPAD_5, 76},
    {KEYCODE_NUMPAD_6, 77},
    {KEYCODE_NUMPAD_7, 71},
    {KEYCODE_NUMPAD_8, 72},
    {KEYCODE_NUMPAD_9, 73},
    {KEYCODE_NUMPAD_COMMA, 121},
    {KEYCODE_NUMPAD_DOT, 83},
    {KEYCODE_NUMPAD_EQUALS, 117},
    {KEYCODE_NUMPAD_SUBTRACT, 74},
    {KEYCODE_NUMPAD_MULTIPLY, 55},
    {KEYCODE_NUMPAD_ADD, 78},
    {KEYCODE_NUMPAD_DIVIDE, 98},

    // special
    {KEYCODE_DEL, 14},
    {KEYCODE_NUMPAD_ENTER, 96},
    {KEYCODE_ESCAPE, 1},
    {KEYCODE_FORWARD_DEL, 111},
    {KEYCODE_ENTER, 28},
    {KEYCODE_SPACE, 57},
    {KEYCODE_TAB, 15},
    {KEYCODE_SCROLL_LOCK, 70},
    {KEYCODE_INSERT, 110},

    // function
    {KEYCODE_F1, 59},
    {KEYCODE_F2, 60},
    {KEYCODE_F3, 61},
    {KEYCODE_F4, 62},
    {KEYCODE_F5, 63},
    {KEYCODE_F6, 64},
    {KEYCODE_F7, 65},
    {KEYCODE_F8, 66},
    {KEYCODE_F9, 67},
    {KEYCODE_F10, 68},
    {KEYCODE_F11, 87},
    {KEYCODE_F12, 88},

    // cursor
    {KEYCODE_DPAD_UP, 103},
    {KEYCODE_DPAD_DOWN, 108},
    {KEYCODE_DPAD_LEFT, 105},
    {KEYCODE_DPAD_RIGHT, 106},

    {KEYCODE_PAGE_UP, 104},
    {KEYCODE_PAGE_DOWN, 109},
    {KEYCODE_MOVE_HOME, 102},
    {KEYCODE_MOVE_END, 107},

    // modifiers
    {KEYCODE_CAPS_LOCK, 58},
    {KEYCODE_META_LEFT, 125},
    {KEYCODE_META_RIGHT, 126},
    {KEYCODE_CTRL_LEFT, 29},
    {KEYCODE_CTRL_RIGHT, 97},
    {KEYCODE_FN, 0x1d0},
    {KEYCODE_ALT_LEFT, 56},
    {KEYCODE_ALT_RIGHT, 100},
    {KEYCODE_SHIFT_LEFT, 42},
    {KEYCODE_SHIFT_RIGHT, 54},
};

// For physical keyboard usage as only keyCode is available.
// TODO: consider modifiers.
KeySym ohKeyCodeToFcitxKeySym(int32_t keyCode) {
    for (const auto &pair : sym_mappings) {
        if (pair.ohKeyCode == keyCode) {
            return pair.sym;
        }
    }
    return {};
}

uint16_t ohKeyCodeToFcitxKeyCode(int32_t keyCode) {
    for (const auto &pair : code_mappings) {
        if (pair.ohKeyCode == keyCode) {
            return pair.linuxKeyCode + 8 /* evdev offset */;
        }
    }
    return 0;
}

Key ohKeyToFcitxKey(uint32_t unicode, int32_t keyCode) {
    auto keysym = unicode ? Key::keySymFromUnicode(unicode) : ohKeyCodeToFcitxKeySym(keyCode);
    return Key{keysym, KeyStates{}, ohKeyCodeToFcitxKeyCode(keyCode)};
}
} // namespace fcitx
