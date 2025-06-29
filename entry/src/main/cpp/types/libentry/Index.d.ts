export const init: (bundle: string, resfile: string) => void
export const setCallback: (callback: (data: string) => void) => void
export const focusIn: (clientPreedit: boolean) => void
export const focusOut: () => void

export interface InputContextState {
  commit: string
  preedit: string
  cursorPos: number
  accepted: boolean
}
export const processKey: (unicode: number, keyCode: number, states: number, isRelease: boolean) => InputContextState
export const reset: () => InputContextState
export const toggle: () => void
export const selectCandidate: (index: number) => void
export const askCandidateActions: (index: number) => void
export const activateCandidateAction: (index: number, id: number) => void
export const activateStatusAreaAction: (id: number) => void
export const setCurrentInputMethod: (inputMethod: string) => void
export const scroll: (start: number, count: number) => void
