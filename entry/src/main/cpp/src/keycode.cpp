#include <cstdint>
#include <linux/input-event-codes.h>
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
    {KEYCODE_A, KEY_A},
    {KEYCODE_B, KEY_B},
    {KEYCODE_C, KEY_C},
    {KEYCODE_D, KEY_D},
    {KEYCODE_E, KEY_E},
    {KEYCODE_F, KEY_F},
    {KEYCODE_G, KEY_G},
    {KEYCODE_H, KEY_H},
    {KEYCODE_I, KEY_I},
    {KEYCODE_J, KEY_J},
    {KEYCODE_K, KEY_K},
    {KEYCODE_L, KEY_L},
    {KEYCODE_M, KEY_M},
    {KEYCODE_N, KEY_N},
    {KEYCODE_O, KEY_O},
    {KEYCODE_P, KEY_P},
    {KEYCODE_Q, KEY_Q},
    {KEYCODE_R, KEY_R},
    {KEYCODE_S, KEY_S},
    {KEYCODE_T, KEY_T},
    {KEYCODE_U, KEY_U},
    {KEYCODE_V, KEY_V},
    {KEYCODE_W, KEY_W},
    {KEYCODE_X, KEY_X},
    {KEYCODE_Y, KEY_Y},
    {KEYCODE_Z, KEY_Z},

    // number
    {KEYCODE_0, KEY_0},
    {KEYCODE_1, KEY_1},
    {KEYCODE_2, KEY_2},
    {KEYCODE_3, KEY_3},
    {KEYCODE_4, KEY_4},
    {KEYCODE_5, KEY_5},
    {KEYCODE_6, KEY_6},
    {KEYCODE_7, KEY_7},
    {KEYCODE_8, KEY_8},
    {KEYCODE_9, KEY_9},

    // symbol
    {KEYCODE_GRAVE, KEY_GRAVE},
    {KEYCODE_BACKSLASH, KEY_BACKSLASH},
    {KEYCODE_LEFT_BRACKET, KEY_LEFTBRACE},
    {KEYCODE_RIGHT_BRACKET, KEY_RIGHTBRACE},
    {KEYCODE_COMMA, KEY_COMMA},
    {KEYCODE_PERIOD, KEY_DOT},
    {KEYCODE_EQUALS, KEY_EQUAL},
    {KEYCODE_MINUS, KEY_MINUS},
    {KEYCODE_APOSTROPHE, KEY_APOSTROPHE},
    {KEYCODE_SEMICOLON, KEY_SEMICOLON},
    {KEYCODE_SLASH, KEY_SLASH},

    // keypad
    {KEYCODE_NUMPAD_0, KEY_KP0},
    {KEYCODE_NUMPAD_1, KEY_KP1},
    {KEYCODE_NUMPAD_2, KEY_KP2},
    {KEYCODE_NUMPAD_3, KEY_KP3},
    {KEYCODE_NUMPAD_4, KEY_KP4},
    {KEYCODE_NUMPAD_5, KEY_KP5},
    {KEYCODE_NUMPAD_6, KEY_KP6},
    {KEYCODE_NUMPAD_7, KEY_KP7},
    {KEYCODE_NUMPAD_8, KEY_KP8},
    {KEYCODE_NUMPAD_9, KEY_KP9},
    {KEYCODE_NUMPAD_COMMA, KEY_KPCOMMA},
    {KEYCODE_NUMPAD_DOT, KEY_KPDOT},
    {KEYCODE_NUMPAD_EQUALS, KEY_KPEQUAL},
    {KEYCODE_NUMPAD_SUBTRACT, KEY_KPMINUS},
    {KEYCODE_NUMPAD_MULTIPLY, KEY_KPASTERISK},
    {KEYCODE_NUMPAD_ADD, KEY_KPPLUS},
    {KEYCODE_NUMPAD_DIVIDE, KEY_KPSLASH},

    // special
    {KEYCODE_DEL, KEY_BACKSPACE},
    {KEYCODE_NUMPAD_ENTER, KEY_KPENTER},
    {KEYCODE_ESCAPE, KEY_ESC},
    {KEYCODE_FORWARD_DEL, KEY_DELETE},
    {KEYCODE_ENTER, KEY_ENTER},
    {KEYCODE_SPACE, KEY_SPACE},
    {KEYCODE_TAB, KEY_TAB},
    {KEYCODE_SCROLL_LOCK, KEY_SCROLLLOCK},
    {KEYCODE_INSERT, KEY_INSERT},

    // function
    {KEYCODE_F1, KEY_F1},
    {KEYCODE_F2, KEY_F2},
    {KEYCODE_F3, KEY_F3},
    {KEYCODE_F4, KEY_F4},
    {KEYCODE_F5, KEY_F5},
    {KEYCODE_F6, KEY_F6},
    {KEYCODE_F7, KEY_F7},
    {KEYCODE_F8, KEY_F8},
    {KEYCODE_F9, KEY_F9},
    {KEYCODE_F10, KEY_F10},
    {KEYCODE_F11, KEY_F11},
    {KEYCODE_F12, KEY_F12},

    // cursor
    {KEYCODE_DPAD_UP, KEY_UP},
    {KEYCODE_DPAD_DOWN, KEY_DOWN},
    {KEYCODE_DPAD_LEFT, KEY_LEFT},
    {KEYCODE_DPAD_RIGHT, KEY_RIGHT},

    {KEYCODE_PAGE_UP, KEY_PAGEUP},
    {KEYCODE_PAGE_DOWN, KEY_PAGEDOWN},
    {KEYCODE_MOVE_HOME, KEY_HOME},
    {KEYCODE_MOVE_END, KEY_END},

    // modifiers
    {KEYCODE_CAPS_LOCK, KEY_CAPSLOCK},
    {KEYCODE_META_LEFT, KEY_LEFTMETA},
    {KEYCODE_META_RIGHT, KEY_RIGHTMETA},
    {KEYCODE_CTRL_LEFT, KEY_LEFTCTRL},
    {KEYCODE_CTRL_RIGHT, KEY_RIGHTCTRL},
    {KEYCODE_FN, KEY_FN},
    {KEYCODE_ALT_LEFT, KEY_LEFTALT},
    {KEYCODE_ALT_RIGHT, KEY_RIGHTALT},
    {KEYCODE_SHIFT_LEFT, KEY_LEFTSHIFT},
    {KEYCODE_SHIFT_RIGHT, KEY_RIGHTSHIFT},
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

Key ohKeyToFcitxKey(uint32_t unicode, int32_t keyCode, uint32_t states) {
    auto keysym = unicode ? Key::keySymFromUnicode(unicode) : ohKeyCodeToFcitxKeySym(keyCode);
    return Key{keysym, KeyStates{states}, ohKeyCodeToFcitxKeyCode(keyCode)};
}
} // namespace fcitx
