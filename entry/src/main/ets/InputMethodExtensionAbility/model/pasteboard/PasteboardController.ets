import { Context } from '@kit.AbilityKit'
import { BusinessError, pasteboard } from '@kit.BasicServicesKit'

const systemPasteboard: pasteboard.SystemPasteboard = pasteboard.getSystemPasteboard()

export class PasteboardController {
  constructor() {
  }
  
  public onCreate(context: Context): void {
    console.debug('onCreate')
    systemPasteboard.on('update', this.onSystemPasteboardUpdate)
  }
  
  public onDestroy(): void {
    console.debug('onDestroy')
    systemPasteboard.off('update', this.onSystemPasteboardUpdate)
  }
  
  public onSystemPasteboardUpdate(): void {
    systemPasteboard.getData((err: BusinessError, data: pasteboard.PasteData) => {
      if (err) {
        console.error(`Failed to get paste data: ${err.message}`)
        return
      }
      const text = data.getPrimaryText()
      console.debug(`get primary text: ${text}`)
    })
  }
}

const pasteboardController = new PasteboardController()

export default pasteboardController
