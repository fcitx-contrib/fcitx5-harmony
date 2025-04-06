import { webview } from '@kit.ArkWeb';
import type { InputMethodExtensionContext } from '@kit.IMEKit';
import { inputMethodEngine } from '@kit.IMEKit';
import { display } from '@kit.ArkUI';
import { KeyCode } from '@kit.InputKit';
import { SystemEvent, VirtualKeyboardEvent } from '../../../fcitx5-keyboard-web/src/api';
import fcitx, { InputContextState } from 'libentry.so';
import { FcitxEvent } from './FcitxEvent';
import { convertCode } from './keycode';
import { onTextChange, redo, resetStacks, undo } from './TextOperation';

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
  private textWithPreedit = ''
  private selectionStart = 0
  private selectionEnd = 0
  private selecting = false
  private pendingEvents: SystemEvent[] = []

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
    for (const event of this.pendingEvents) {
      this.sendEvent(event)
    }
    this.pendingEvents = []
  }

  private sendEvent(event: SystemEvent) {
    if (this.port) {
      this.port?.postMessageEvent(JSON.stringify(event))
    } else {
      this.pendingEvents.push(event)
    }
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

  private enterKeyLabelMapping: Map<number, string> = new Map([
    [inputMethodEngine.ENTER_KEY_TYPE_UNSPECIFIED, ''],
    [inputMethodEngine.ENTER_KEY_TYPE_GO, 'go'],
    [inputMethodEngine.ENTER_KEY_TYPE_SEARCH, 'search'],
    [inputMethodEngine.ENTER_KEY_TYPE_SEND, 'send'],
    [inputMethodEngine.ENTER_KEY_TYPE_NEXT, 'next'],
    [inputMethodEngine.ENTER_KEY_TYPE_DONE, 'done'],
    [inputMethodEngine.ENTER_KEY_TYPE_PREVIOUS, 'previous'],
    [inputMethodEngine.ENTER_KEY_TYPE_NEWLINE, 'newline'],
  ])

  public setEnterKeyType() {
    const type = this.attribute?.enterKeyType ?? 0
    const label: string = this.enterKeyLabelMapping[type]
    this.sendEvent({ type: 'ENTER_KEY_TYPE', data: label })
  }

  public setUndo(enabled: boolean) {
    this.sendEvent({ type: 'UNDO', data: enabled })
  }

  public setRedo(enabled: boolean) {
    this.sendEvent({ type: 'REDO', data: enabled })
  }

  private getTextBefore() {
    if (!this.textInputClient) {
      return ''
    }
    return this.textInputClient.getForwardSync(this.textInputClient.getTextIndexAtCursorSync())
  }

  private getTextAfter() {
    if (!this.textInputClient) {
      return ''
    }
    let step = 1024
    while (true) {
      const text = this.textInputClient.getBackwardSync(step)
      if (text.length < step) {
        return text
      }
      step *= 2
    }
  }

  private commitString(text: string): void {
    if (this.preedit === text) {
      // System won't emit textChange, and the textChange for preedit was blocked.
      // So we call onTextChange manually.
      onTextChange(this.textWithPreedit)
    }
    this.updatePreviewText('')
    this.insertText(text)
    this.preedit = ''
    this.preeditIndex = this.textInputClient?.getTextIndexAtCursorSync() ?? -1
  }

  private updatePreviewText(text: string) {
    let start = this.preeditIndex
    let end = start + this.preedit.length
    if (this.preedit || text) {
      try {
        this.textInputClient?.setPreviewTextSync(text, { start, end })
      } catch (e) {
        // There is selection.
        this.deleteForward(1)
        start = this.textInputClient!.getTextIndexAtCursorSync()
        end = start
        this.textInputClient?.setPreviewTextSync(text, { start, end })
      }
      if (!text) {
        this.textInputClient?.finishTextPreviewSync()
      }
    }
    this.preedit = text
  }

  private processResult(res: InputContextState): boolean {
    if (!res.accepted) {
      return false
    }
    if (res.commit) {
      this.commitString(res.commit)
    }
    this.updatePreviewText(res.preedit)
    return true
  }

  private move(direction: inputMethodEngine.Direction) {
    if (this.selecting) {
      this.textInputClient?.selectByMovementSync({ direction })
    } else {
      this.textInputClient?.moveCursorSync(direction)
    }
  }

  private moveWithinLine(destination: 'home' | 'end') {
    const before = this.getTextBefore()
    if (destination === 'home') {
      const index = before.lastIndexOf('\n') + 1 // works for first line as well
      if (this.selecting) {
        this.textInputClient?.selectByRangeSync({ start: index, end: this.textInputClient.getTextIndexAtCursorSync() })
      } else {
        for (let i = before.length; i > index; --i) {
          this.textInputClient?.moveCursorSync(inputMethodEngine.Direction.CURSOR_LEFT)
        }
      }
    } else {
      const after = this.getTextAfter()
      let offset = after.indexOf('\n')
      if (offset === -1) { // last line
        offset = after.length
      }
      if (this.selecting) {
        this.textInputClient?.selectByRangeSync({
          start: this.selectionStart,
          end: this.textInputClient.getTextIndexAtCursorSync() + offset
        })
      } else {
        for (let i = 0; i < offset; ++i) {
          this.textInputClient?.moveCursorSync(inputMethodEngine.Direction.CURSOR_RIGHT)
        }
      }
    }
  }

  private setCursor(index: number) {
    if (index) {
      // 2 steps so that 📍 won't be shown.
      this.textInputClient?.selectByRangeSync({ start: index - 1, end: index - 1 })
      this.textInputClient?.moveCursorSync(inputMethodEngine.Direction.CURSOR_RIGHT)
    } else {
      this.textInputClient?.selectByRangeSync({ start: 0, end: 0 })
      this.textInputClient?.moveCursorSync(inputMethodEngine.Direction.CURSOR_LEFT)
    }
  }

  public handleKey(key: string, keyCode?: number): void {
    const res = fcitx.processKey(key ? key.charCodeAt(0) : 0, keyCode ?? 0, false)
    if (!this.processResult(res)) {
      switch (keyCode) { // Check code first as enter has to be handled differently, which has key \r.
        case KeyCode.KEYCODE_DEL:
          return this.deleteForward(1)
        case KeyCode.KEYCODE_DPAD_DOWN:
          return this.move(inputMethodEngine.Direction.CURSOR_DOWN)
        case KeyCode.KEYCODE_DPAD_LEFT:
          return this.move(inputMethodEngine.Direction.CURSOR_LEFT)
        case KeyCode.KEYCODE_DPAD_RIGHT:
          return this.move(inputMethodEngine.Direction.CURSOR_RIGHT)
        case KeyCode.KEYCODE_DPAD_UP:
          return this.move(inputMethodEngine.Direction.CURSOR_UP)
        case KeyCode.KEYCODE_MOVE_END:
          return this.moveWithinLine('end')
        case KeyCode.KEYCODE_ENTER:
          this.textInputClient?.sendKeyFunction(this.attribute?.enterKeyType)
          break
        case KeyCode.KEYCODE_MOVE_HOME:
          return this.moveWithinLine('home')
        default:
          if (key) {
            this.insertText(key)
          }
      }
    }
  }

  public handleVirtualKeyboardEvent(event: VirtualKeyboardEvent): void | Promise<void> {
    switch (event.type) {
      case 'ASK_CANDIDATE_ACTIONS':
        return fcitx.askCandidateActions(event.data)
      case 'CANDIDATE_ACTION':
        return fcitx.activateCandidateAction(event.data.index, event.data.id);
      case 'COLLAPSE':
        return this.panel?.hide()
      case 'COMMIT':
        this.processResult(fcitx.reset())
        return this.insertText(event.data)
      case 'COPY':
        return this.textInputClient?.sendExtendAction(inputMethodEngine.ExtendAction.COPY)
      case 'CUT':
        return this.textInputClient?.sendExtendAction(inputMethodEngine.ExtendAction.CUT)
      case 'DESELECT':
        this.selecting = false
        this.setCursor(this.textInputClient?.getTextIndexAtCursorSync() ?? 0)
        break
      case 'KEY_DOWN':
        return this.handleKey(event.data.key, convertCode(event.data.code))
      case 'PASTE':
        return this.textInputClient?.sendExtendAction(inputMethodEngine.ExtendAction.PASTE)
      case 'REDO':
        return redo(this.textInputClient!)
      case 'SELECT':
        this.selecting = true
        break
      case 'SELECT_ALL':
        this.selecting = true
        this.textInputClient?.sendExtendAction(inputMethodEngine.ExtendAction.SELECT_ALL)
        break
      case 'SELECT_CANDIDATE':
        return fcitx.selectCandidate(event.data)
      case 'STATUS_AREA_ACTION':
        return fcitx.activateStatusAreaAction(event.data)
      case 'UNDO':
        return undo(this.textInputClient!)
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
    const defaultDisplay = display.getDefaultDisplaySync()
    const width = defaultDisplay.width
    const height = defaultDisplay.height
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
    keyboardDelegate.on('keyDown', (e) => { return this.physicalKeyEventHandler(e) })
    keyboardDelegate.on('keyUp', (e) => { return this.physicalKeyEventHandler(e) })
    // This is not called on focus.
    keyboardDelegate.on('textChange', (text: string) => {
      this.textWithPreedit = text
      // Block text changes with a preedit.
      if (!this.preedit) {
        onTextChange(text)
      }
      // Mainly for clicking x to clear all text including preedit,
      // but harmless for pressing backspace till nothing left.
      if (!text) {
        fcitx.reset()
      }
    })
    // This is called on focus. There is a bug on Harmony's side that makes it unreliable.
    // Reproduce: having 是的, double click 是 to select, drag 📍 on the left to the right,
    // then double click 的 to select. The last step won't update selection to 1,2.
    keyboardDelegate.on('selectionChange', (oldBegin: number, oldEnd: number, newBegin: number, newEnd: number) => {
      console.debug(`selectionChange ${oldBegin} ${oldEnd} ${newBegin} ${newEnd}`)
      this.selectionStart = newBegin
      this.selectionEnd = newEnd
      // Because selectByMovementSync won't call this, doing below won't interrupt selection when start and end overlap.
      this.selecting = this.selectionStart !== this.selectionEnd
      if (!this.preedit) {
        this.preeditIndex = this.selectionStart
      }
      this.sendEvent({ type: this.selecting? 'SELECT' : 'DESELECT' })
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
      // If we do it synchronously, getTextIndexAtCursorSync highly likely crashes.
      setTimeout((): void => resetStacks(this.getTextBefore() + this.getTextAfter()), 0)
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
    this.panel?.off('show')
    this.panel?.off('hide')
  }
}

const keyboardController = new KeyboardController();

export default keyboardController;
