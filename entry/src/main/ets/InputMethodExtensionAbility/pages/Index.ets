import { textLayoutData } from './keyboard/LayoutData';
import keyboardController from '../model/KeyboardController';
import { TextKey } from './keyboard/KeyType';

@Component
struct KeyItem {
  @State keyValue: TextKey = textLayoutData[0][0];
  @State keyBgc: string = "#fff"
  @State keyFontColor: string = "#000"

  build() {
    Column() {
      Flex({
        direction: FlexDirection.Column,
        alignItems: ItemAlign.Center, justifyContent: FlexAlign.Center
      }) {
        Text(this.keyValue.character).fontSize(20).fontColor(this.keyFontColor)
      }
    }
    .backgroundColor(this.keyBgc)
    .borderRadius(6)
    .width("8%")
    .height("65%")
    .onClick(() => {
      keyboardController.insertText(this.keyValue.character.toLowerCase());
    })
  }
}

@Component
export struct BackspaceItem {
  @State keyValue: TextKey = { character: '⌫' }
  @State keyBgc: string = "#9f9f9f"
  @State keyFontColor: string = "#000"

  build() {
    Column() {
      Flex({
        direction: FlexDirection.Column,
        alignItems: ItemAlign.Center, justifyContent: FlexAlign.Center
      }) {
        Text(this.keyValue.character).fontSize(20).fontColor(this.keyFontColor)
      }
    }
    .backgroundColor(this.keyBgc)
    .width("13%")
    .height("65%")
    .borderRadius(6)
    .onClick(() => {
      keyboardController.deleteForward(1);
    })
  }
}

@Component
export struct SpaceItem {
  @State keyValue: TextKey = { character: '␣' }
  @State keyBgc: string = "#fff"
  @State keyFontColor: string = "#000"

  build() {
    Column() {
      Flex({
        direction: FlexDirection.Column,
        alignItems: ItemAlign.Center, justifyContent: FlexAlign.Center
      }) {
        Text(this.keyValue.character).fontSize(20).fontColor(this.keyFontColor)
      }
    }
    .backgroundColor(this.keyBgc)
    .width("40%")
    .height("65%")
    .borderRadius(6)
    .onClick(() => {
      keyboardController.insertText(' ');
    })
  }
}

@Component
struct TextKeyboard {
  private textList: TextKey[][] = textLayoutData;

  build() {
    Flex({ direction: FlexDirection.Column, alignItems: ItemAlign.Center, justifyContent: FlexAlign.SpaceAround }) {
      ForEach(this.textList, (row: TextKey[]) => {
        Flex({ justifyContent: FlexAlign.SpaceAround }) {
          ForEach(row, (column: TextKey) => {
            if (column.character == "⌫") {
              BackspaceItem();
            } else if (column.character == "␣") {
              SpaceItem();
            } else {
              KeyItem({ keyValue: column });
            }
          }, (column: TextKey) => column.character);
        }
        .width('96%')
        .height('20%')
      }, (row: TextKey[]) => textLayoutData.indexOf(row).toString());
    }
  }
}

@Entry
@Component
struct Index {
  build() {
    Stack() {
      Flex({
        direction: FlexDirection.Column,
        alignItems: ItemAlign.Center,
        justifyContent: FlexAlign.End
      }) {
        Flex({
          direction: FlexDirection.Column,
          alignItems: ItemAlign.Center,
          justifyContent: FlexAlign.SpaceBetween
        }) {
          TextKeyboard();
        }
        .align(Alignment.End)
        .width("100%")
        .height("96%")
      }
      .height("100%").align(Alignment.End).backgroundColor("#cdd0d7")
    }
    .position({ x: 0, y: 0 }).zIndex(99999)
  }
}
