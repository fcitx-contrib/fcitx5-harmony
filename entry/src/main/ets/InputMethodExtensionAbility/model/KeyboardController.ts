import { webview } from '@kit.ArkWeb'
import { inputMethodEngine } from '@kit.IMEKit'
import type { InputMethodExtensionContext } from '@kit.IMEKit'
import { display } from '@kit.ArkUI';
import { KeyCode } from '@kit.InputKit';
import { SystemEvent, VirtualKeyboardEvent } from '../../../fcitx5-keyboard-web/src/api'
import fcitx from 'libentry.so';
import { FcitxEvent } from './FcitxEvent';
import { convertCode } from './keycode'

const ability: inputMethodEngine.InputMethodAbility = inputMethodEngine.getInputMethodAbility();
const keyboardDelegate = inputMethodEngine.getKeyboardDelegate() // Physical keyboard

export class KeyboardController {
  private ctx: InputMethodExtensionContext | undefined = undefined;
  private panel: inputMethodEngine.Panel | undefined = undefined;
  private attribute: inputMethodEngine.EditorAttribute | null = null
  private port: webview.WebMessagePort | null = null
  private textInputClient: inputMethodEngine.InputClient | undefined = undefined;
  private keyboardController: inputMethodEngine.KeyboardController | undefined = undefined;
  private preedit = ''
  private preeditIndex = 0

  constructor() {
  }

  public onCreate(context: InputMethodExtensionContext): void {
    console.debug("onCreate")
    fcitx.setCallback((data: string) => {
      this.processAsyncData(data)
    })
    this.ctx = context;
    this.initWindow();
    this.registerListener();
  }

  public onDestroy(): void {
    console.debug("onDestroy")
    this.unRegisterListener();
    if (this.panel) {
      ability.destroyPanel(this.panel);
    }
    if (this.ctx) {
      this.ctx.destroy();
    }
  }

  public setPort(port: webview.WebMessagePort) {
    this.port = port
  }

  private sendEvent(event: SystemEvent) {
    this.port?.postMessageEvent(JSON.stringify(event))
  }

  private processAsyncData(data: string) {
    console.debug(`processing data: ${data}`)
    const event = JSON.parse(data) as SystemEvent | FcitxEvent
    switch (event.type) {
      case 'COMMIT':
        if (event.data) {
          this.commitString(event.data)
        }
        break
      case 'PREEDIT':
        this.updatePreviewText(event.data.preedit)
        break
      default:
        this.sendEvent(event)
        break
    }
  }

  public setEnterKeyType() {
    const type = this.attribute?.enterKeyType ?? 0
    const label = {
      [inputMethodEngine.ENTER_KEY_TYPE_UNSPECIFIED]: '',
      [inputMethodEngine.ENTER_KEY_TYPE_GO]: 'go',
      [inputMethodEngine.ENTER_KEY_TYPE_SEARCH]: 'search',
      [inputMethodEngine.ENTER_KEY_TYPE_SEND]: 'send',
      [inputMethodEngine.ENTER_KEY_TYPE_NEXT]: 'next',
      [inputMethodEngine.ENTER_KEY_TYPE_DONE]: 'done',
      [inputMethodEngine.ENTER_KEY_TYPE_PREVIOUS]: 'previous',
      [inputMethodEngine.ENTER_KEY_TYPE_NEWLINE]: 'newline',
    }[type]
    this.sendEvent({ type: 'ENTER_KEY_TYPE',  data: label })
  }

  private getCursor() {
    if (!this.textInputClient) {
      return -1
    }
    let step = 1024
    while (true) {
      const text = this.textInputClient.getForwardSync(step)
      if (text.length < step) {
        return text.length
      }
      step *= 2
    }
  }

  private commitString(text: string): void {
    if (this.preedit) {
      this.textInputClient?.setPreviewTextSync('',
        { start: this.preeditIndex, end: this.preeditIndex + this.preedit.length })
      this.textInputClient?.finishTextPreviewSync()
    }
    this.insertText(text)
    this.preedit = ''
    this.preeditIndex = this.getCursor()
  }

  private updatePreviewText(text: string) {
    const start = this.preedit ? this.preeditIndex : this.getCursor()
    const end = start + this.preedit.length
    if (this.preedit || text) {
      this.textInputClient?.setPreviewTextSync(text, { start, end })
      if (!text) {
        this.textInputClient?.finishTextPreviewSync()
      }
    }
    this.preedit = text
    this.preeditIndex = start
  }

  private processResult(res: ReturnType<typeof fcitx.processKey>): boolean {
    if (!res.accepted) {
      return false
    }
    if (res.commit) {
      this.commitString(res.commit)
    }
    this.updatePreviewText(res.preedit)
    return true
  }

