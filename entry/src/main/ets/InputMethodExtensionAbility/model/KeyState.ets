import { KeyEvent } from "@kit.InputKit";

/**
 * translated from
 * [fcitx-utils/keysym.h](https://github.com/fcitx/fcitx5/blob/0346e58/src/lib/fcitx-utils/keysym.h)
 */
export enum KeyState {
  NO_STATE = 0,
  SHIFT = 1 << 0,
  CAPS_LOCK = 1 << 1,
  CTRL = 1 << 2,
  ALT = 1 << 3,
  MOD_1 = ALT,
  ALT_SHIFT = ALT | SHIFT,
  CTRL_SHIFT = CTRL | SHIFT,
  CTRL_ALT = CTRL | ALT,
  CTRL_ALT_SHIFT = CTRL | ALT | SHIFT,
  NUM_LOCK = 1 << 4,
  MOD_2 = NUM_LOCK,
  HYPER = 1 << 5,
  MOD_3 = HYPER,
  SUPER = 1 << 6,
  MOD_4 = SUPER,
  MOD_5 = 1 << 7,
  MOUSE_PRESSED = 1 << 8,
  HANDLED_MASK = 1 << 24,
  IGNORED_MASK = 1 << 25,
  SUPER_2 = 1 << 26,  // Gtk virtual Super
  HYPER_2 = 1 << 27,  // Gtk virtual Hyper
  META = 1 << 28,
  VIRTUAL = 1 << 29,

  REPEAT = 1 << 31,
  USED_MASK = 0x5c001fff,
  SIMPLE_MASK = CTRL_ALT_SHIFT | SUPER | SUPER_2 | HYPER | META
}

export function statesFromKeyEvent(e: KeyEvent): number {
  let states = KeyState.NO_STATE
  if (e.altKey) states |= KeyState.ALT
  if (e.ctrlKey) states |= KeyState.CTRL
  if (e.shiftKey) states |= KeyState.SHIFT
  if (e.capsLock) states |= KeyState.CAPS_LOCK
  if (e.numLock) states |= KeyState.NUM_LOCK
  if (e.logoKey) states |= KeyState.META
  return states
}