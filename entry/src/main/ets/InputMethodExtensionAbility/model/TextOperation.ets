import { inputMethodEngine } from '@kit.IMEKit'
import keyboardController from './KeyboardController'

interface OperationAddition {
  start: number
  text: string
  time: number
}

interface InsertOperation extends OperationAddition {
  type: 'insert'
}

interface DeleteOperation extends OperationAddition {
  type: 'delete'
}

interface ReplaceOperation extends OperationAddition {
  type: 'replace'
  oldText: string
}

type TextOperation = InsertOperation | DeleteOperation | ReplaceOperation

const undoes: TextOperation[] = []
const redoes: TextOperation[] = []
let currentText: string
let isUndoRedo = false
const STACK_LIMIT = 1024
const MERGE_TIMEOUT = 5000

function clearArray(array: Array<TextOperation>) {
  array.splice(0, array.length)
}

export function resetStacks(text: string) {
  currentText = text
  clearArray(undoes)
  clearArray(redoes)
  keyboardController.setUndo(false)
  keyboardController.setRedo(false)
}

function replace(client: inputMethodEngine.InputClient, start: number, end: number, text: string) {
  client.selectByRangeSync({ start, end })
  if (start !== end) {
    client.deleteBackwardSync(1)
  }
  client.insertTextSync(text)
}

function calculateDiff(a: string, b: string): TextOperation {
  let prefixLength = 0;
  while (prefixLength < a.length && prefixLength < b.length && a[prefixLength] === b[prefixLength]) {
    prefixLength++;
  }

  let suffixLength = 0;
  while (suffixLength < a.length - prefixLength && suffixLength < b.length - prefixLength &&
    a[a.length - 1 - suffixLength] === b[b.length - 1 - suffixLength]) {
    suffixLength++;
  }

  const remainingA = a.slice(prefixLength, a.length - suffixLength);
  const remainingB = b.slice(prefixLength, b.length - suffixLength);
  const time = new Date().getTime()

  if (remainingA && !remainingB) {
    return {
      type: 'delete',
      start: prefixLength,
      text: remainingA,
      time
    }
  } else if (!remainingA && remainingB) {
    return {
      type: 'insert',
      start: prefixLength,
      text: remainingB,
      time
    }
  } else {
    return {
      type: 'replace',
      start: prefixLength,
      oldText: remainingA,
      text: remainingB,
      time
    }
  }
}

export function onTextChange(text: string) {
  if (currentText === text) {
    return
  }
  if (!isUndoRedo) {
    clearArray(redoes)
    const diff = calculateDiff(currentText, text)
    let merged = false
    if (undoes.length && !redoes.length) {
      const lastUndo = undoes[undoes.length - 1]
      if (diff.time - lastUndo.time < MERGE_TIMEOUT) {
        if (lastUndo.type === 'insert' && diff.type === 'insert') {
          if (lastUndo.start + lastUndo.text.length === diff.start) {
            // Continuous insert
            lastUndo.text += diff.text
            merged = true
          }
        } else if (lastUndo.type === 'delete' && diff.type === 'delete') {
          if (diff.start + diff.text.length === lastUndo.start) {
            // Continuous Backspace
            lastUndo.start = diff.start
            lastUndo.text = diff.text + lastUndo.text
            merged = true
          } else if (lastUndo.start === diff.start) {
            // Continuous Delete
            lastUndo.text += diff.text
            merged = true
          }
        }
      }
    }
    if (!merged) {
      undoes.push(diff)
      if (undoes.length > STACK_LIMIT) {
        undoes.shift()
      }
    }
    keyboardController.setUndo(true)
    keyboardController.setRedo(false)
  }
  currentText = text
  isUndoRedo = false
}

export function undo(client: inputMethodEngine.InputClient) {
  const lastUndo = undoes.pop()
  if (lastUndo) {
    isUndoRedo = true
    switch (lastUndo.type) {
      case 'insert':
        replace(client, lastUndo.start, lastUndo.start + lastUndo.text.length, '')
        break
      case 'delete':
        replace(client, lastUndo.start, lastUndo.start, lastUndo.text)
        break
      case 'replace':
        replace(client, lastUndo.start, lastUndo.start + lastUndo.text.length, lastUndo.oldText)
        break
    }
    redoes.push(lastUndo)
    if (!undoes.length) {
      keyboardController.setUndo(false)
    }
    keyboardController.setRedo(true)
  }
}

export function redo(client: inputMethodEngine.InputClient) {
  const lastRedo = redoes.pop()
  if (lastRedo) {
    isUndoRedo = true
    switch (lastRedo.type) {
      case 'insert':
        replace(client, lastRedo.start, lastRedo.start, lastRedo.text)
        break
      case 'delete':
        replace(client, lastRedo.start, lastRedo.start + lastRedo.text.length, '')
        break
      case 'replace':
        replace(client, lastRedo.start, lastRedo.start + lastRedo.oldText.length, lastRedo.text)
        break
    }
    undoes.push(lastRedo)
    keyboardController.setUndo(true)
    if (!redoes.length) {
      keyboardController.setRedo(false)
    }
  }
}
