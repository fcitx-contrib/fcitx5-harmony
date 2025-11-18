import { PasteboardEntry } from './PasteboardEntry';
import relationalStore from '@ohos.data.relationalStore';
import { rdbStore } from './PasteboardDatabase';

export class PasteboardDao {
  constructor() {
  }

  async insert(entry: PasteboardEntry): Promise<number> {
    let res = 0
    const bucket: relationalStore.ValuesBucket = {
      id: entry.id,
      text: entry.text,
      pinned: entry.pinned,
      timestamp: entry.timestamp,
      type: entry.type,
      deleted: entry.deleted,
      sensitive: entry.sensitive
    }
    rdbStore?.insert(PasteboardEntry.TABLE_NAME, bucket, (err, rowId) => {
      if (err) {
        return
      }
      res = rowId
    })
    return res
  }

  async updatePinStatus(id: number, pinned: boolean): Promise<void> {
    rdbStore?.executeSql(`UPDATE ${PasteboardEntry.TABLE_NAME} SET pinned=${pinned} WHERE id=${id}`)
  }

  async updateText(id: number, text: string): Promise<void> {
    rdbStore?.executeSql(`UPDATE ${PasteboardEntry.TABLE_NAME} SET text=${text} WHERE id=${id}`)
  }

  async updateTime(id: number, timestamp: number): Promise<void> {
    rdbStore?.executeSql(`UPDATE ${PasteboardEntry.TABLE_NAME} SET timestamp=${timestamp} WHERE id=${id}`)
  }

  async itemCount(): Promise<number> {
    let res = 0
    rdbStore?.querySql(`SELECT COUNT(*) FROM ${PasteboardEntry.TABLE_NAME} WHERE deleted=0`, (err, resultSet) => {
      if (err) return
      if (resultSet.goToFirstRow()) {
        res = resultSet.getLong(0)
      }
      resultSet.close()
    })
    return res
  }

  async getById(id: number): Promise<PasteboardEntry> {
    let res = new PasteboardEntry('')
    rdbStore?.querySql(`SELECT * FROM ${PasteboardEntry.TABLE_NAME} WHERE id=${id} AND deleted=0 LIMIT 1`, (err, resultSet) => {
      if (err) return
      while (resultSet.goToNextRow()) {
        res.id = resultSet.getLong(resultSet.getColumnIndex('id'))
        res.text = resultSet.getString(resultSet.getColumnIndex('text'))
        res.pinned = resultSet.getValue(resultSet.getColumnIndex('pinned')) as boolean
        res.timestamp = resultSet.getLong(resultSet.getColumnIndex('timestamp'))
        res.type = resultSet.getString(resultSet.getColumnIndex('type'))
        res.deleted = resultSet.getValue(resultSet.getColumnIndex('deleted')) as boolean
        res.sensitive = resultSet.getValue(resultSet.getColumnIndex('sensitive')) as boolean
      }
      resultSet.close()
    })
    return res
  }

  async getByRowId(rowId: number): Promise<PasteboardEntry> {
    let res = new PasteboardEntry('')
    rdbStore?.querySql(`SELECT * FROM ${PasteboardEntry.TABLE_NAME} WHERE id=${rowId} AND deleted=0 LIMIT 1`, (err, resultSet) => {
      if (err) return
      while (resultSet.goToNextRow()) {
        res.id = resultSet.getLong(resultSet.getColumnIndex('id'))
        res.text = resultSet.getString(resultSet.getColumnIndex('text'))
        res.pinned = resultSet.getValue(resultSet.getColumnIndex('pinned')) as number === 1
        res.timestamp = resultSet.getLong(resultSet.getColumnIndex('timestamp'))
        res.type = resultSet.getString(resultSet.getColumnIndex('type'))
        res.deleted = resultSet.getValue(resultSet.getColumnIndex('deleted')) as number === 1
        res.sensitive = resultSet.getValue(resultSet.getColumnIndex('sensitive')) as number === 1
      }
      resultSet.close()
    })
    return res
  }

  async haveUnpinned(): Promise<boolean> {
    let res = false
    rdbStore?.querySql(`SELECT EXISTS(SELECT 1 FROM ${PasteboardEntry.TABLE_NAME} WHERE pinned=0 AND deleted=0)`, (err, resultSet) => {
      if (err) return
      if (resultSet.goToFirstRow()) {
        res = resultSet.getValue(0) as number === 1
      }
      resultSet.close()
    })
    return res
  }

