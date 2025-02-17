import { KeyCode } from '@kit.InputKit'

const jsCodeMap = {
  Backspace: KeyCode.KEYCODE_DEL
}

export function convertCode(jsCode: string) {
  return jsCodeMap[jsCode] ?? ''
}
