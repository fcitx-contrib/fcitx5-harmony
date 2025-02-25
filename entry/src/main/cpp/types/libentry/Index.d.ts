export const init: (bundle: string, resfile: string) => void
export const focusIn: (clientPreedit: boolean) => void
export const focusOut: () => void
export const reset: () => void
export const processKey: (unicode: number, keyCode: number, isRelease: boolean) => {
  commit: string
  preedit: string
  cursorPos: number
  accepted: boolean
}