  async getAllUnpinned(): Promise<Array<PasteboardEntry>> {
    const res: Array<PasteboardEntry> = []
    rdbStore?.querySql(`SELECT * FROM ${PasteboardEntry.TABLE_NAME} WHERE pinned=0 AND deleted=0`, (err, resultSet) => {
      if (err) return
      if (resultSet.goToFirstRow()) {
        do {
          const text = resultSet.getString(resultSet.getColumnIndex('text'))
          const entry = new PasteboardEntry(text)
          entry.pinned = resultSet.getValue(resultSet.getColumnIndex('pinned')) as number === 1
          entry.timestamp = resultSet.getLong(resultSet.getColumnIndex('timestamp'))
          entry.type = resultSet.getString(resultSet.getColumnIndex('type'))
          entry.deleted = resultSet.getValue(resultSet.getColumnIndex('deleted')) as number === 1
          entry.sensitive = resultSet.getValue(resultSet.getColumnIndex('sensitive')) as number === 1
          res.push(entry)
        } while (resultSet.goToNextRow())
      }
    })
    return res
  }

  async getEntries(offset: number, limit: number): Promise<Array<PasteboardEntry>> {
    let res: Array<PasteboardEntry> = []
    rdbStore?.querySql(`SELECT * FROM ${PasteboardEntry.TABLE_NAME} WHERE deleted=0` +
        `ORDER BY pinned DESC, timestamp DESC LIMIT ${limit} OFFSET ${offset}`, (err, resultSet) => {
      if (err) return
      if (resultSet.goToFirstRow()) {
        do {
          const text = resultSet.getString(resultSet.getColumnIndex('text'))
          const entry = new PasteboardEntry(text)
          entry.pinned = resultSet.getValue(resultSet.getColumnIndex('pinned')) as number === 1
          entry.timestamp = resultSet.getLong(resultSet.getColumnIndex('timestamp'))
          entry.type = resultSet.getString(resultSet.getColumnIndex('type'))
          entry.deleted = resultSet.getValue(resultSet.getColumnIndex('deleted')) as number === 1
          entry.sensitive = resultSet.getValue(resultSet.getColumnIndex('sensitive')) as number === 1
          res.push(entry)
        } while (resultSet.goToNextRow())
      }
    })
    return res
  }

  async find(text: string, sensitive: boolean = false): Promise<PasteboardEntry | null> {
    let res: PasteboardEntry | null = null
    rdbStore?.querySql(`SELECT * FROM ${PasteboardEntry.TABLE_NAME} WHERE text=${text} AND sensitive=${sensitive} AND deleted=0 LIMIT 1`, (err, resultSet) => {
      if (err) return
      if (resultSet.goToFirstRow()) {
        const text = resultSet.getString(resultSet.getColumnIndex('text'))
        const entry = new PasteboardEntry(text)
        entry.pinned = resultSet.getValue(resultSet.getColumnIndex('pinned')) as number === 1
        entry.timestamp = resultSet.getLong(resultSet.getColumnIndex('timestamp'))
        entry.type = resultSet.getString(resultSet.getColumnIndex('type'))
        entry.deleted = resultSet.getValue(resultSet.getColumnIndex('deleted')) as number === 1
        entry.sensitive = resultSet.getValue(resultSet.getColumnIndex('sensitive')) as number === 1
      }
    })
    return res
  }

  async findAllIds(): Promise<Array<number>> {
    let res: Array<number> = []
    rdbStore?.querySql(`SELECT id FROM ${PasteboardEntry.TABLE_NAME} WHERE deleted=0`, (err, resultSet) => {
      if (err) return
      if (resultSet.goToFirstRow()) {
        do {
          const id = resultSet.getValue(resultSet.getColumnIndex('id')) as number
          res.push(id)
        } while (resultSet.goToNextRow())
      }
    })
    return res
  }

  async findUnpinnedIds(): Promise<Array<number>> {
    let res: Array<number> = []
    rdbStore?.querySql(`SELECT id FROM ${PasteboardEntry.TABLE_NAME} WHERE pinned=0 AND deleted=0`, (err, resultSet) => {
      if (err) return
      if (resultSet.goToFirstRow()) {
        do {
          const id = resultSet.getValue(resultSet.getColumnIndex('id')) as number
          res.push(id)
        } while (resultSet.goToNextRow())
      }
    })
    return res
  }

  async markAsDeleted(...ids: number[]): Promise<void> {
    rdbStore?.executeSql(`UPDATE ${PasteboardEntry.TABLE_NAME} SET deleted=1 WHERE id in (${ids})`)
  }

  async markUnpinnedAsDeletedEarlierThan(timestamp: number): Promise<void> {
    rdbStore?.executeSql(`UPDATE ${PasteboardEntry.TABLE_NAME} SET deleted=1 WHERE timestamp<${timestamp} AND pinned=0 AND deleted=0`)
  }

  async undoDelete(...ids: number[]): Promise<void> {
    rdbStore?.executeSql(`UPDATE ${PasteboardEntry.TABLE_NAME} SET deleted=0 WHERE id in (${ids}) AND deleted=1`)
  }

  async realDelete(): Promise<void> {
    rdbStore?.executeSql(`DELETE FROM ${PasteboardEntry.TABLE_NAME} WHERE deleted=1`)
  }

  async clearAllTables(): Promise<void> {
    rdbStore?.executeSql(`DELETE FROM ${PasteboardEntry.TABLE_NAME}`)
  }
}
