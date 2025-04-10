import { spawnSync } from 'child_process'
import { mkdirSync } from 'fs'
import path from 'path'

const [destDir, buildDir, prebuiltUsrDir] = process.argv.slice(2)

const env = { ...process.env, DESTDIR: path.resolve(destDir) }

const destShareDir = `${destDir}/usr/share`
const isoJsonDir = `${destShareDir}/iso-codes/json`
mkdirSync(isoJsonDir, { recursive: true })

const commands = [
  ['cmake', '--install', buildDir, '--component', 'config'],
  ['cp', '-r', `${prebuiltUsrDir}/share/X11`, destShareDir],
  ['cp', `${prebuiltUsrDir}/share/iso-codes/json/iso_3166-1.json`, isoJsonDir],
  ['cp', `${prebuiltUsrDir}/share/iso-codes/json/iso_639-3.json`, isoJsonDir],
]

for (const command of commands) {
  const result = spawnSync(command[0], command.slice(1), {
    env,
    stdio: 'inherit',
    shell: true
  })
  
  if (result.error || result.status !== 0) {
    exit(1)
  }
}
