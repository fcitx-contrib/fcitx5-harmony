import relationalStore from '@ohos.data.relationalStore';
import { BusinessError } from '@kit.BasicServicesKit';

const STORE_NAME = 'clbdb'

const STORE_CONFIG: relationalStore.StoreConfig = {
  name: STORE_NAME,
  securityLevel: relationalStore.SecurityLevel.S1
}

const SQL_CREATE_TABLE = "CREATE TABLE IF NOT EXISTS clipboard (" +
  "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, " +
  "text TEXT NOT NULL, " +
  "pinned INTEGER NOT NULL, timestamp INTEGER NOT NULL DEFAULT -1, " +
  "type TEXT NOT NULL DEFAULT 'text/plain', " +
  "deleted INTEGER NOT NULL DEFAULT 0, " +
  "sensitive INTEGER NOT NULL DEFAULT 0)"

/**
 * Must call {@method PasteBoardDatabase.init} before call this
 */
export let rdbStore: relationalStore.RdbStore | undefined = undefined

export class PasteboardDatabase {
  static VERSION = 1

  static init(context: Context) {
    relationalStore.getRdbStore(context, STORE_CONFIG, (err, store) => {
      if (err) {
        console.error(`Failed to get RdbStore: ${err.message}`)
        return
      }
      if (store.version == 0) {
        store.executeSql(SQL_CREATE_TABLE).then(() => {
          store.version = PasteboardDatabase.VERSION
        }).catch((err: BusinessError) => {
          console.error(`Failed to create table for ${STORE_NAME}: ${err.message}`)
        })
      }
      rdbStore = store
    })
  }
}
