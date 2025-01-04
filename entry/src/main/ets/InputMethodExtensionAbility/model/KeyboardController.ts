import { inputMethodEngine, InputMethodExtensionContext } from '@kit.IMEKit'
import { display } from '@kit.ArkUI';
import { KeyCode } from '@kit.InputKit';

const ability: inputMethodEngine.InputMethodAbility = inputMethodEngine.getInputMethodAbility();

export class KeyboardController {
  private ctx: InputMethodExtensionContext | undefined = undefined;
  private panel: inputMethodEngine.Panel | undefined = undefined;
  private textInputClient: inputMethodEngine.InputClient | undefined = undefined;
  private keyboardController: inputMethodEngine.KeyboardController | undefined = undefined;

  constructor() {
  }

  public onCreate(context: InputMethodExtensionContext): void {
    this.ctx = context;
    this.initWindow();
    this.registerListener();
  }

  public onDestroy(): void {
    this.unRegisterListener();
    if (this.panel) {
      ability.destroyPanel(this.panel);
    }
    if (this.ctx) {
      this.ctx.destroy();
    }
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

  private registerListener(): void {
    this.registerInputListener();
  }

  private registerInputListener():
    void {
    ability.on('inputStart', (kbController, textInputClient) => {
      this.textInputClient = textInputClient;
      this.keyboardController = kbController;
    })
    ability.on('inputStop', () => {
      this.onDestroy();
    });
  }

  private unRegisterListener():
    void {
    ability.off('inputStart');
    ability.off('inputStop', () => {
    });
  }
}

const keyboardController = new KeyboardController();

export default keyboardController;
