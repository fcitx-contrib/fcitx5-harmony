export const init: (bundle: string, resfile: string) => void
export const setCallback: (callback: (data: string) => void) => void
export const focusIn: (clientPreedit: boolean) => void
export const focusOut: () => void
export const reset: () => void

export interface InputContextState {
  commit: string
  preedit: string
  cursorPos: number
  accepted: boolean
}
export const processKey: (unicode: number, keyCode: number, isRelease: boolean) => InputContextState
export const selectCandidate: (index: number) => void
export const activateStatusAreaAction: (index: number) => void
