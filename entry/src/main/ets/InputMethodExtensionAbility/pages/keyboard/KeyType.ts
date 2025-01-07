export interface TextKey {
  label: string; // What's on UI.
  key?: string; // What's send to IM, if not label.
  code?: number; // If not a normal character, e.g. Backspace, this is needed.
}
