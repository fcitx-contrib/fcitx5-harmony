export namespace StrUtil {
  export function isBlank(str: string): boolean {
    return Array.from(str).every((ch) => ch.match(/\s/))
  }
}
