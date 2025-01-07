import { inputMethodEngine } from '@kit.IMEKit'
import type { InputMethodExtensionContext} from '@kit.IMEKit'
import { display } from '@kit.ArkUI';
import fcitx from 'libentry.so';

const ability: inputMethodEngine.InputMethodAbility = inputMethodEngine.getInputMethodAbility();
const keyboardDelegate = inputMethodEngine.getKeyboardDelegate() // Physical keyboard

export class KeyboardController {
  private ctx: InputMethodExtensionContext | undefined = undefined;
  private panel: inputMethodEngine.Panel | undefined = undefined;
  private textInputClient: inputMethodEngine.InputClient | undefined = undefined;
  private keyboardController: inputMethodEngine.KeyboardController | undefined = undefined;

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

  public handleKey(key: string, keyCode?: number): void {
    fcitx.processKey(key ? key.charCodeAt(0) : 0, keyCode ?? 0, false)
  }

  public insertText(text: string): void {
    if (this.textInputClient) {
      this.textInputClient.insertText(text);
    }
  }

  public deleteForward(length: number): void {
    if (this.textInputClient) {
      this.textInputClient.deleteForward(length);
    }
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
      if (this.panel) {
        await this.panel.resize(dWidth, keyHeight);
        await this.panel.moveTo(0, nonBarPosition);
        await this.panel.setUiContent('InputMethodExtensionAbility/pages/Index');
      }
    });
  }

  private physicalKeyEventHandler(e: inputMethodEngine.KeyEvent): boolean {
    const isRelease = e.keyAction === 3
    fcitx.processKey(0, e.keyCode, isRelease)
    return false
  }

  private registerListener(): void {
    this.registerInputListener();
    keyboardDelegate.on('keyDown', this.physicalKeyEventHandler)
    keyboardDelegate.on('keyUp', this.physicalKeyEventHandler)
  }

  private registerInputListener(): void {
    ability.on('inputStart', (kbController, textInputClient) => {
      console.debug('inputStart')
      this.textInputClient = textInputClient;
      this.keyboardController = kbController;
      fcitx.focusIn()
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
