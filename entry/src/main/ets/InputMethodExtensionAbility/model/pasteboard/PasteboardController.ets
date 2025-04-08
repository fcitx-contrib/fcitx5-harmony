import { Context } from '@kit.AbilityKit'
import { BusinessError, pasteboard } from '@kit.BasicServicesKit'
import sql from '@liushengyi/smartdb'
import { PasteboardDao } from './PasteboardDao'
import { PasteboardDatabase } from './PasteboardDatabase'
import { PasteboardEntry } from './PasteboardEntry'
import { HashSet } from '@kit.ArkTS'
import { StrUtil } from '../../../utils/StrUtil'

const systemPasteboard: pasteboard.SystemPasteboard = pasteboard.getSystemPasteboard()

export type OnPasteboardUpdateListener = (entry: PasteboardEntry) => void

export class PasteboardController {
  private psbDao: PasteboardDao | null = null

  constructor() {
  }

  private onUpdateListeners = new HashSet<OnPasteboardUpdateListener>()

  public addOnUpdateListener(listener: OnPasteboardUpdateListener): void {
    this.onUpdateListeners.add(listener)
  }

  public removeOnUpdateListener(listener: OnPasteboardUpdateListener): void {
    this.onUpdateListeners.remove(listener)
  }

  private itemCount: number = 0

  getItemCount(): number {
    return this.itemCount
  }

  private async updateItemCount() {
    this.itemCount = await this.psbDao!.itemCount()
  }

  public lastEntry: PasteboardEntry | null = null

  private updateLastEntry(entry: PasteboardEntry): void {
    this.lastEntry = entry
    this.onUpdateListeners.forEach((listener) => {
      listener?.(entry)
    })
  }

  public onCreate(context: Context): void {
    console.debug('onCreate')
    PasteboardDatabase.init(context)
    this.psbDao = new PasteboardDao()
    systemPasteboard.on('update', this.onSystemPasteboardUpdate)
    const launch = async () => {
      await this.updateItemCount()
    }
    launch().catch()
  }

  public onDestroy(): void {
    console.debug('onDestroy')
    systemPasteboard.off('update', this.onSystemPasteboardUpdate)
  }

  async get(id: number): Promise<PasteboardEntry> {
    return await this.psbDao!.getById(id)
  }

  async haveUnpinned(): Promise<boolean> {
    return await this.psbDao!.haveUnpinned()
  }

  async allEntries(): Promise<Array<PasteboardEntry>> {
    return await this.psbDao!.allEntries()
  }

  async pin(id: number) {
    await this.psbDao!.updatePinStatus(id, true)
  }

  async unpin(id: number) {
    await this.psbDao!.updatePinStatus(id, false)
  }

  async updateText(id: number, text: string) {
    if (this.lastEntry) {
      if (id === this.lastEntry.id) {
        const copy = this.lastEntry
        copy.text = text
        this.updateLastEntry(copy)
      }
    }
    await this.psbDao!.updateText(id, text)
  }

  async delete(id: number) {
    await this.psbDao!.markAsDeleted(id)
    await this.updateItemCount()
  }

  async deleteAll(skipPinned: boolean = true): Promise<Array<number>> {
    let ids: number[]
    if (skipPinned) {
      ids = await this.psbDao!.findUnpinnedIds()
    } else {
      ids = await this.psbDao!.findAllIds()
    }
    this.psbDao?.markAsDeleted(...ids)
    await this.updateItemCount()
    return ids
  }

  async undoDelete(...ids: number[]) {
    await this.psbDao!.undoDelete(...ids)
    await this.updateItemCount()
  }

  async realDelete() {
    await this.psbDao!.realDelete()
  }

  async nukeTables() {
    await this.psbDao!.clearAllTables()
    await this.updateItemCount()
  }

  @sql.Transactional()
  private async insertEntry(entry: PasteboardEntry): Promise<PasteboardEntry> {
    const rowId = await this.psbDao!.insert(entry)
    if (!rowId) {
      return entry
    }
    const inserted = await this.psbDao!.getByRowId(rowId)
    return inserted ? inserted : entry
  }

  public onSystemPasteboardUpdate(): void {
    systemPasteboard.getData(async (err: BusinessError, data: pasteboard.PasteData) => {
      if (err) {
        console.error(`Failed to get paste data: ${err.message}`)
        return
      }
      const entry = PasteboardEntry.fromPasteData(data)
      if (!entry || StrUtil.isBlank(entry.text)) {
        return
      }
      try {
        const existed = await pasteboardController.psbDao!.find(entry.text, entry.sensitive)
        if (existed) {
          const copy = existed
          copy.timestamp = entry.timestamp
          pasteboardController.updateLastEntry(copy)
          await pasteboardController.psbDao!.updateTime(existed.id, entry.timestamp)
        } else {
          const insertedEntry = await pasteboardController.insertEntry(entry)
          console.debug('get entry text: ' + insertedEntry.text)
          pasteboardController.updateLastEntry(insertedEntry)
          pasteboardController.updateItemCount()
        }
      } catch (e) {
        const error = e as Error
        console.warn(`Failed to update clipboard database: ${error.message}`)
        pasteboardController.updateLastEntry(entry)
      }
    })
  }
}

const pasteboardController = new PasteboardController()

export default pasteboardController
