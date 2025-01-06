import { InputMethodExtensionAbility } from "@kit.IMEKit";
import type Want from "@ohos.app.ability.Want";
import keyboardController from "./model/KeyboardController";
import fcitx from 'libentry.so';

export default class FcitxInputMethodService extends InputMethodExtensionAbility {
  onCreate(want: Want): void {
    keyboardController.onCreate(this.context)
    const bundle = this.context.bundleCodeDir
    const resfile = this.context.resourceDir
    fcitx.init(bundle, resfile)
  }

  onDestroy(): void {
    keyboardController.onDestroy()
  }
}
