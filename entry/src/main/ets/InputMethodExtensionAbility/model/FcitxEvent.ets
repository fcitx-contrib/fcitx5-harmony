interface CommitEvent {
  type: 'COMMIT'
  data: string
}

interface PreeditData {
  preedit: string
  cursorPos: number
}

interface PreeditEvent {
  type: 'PREEDIT'
  data: PreeditData
}

export type FcitxEvent = CommitEvent | PreeditEvent