  public handleKey(key: string, keyCode?: number): void {
    const res = fcitx.processKey(key ? key.charCodeAt(0) : 0, keyCode ?? 0, false)
    if (!this.processResult(res)) {
      switch (keyCode) { // Check code first as enter has to be handled differently, which has key \r.
      case KeyCode.KEYCODE_DEL:
        this.deleteForward(1)
        break
      case KeyCode.KEYCODE_DPAD_DOWN:
        this.textInputClient?.moveCursorSync(inputMethodEngine.Direction.CURSOR_DOWN)
        break
      case KeyCode.KEYCODE_DPAD_LEFT:
        this.textInputClient?.moveCursorSync(inputMethodEngine.Direction.CURSOR_LEFT)
        break
      case KeyCode.KEYCODE_DPAD_RIGHT:
        this.textInputClient?.moveCursorSync(inputMethodEngine.Direction.CURSOR_RIGHT)
        break
      case KeyCode.KEYCODE_DPAD_UP:
        this.textInputClient?.moveCursorSync(inputMethodEngine.Direction.CURSOR_UP)
        break
      case KeyCode.KEYCODE_ENTER:
        this.textInputClient?.sendKeyFunction(this.attribute?.enterKeyType)
        break
      default:
        if (key) {
          this.insertText(key)
        }
      }
    }
  }

  public handleVirtualKeyboardEvent(event: VirtualKeyboardEvent) {
    switch (event.type) {
      case 'COPY':
        this.textInputClient?.sendExtendAction(inputMethodEngine.ExtendAction.COPY)
        break
      case 'CUT':
        this.textInputClient?.sendExtendAction(inputMethodEngine.ExtendAction.CUT)
        break
      case 'KEY_DOWN':
        this.handleKey(event.data.key, convertCode(event.data.code))
        break
      case 'PASTE':
        this.textInputClient?.sendExtendAction(inputMethodEngine.ExtendAction.PASTE)
        break
      case 'SELECT_CANDIDATE':
        fcitx.selectCandidate(event.data)
        break
    }
  }

  public insertText(text: string): void {
    this.textInputClient?.insertTextSync(text);
  }

  public deleteForward(length: number): void {
    this.textInputClient?.deleteForwardSync(length);
  }

  private initWindow(): void {
    if (this.ctx === undefined) {
      return;
    }
    // 1260x2720 on emulator
    const { width, height } = display.getDefaultDisplaySync()
    let containerHeight = Math.min(width / 3 * 2, height / 3) // Same with xiaoyi on emulator
    let panelInfo: inputMethodEngine.PanelInfo = {
      type: inputMethodEngine.PanelType.SOFT_KEYBOARD,
      flag: inputMethodEngine.PanelFlag.FLG_FIXED
    };
    ability.createPanel(this.ctx, panelInfo).then((inputPanel: inputMethodEngine.Panel) => {
      this.panel = inputPanel;
      this.panel.on('show', () => {
        console.debug('show')
      })
      this.panel.on('hide', () => {
        console.debug('hide')
        this.sendEvent({ type: 'HIDE' })
      })
      this.panel.resize(width, containerHeight)
      this.panel.setUiContent('InputMethodExtensionAbility/pages/VirtualKeyboard')
    })
  }

  private physicalKeyEventHandler(e: inputMethodEngine.KeyEvent): boolean {
    const isRelease = e.keyAction === 3
    const res = fcitx.processKey(0, e.keyCode, isRelease)
    return this.processResult(res)
  }

  private registerListener(): void {
    this.registerInputListener();
    keyboardDelegate.on('keyDown', this.physicalKeyEventHandler.bind(this))
    keyboardDelegate.on('keyUp', this.physicalKeyEventHandler.bind(this))
    keyboardDelegate.on('textChange', (text: string) => {
      // Mainly for clicking x to clear all text including preedit,
      // but harmless for pressing backspace till nothing left.
      if (!text) {
        fcitx.reset()
      }
    })
  }

  private registerInputListener(): void {
    ability.on('inputStart', (kbController, textInputClient) => {
      console.debug('inputStart')
      this.textInputClient = textInputClient;
      this.keyboardController = kbController;
      this.attribute = textInputClient.getEditorAttributeSync()
      fcitx.focusIn(this.attribute.isTextPreviewSupported)
      this.setEnterKeyType()
    })
    ability.on('inputStop', () => {
      // This is not paired with inputStart. Only called when switching to Celia.
      console.debug('inputStop')
      fcitx.focusOut()
    });
  }

  private unRegisterListener(): void {
    ability.off('inputStart');
    ability.off('inputStop', () => {
    });
    keyboardDelegate.off('keyDown')
    keyboardDelegate.off('keyUp')
    keyboardDelegate.off('textChange')
    this?.panel.off('show')
    this?.panel.off('hide')
  }
}

const keyboardController = new KeyboardController();

export default keyboardController;
