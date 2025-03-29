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

  @sql.SqlUpdate(`UPDATE ${PasteboardEntry.TABLE_NAME} SET pinned=#{pinned} WHERE id=#{id}`)
  updatePinStatus(@sql.Param('id') id: number, @sql.Param('pinned') pinned: boolean): Promise<void> {
    return sql.PromiseNull()
  }

  @sql.SqlUpdate(`UPDATE ${PasteboardEntry.TABLE_NAME} SET text=#{text} WHERE id=#{id}`)
  updateText(@sql.Param('id') id: number, @sql.Param('text') text: string): Promise<void> {
    return sql.PromiseNull()
  }

  @sql.SqlUpdate(`UPDATE ${PasteboardEntry.TABLE_NAME} SET timestamp=#{timestamp} WHERE id=#{id}`)
  updateTime(@sql.Param('id') id: number, @sql.Param('timestamp') timestamp: number): Promise<void> {
    return sql.PromiseNull()
  }

  @sql.SqlQuery(`SELECT COUNT(*) FROM ${PasteboardEntry.TABLE_NAME} WHERE deleted=0`)
  @sql.ReturnType(Number)
  itemCount(): Promise<number> {
    return sql.PromiseNull()
  }

  @sql.SqlQuery(`SELECT * FROM ${PasteboardEntry.TABLE_NAME} WHERE id=#{id} AND deleted=0 LIMIT 1`)
  @sql.ReturnType(PasteboardEntry)
  getById(@sql.Param('id') id: number): Promise<PasteboardEntry> {
    return sql.PromiseNull()
  }

  @sql.SqlQuery(`SELECT * FROM ${PasteboardEntry.TABLE_NAME} WHERE rowId=#{rowId} AND deleted=0 LIMIT 1`)
  @sql.ReturnType(PasteboardEntry)
  getByRowId(@sql.Param('rowId') rowId: number): Promise<PasteboardEntry> {
    return sql.PromiseNull()
  }

  @sql.SqlQuery(`SELECT EXISTS(SELECT 1 FROM ${PasteboardEntry.TABLE_NAME} WHERE pinned=0 AND deleted=0)`)
  @sql.ReturnType(Boolean)
  haveUnpinned(): Promise<boolean> {
    return sql.PromiseNull()
  }

  @sql.SqlQuery(`SELECT * FROM ${PasteboardEntry.TABLE_NAME} WHERE pinned=0 AND deleted=0`)
  @sql.ReturnType([PasteboardEntry])
  getAllUnpinned(): Promise<Array<PasteboardEntry>> {
    return sql.PromiseNull()
  }

  @sql.SqlQuery(`SELECT * FROM ${PasteboardEntry.TABLE_NAME} WHERE deleted=0 ORDER BY pinned DESC, timestamp DESC`)
  @sql.ReturnType([PasteboardEntry])
  allEntries(): Promise<Array<PasteboardEntry>> {
    return sql.PromiseNull()
  }

  @sql.SqlQuery(`SELECT * FROM ${PasteboardEntry.TABLE_NAME} WHERE text=#{text} AND sensitive=#{sensitive} AND deleted=0 LIMIT 1`)
  @sql.ReturnType(PasteboardEntry)
  find(@sql.Param('text') text: string,
    @sql.Param('sensitive') sensitive: boolean = false): Promise<PasteboardEntry> {
    return sql.PromiseNull()
  }

  @sql.SqlQuery(`SELECT id FROM ${PasteboardEntry.TABLE_NAME} WHERE deleted=0`)
  @sql.ReturnType([Number])
  findAllIds(): Promise<Array<number>> {
    return sql.PromiseNull()
  }

  @sql.SqlQuery(`SELECT id FROM ${PasteboardEntry.TABLE_NAME} WHERE pinned=0 AND deleted=0`)
  @sql.ReturnType([Number])
  findUnpinnedIds(): Promise<Array<number>> {
    return sql.PromiseNull()
  }

  @sql.SqlUpdate(`UPDATE ${PasteboardEntry.TABLE_NAME} SET deleted=1 WHERE id in (#{ids})`)
  markAsDeleted(@sql.Param('ids') ...ids: number[]): Promise<void> {
    return sql.PromiseNull()
  }

  @sql.SqlUpdate(`UPDATE ${PasteboardEntry.TABLE_NAME} SET deleted=1 WHERE timestamp<#{timestamp} AND pinned=0 AND deleted=0`)
  markUnpinnedAsDeletedEarlierThan(@sql.Param('timestamp') timestamp: number): Promise<void> {
    return sql.PromiseNull()
  }

  @sql.SqlUpdate(`UPDATE ${PasteboardEntry.TABLE_NAME} SET deleted=0 WHERE id in (#{ids}) AND deleted=1`)
  undoDelete(@sql.Param(`ids`) ...ids: number[]): Promise<void> {
    return sql.PromiseNull()
  }

  @sql.SqlDelete(`DELETE FROM ${PasteboardEntry.TABLE_NAME} WHERE deleted=1`)
  realDelete(): Promise<void> {
    return sql.PromiseNull()
  }

  @sql.SqlDelete(`DELETE FROM ${PasteboardEntry.TABLE_NAME}`)
  clearAllTables(): Promise<void> {
    return sql.PromiseNull()
  }
}
