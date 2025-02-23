import { hapTasks, OhosHapContext, OhosPluginId } from '@ohos/hvigor-ohos-plugin'
import { getNode, hvigor, HvigorNode } from '@ohos/hvigor'
import { promises as fs } from 'fs';

const param = hvigor.getParameter()
const buildAbiOverride = process.env['BUILD_ABI'] ?? param.getExtParam('buildABI')
const supportedAbis = ['arm64-v8a', 'x86_64']

const rootNode = getNode(__filename)
rootNode.afterNodeEvaluate(node => {
    const appContext = node.getContext(OhosPluginId.OHOS_HAP_PLUGIN) as OhosHapContext;
    const buildProfileOpt = appContext.getBuildProfileOpt()
    buildProfileOpt['buildOption']['externalNativeOptions']['abiFilters'] =
        buildAbiOverride?.split(',') ?? supportedAbis
    appContext.setBuildProfileOpt(buildProfileOpt)

    registerCleanCxxTask(node)
})

function registerCleanCxxTask(node: HvigorNode) {
    node.registerTask({
        name: 'cleanCxxIntermediates',
        run() {
            try {
                fs.rm(node.nodeDir.file('.cxx').filePath, { recursive: true, force: true })
            } catch (_) {
            }
        },
        postDependencies: ['clean']
    })
}

export default {
    system: hapTasks,  /* Built-in plugin of Hvigor. It cannot be modified. */
    plugins:[]         /* Custom plugin to extend the functionality of Hvigor. */
}
