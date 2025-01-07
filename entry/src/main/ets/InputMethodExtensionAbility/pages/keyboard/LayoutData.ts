import type { TextKey } from "./KeyType"
import { KeyCode } from '@kit.InputKit';

export let textLayoutData: TextKey[][] = [
  [
    { label: '1' },
    { label: '2' },
    { label: '3' },
    { label: '4' },
    { label: '5' },
    { label: '6' },
    { label: '7' },
    { label: '8' },
    { label: '9' },
    { label: '0' }
  ],
  [
    { label: 'Q' },
    { label: 'W' },
    { label: 'E' },
    { label: 'R' },
    { label: 'T' },
    { label: 'Y' },
    { label: 'U' },
    { label: 'I' },
    { label: 'O' },
    { label: 'P' }
  ],
  [
    { label: 'A' },
    { label: 'S' },
    { label: 'D' },
    { label: 'F' },
    { label: 'G' },
    { label: 'H' },
    { label: 'J' },
    { label: 'K' },
    { label: 'L' },
  ],
  [
    { label: '`' },
    { label: 'Z' },
    { label: 'X' },
    { label: 'C' },
    { label: 'V' },
    { label: 'B' },
    { label: 'N' },
    { label: 'M' },
    { label: '⌫', key: '', code: KeyCode.KEYCODE_DEL },
  ],
  [
    { label: ',' },
    { label: '␣', key: ' ' },
    { label: '.' },
  ]
]
