export type FcitxEvent = {
  type: 'COMMIT'
  data: string
} | {
  type: 'PREEDIT'
  data: {
    preedit: string
    cursorPos: number
  }
}
