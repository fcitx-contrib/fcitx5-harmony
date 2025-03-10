import { webview } from '@kit.ArkWeb'
import keyboardController from '../model/KeyboardController'
import { VirtualKeyboardEvent } from '../../../fcitx5-keyboard-web/src/api'
import { convertCode } from './keycode'

webview.once("webInited", () => {
  console.log("webInited")
})

@Entry
@Component
struct Index {
  controller: webview.WebviewController = new webview.WebviewController()
  port: webview.WebMessagePort | null = null
  @State webReady: boolean = false

  build() {
    Stack() {
      Flex().backgroundColor("#e3e4e6").width('100%').height('100%')
        .zIndex(99999).visibility(this.webReady ? Visibility.Hidden : Visibility.Visible)
      Web({ src: $rawfile('index.html'), controller: this.controller }).onPageEnd(() => {
        const ports = this.controller.createWebMessagePorts()
        this.port = ports[1]
        this.port.onMessageEvent(msg => {
          const event = JSON.parse(msg as string) as VirtualKeyboardEvent
          switch (event.type) {
            case 'KEY_DOWN':
              keyboardController.handleKey(event.data.key, convertCode(event.data.code))
              break
            case 'SELECT_CANDIDATE':
              keyboardController.selectCandidate(event.data)
              break
          }
        })
        this.controller.postMessage('__init_port__', [ports[0]], '*')
        keyboardController.setPort(this.port)
        // Needed because webview isn't available on first focus in.
        keyboardController.setEnterKeyType()
        this.webReady = true
      })
    }
  }
}
