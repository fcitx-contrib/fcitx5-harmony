import { KeyCode } from '@kit.InputKit'

const jsCodeMap = {
  Backspace: KeyCode.KEYCODE_DEL,
  Enter: KeyCode.KEYCODE_ENTER,
}

export function convertCode(jsCode: string) {
  return jsCodeMap[jsCode] ?? ''
}
