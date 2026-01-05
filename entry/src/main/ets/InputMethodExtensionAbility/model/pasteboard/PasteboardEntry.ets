import { pasteboard } from '@kit.BasicServicesKit';

export class PasteboardEntry {
  static TABLE_NAME = 'clipboard'
  id: number = 0
  text: string
  pinned: boolean = false
  timestamp: number = Date.now()
  type: string = pasteboard.MIMETYPE_TEXT_PLAIN
  deleted: boolean = false
  sensitive: boolean = false

  constructor(text: string) {
    this.text = text
  }

  static fromPasteData(pasteData: pasteboard.PasteData): PasteboardEntry | undefined {
    const prop = pasteData.getProperty()
    const record = pasteData.getRecord(0)
    const str = record.plainText
    if (!str) return undefined
    const newEntry = new PasteboardEntry(str)
    newEntry.timestamp = prop.timestamp
    newEntry.type = prop.mimeTypes[0]
    return newEntry
  }
}
