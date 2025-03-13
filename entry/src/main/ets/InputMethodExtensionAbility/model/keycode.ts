import { KeyCode } from '@kit.InputKit'

const jsCodeMap = {
  ArrowDown: KeyCode.KEYCODE_DPAD_DOWN,
  ArrowLeft: KeyCode.KEYCODE_DPAD_LEFT,
  ArrowRight: KeyCode.KEYCODE_DPAD_RIGHT,
  ArrowUp: KeyCode.KEYCODE_DPAD_UP,
  Backspace: KeyCode.KEYCODE_DEL,
  End: KeyCode.KEYCODE_MOVE_END,
  Enter: KeyCode.KEYCODE_ENTER,
  Home: KeyCode.KEYCODE_MOVE_HOME,
}

export function convertCode(jsCode: string) {
  return jsCodeMap[jsCode] ?? ''
}
