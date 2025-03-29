import sql from '@liushengyi/smartdb';
import { PasteboardEntry } from './PasteboardEntry';

export class PasteboardDao {
  constructor() {
  }

  @sql.SqlInsert(`INSERT OR ABORT INTO ${PasteboardEntry.TABLE_NAME} (text, pinned, timestamp, type, deleted, sensitive)
  VALUES (#{entry.text}, #{entry.pinned}, #{entry.timestamp}, #{entry.type}, #{entry.deleted}, #{entry.sensitive})`,
  { table: PasteboardEntry.TABLE_NAME, id: 'id' })
  insert(@sql.Param('entry') entry: PasteboardEntry): Promise<number> {
    return sql.PromiseNull()
  }

  @sql.SqlUpdate(`UPDATE ${PasteboardEntry.TABLE_NAME} SET timestamp=#{timestamp} WHERE id=#{id}`)
  updateTime(@sql.Param('id') id: number, @sql.Param('timestamp') timestamp: number): Promise<void> {
    return sql.PromiseNull()
  }

  @sql.SqlQuery(`SELECT * FROM ${PasteboardEntry.TABLE_NAME} WHERE rowId=#{rowId} AND deleted=0 LIMIT 1`)
  @sql.ReturnType(PasteboardEntry)
  get(@sql.Param('rowId') rowId: number): Promise<PasteboardEntry> {
    return sql.PromiseNull()
  }

  @sql.SqlQuery(`SELECT * FROM ${PasteboardEntry.TABLE_NAME} WHERE text=#{text} AND sensitive=#{sensitive} AND deleted=0 LIMIT 1`)
  @sql.ReturnType(PasteboardEntry)
  find(@sql.Param('text') text: string,
    @sql.Param('sensitive') sensitive: boolean = false): Promise<PasteboardEntry> {
    return sql.PromiseNull()
  }
}
