import { pasteboard } from '@kit.BasicServicesKit';
import sql from '@liushengyi/smartdb';

export class PasteboardEntry {
  static TABLE_NAME = 'clipboard'
  @sql.SqlColumn(sql.ColumnType.INTEGER)
  id: number = 0
  @sql.SqlColumn(sql.ColumnType.TEXT)
  text: string
  @sql.SqlColumn(sql.ColumnType.INTEGER)
  pinned: boolean = false
  @sql.SqlColumn(sql.ColumnType.INTEGER)
  timestamp: number = Date.now()
  @sql.SqlColumn(sql.ColumnType.TEXT)
  type: string = pasteboard.MIMETYPE_TEXT_PLAIN
  @sql.SqlColumn(sql.ColumnType.INTEGER)
  deleted: boolean = false
  @sql.SqlColumn(sql.ColumnType.INTEGER)
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
