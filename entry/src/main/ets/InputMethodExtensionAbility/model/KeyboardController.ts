import { inputMethodEngine } from '@kit.IMEKit'
import type { InputMethodExtensionContext } from '@kit.IMEKit'
import { display } from '@kit.ArkUI';
import { KeyCode } from '@kit.InputKit';
import fcitx from 'libentry.so';

const ability: inputMethodEngine.InputMethodAbility = inputMethodEngine.getInputMethodAbility();
const keyboardDelegate = inputMethodEngine.getKeyboardDelegate() // Physical keyboard

export class KeyboardController {
  private ctx: InputMethodExtensionContext | undefined = undefined;
  private panel: inputMethodEngine.Panel | undefined = undefined;
  private textInputClient: inputMethodEngine.InputClient | undefined = undefined;
  private keyboardController: inputMethodEngine.KeyboardController | undefined = undefined;
  private preedit = ''
  private preeditIndex = 0

  constructor() {
  }

  public onCreate(context: InputMethodExtensionContext): void {
    console.debug("onCreate")
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

  private processResult(res: ReturnType<typeof fcitx.processKey>): boolean {
    if (!res.accepted) {
      return false
    }
    if (res.commit) {
      if (this.preedit) {
        this.textInputClient?.setPreviewTextSync('',
          { start: this.preeditIndex, end: this.preeditIndex + this.preedit.length })
        this.textInputClient?.finishTextPreviewSync()
      }
      this.insertText(res.commit)
      this.preedit = ''
      this.preeditIndex = this.getCursor()
    }
    const start = this.preedit ? this.preeditIndex : this.getCursor()
    const end = start + this.preedit.length
    if (this.preedit || res.preedit) {
      this.textInputClient?.setPreviewTextSync(res.preedit, { start, end })
      if (!res.preedit) {
        this.textInputClient?.finishTextPreviewSync()
      }
    }
    this.preedit = res.preedit
    this.preeditIndex = start
    return true
  }

  public handleKey(key: string, keyCode?: number): void {
    const res = fcitx.processKey(key ? key.charCodeAt(0) : 0, keyCode ?? 0, false)
    if (!this.processResult(res)) {
      if (key) {
        this.insertText(key)
      } else {
        switch (keyCode) {
        case KeyCode.KEYCODE_DEL:
          this.deleteForward(1)
          break
        }
      }
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
    let dis = display.getDefaultDisplaySync();
    let dWidth = dis.width;
    let dHeight = dis.height;
    let keyHeightRate = 0.3;
    let keyHeight = dHeight * keyHeightRate;
    let nonBarPosition = dHeight - keyHeight;
    let panelInfo: inputMethodEngine.PanelInfo = {
      type: inputMethodEngine.PanelType.SOFT_KEYBOARD,
      flag: inputMethodEngine.PanelFlag.FLG_FIXED
    };
    ability.createPanel(this.ctx, panelInfo).then(async (inputPanel: inputMethodEngine.Panel) => {
      this.panel = inputPanel;
      await this.panel.resize(dWidth, keyHeight);
      await this.panel.setUiContent('InputMethodExtensionAbility/pages/Index');
    });
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
  }

  private registerInputListener(): void {
    ability.on('inputStart', (kbController, textInputClient) => {
      console.debug('inputStart')
      this.textInputClient = textInputClient;
      this.keyboardController = kbController;
      const attribute = textInputClient.getEditorAttributeSync()
      fcitx.focusIn(attribute.isTextPreviewSupported)
    })
    ability.on('inputStop', () => {
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
  }
}

const keyboardController = new KeyboardController();

export default keyboardController;
