import { Context } from '@kit.AbilityKit'
import { BusinessError, pasteboard } from '@kit.BasicServicesKit'
import sql from '@liushengyi/smartdb'
import { PasteboardDao } from './PasteboardDao'
import { PasteboardDatabase } from './PasteboardDatabase'
import { PasteboardEntry } from './PasteboardEntry'
import { HashSet } from '@kit.ArkTS'
import { StrUtil } from '../../../utils/StrUtil'

const systemPasteboard: pasteboard.SystemPasteboard = pasteboard.getSystemPasteboard()

export interface OnPasteboardUpdateListener {
  onUpdate(entry: PasteboardEntry): void
}

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

  public lastEntry: PasteboardEntry | null = null

  private updateLastEntry(entry: PasteboardEntry): void {
    this.lastEntry = entry
    this.onUpdateListeners.forEach((listener) => {
      listener?.onUpdate(entry)
    })
  }

  public onCreate(context: Context): void {
    console.debug('onCreate')
    PasteboardDatabase.init(context)
    this.psbDao = new PasteboardDao()
    systemPasteboard.on('update', this.onSystemPasteboardUpdate)
  }

  public onDestroy(): void {
    console.debug('onDestroy')
    systemPasteboard.off('update', this.onSystemPasteboardUpdate)
  }

  @sql.Transactional()
  private async insertEntry(entry: PasteboardEntry): Promise<PasteboardEntry> {
    const rowId = await this.psbDao!.insert(entry)
    if (!rowId) {
      return entry
    }
    const inserted = await this.psbDao!.get(rowId)
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
          await pasteboardController.psbDao!.updateTime(existed.id, entry.timestamp)
        } else {
          const insertedEntry = await pasteboardController.insertEntry(entry)
          console.debug('get entry text: ' + insertedEntry.text)
          pasteboardController.updateLastEntry(insertedEntry)
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
