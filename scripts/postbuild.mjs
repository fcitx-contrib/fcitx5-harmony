import { spawnSync } from 'child_process'
import path from 'path'

const [destDir, buildDir] = process.argv.slice(2)

const env = { ...process.env, DESTDIR: path.resolve(destDir) }
const result = spawnSync('cmake', ['--install', buildDir, '--component', 'config'], {
  env,
  stdio: 'inherit',
  shell: true
})

if (result.error || result.status !== 0) {
  exit(1)
}
