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
    this.initWindow(); // 初始化窗口
    this.registerListener(); // 注册对输入法框架的事件监听
  }

  public onDestroy(): void { // 应用生命周期销毁
    this.unRegisterListener(); // 去注册事件监听
    if (this.panel) { // 销毁窗口
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

  private initWindow(): void { // 初始化窗口
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
    this.registerInputListener(); // 注册对输入法框架服务的监听
    // 注册隐藏键盘事件监听等
  }

  private registerInputListener():
    void {
    // 注册对输入法框架服务的开启及停止事件监听
    ability.on('inputStart', (kbController, textInputClient) => {
      this.textInputClient = textInputClient; // 此为输入法客户端实例，由此调用输入法框架提供给输入法应用的功能接口
      this.keyboardController = kbController;
    })
    ability.on('inputStop', () => {
      this.onDestroy(); // 销毁KeyboardController
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
