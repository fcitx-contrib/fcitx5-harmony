import { InputMethodExtensionAbility } from "@kit.IMEKit";
import type Want from "@ohos.app.ability.Want";
import keyboardController from "./model/KeyboardController";

export default class FcitxInputMethodService extends InputMethodExtensionAbility {
  onCreate(want: Want): void {
    keyboardController.onCreate(this.context)
  }

  onDestroy(): void {
    keyboardController.onDestroy()
  }
}
