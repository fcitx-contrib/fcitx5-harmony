import sql from '@liushengyi/smartdb';
import relationalStore from '@ohos.data.relationalStore';

export class PasteboardDatabase {
  static VERSION = 1

  static init(context: Context): void {
    sql.dbHelper.initDb(context, 'clbdb', PasteboardDatabase.VERSION, new PasteboardDbOpenHelper())
  }
}

class PasteboardDbOpenHelper extends sql.DbOpenHelper {
  constructor() {
    super()
  }

  override onCreate(db: relationalStore.RdbStore): void {
    db.executeSync("CREATE TABLE IF NOT EXISTS clipboard (" +
      "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, " +
      "text TEXT NOT NULL, " +
      "pinned INTEGER NOT NULL, timestamp INTEGER NOT NULL DEFAULT -1, " +
      "type TEXT NOT NULL DEFAULT 'text/plain', " +
      "deleted INTEGER NOT NULL DEFAULT 0, " +
      "sensitive INTEGER NOT NULL DEFAULT 0)")
  }

  override onUpgrade(db: relationalStore.RdbStore, oldVersion: number, newVersion: number): void {
  }
}
