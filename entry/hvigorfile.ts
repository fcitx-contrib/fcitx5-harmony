import { hapTasks, OhosHapContext, OhosPluginId } from '@ohos/hvigor-ohos-plugin'
import { getNode, hvigor, HvigorNode } from '@ohos/hvigor'
import { rmSync } from 'fs'

const param = hvigor.getParameter()
const buildAbiOverride = process.env['BUILD_ABI'] ?? param.getExtParam('buildABI')
const supportedAbis = ['arm64-v8a', 'x86_64']

const rootNode = getNode(__filename)
rootNode.afterNodeEvaluate(node => {
    const hapContext = node.getContext(OhosPluginId.OHOS_HAP_PLUGIN) as OhosHapContext
    const buildProfileOpt = hapContext.getBuildProfileOpt()
    buildProfileOpt['buildOption']['externalNativeOptions']['abiFilters'] =
        buildAbiOverride?.split(',') ?? supportedAbis
    hapContext.setBuildProfileOpt(buildProfileOpt)

    registerCleanCxxTask(node)
    registerLibsResourceBridgeTask(node)
})

function registerCleanCxxTask(node: HvigorNode) {
    node.registerTask({
        name: 'cleanCxxIntermediates',
        run() {
            try {
                rmSync(node.nodeDir.file('.cxx').filePath, { recursive: true, force: true })
            } catch {}
        },
        postDependencies: ['clean']
    })
}

function registerLibsResourceBridgeTask(node: HvigorNode) {
    node.registerTask({
        name: 'libsBeforeResourceBridge',
        run() {
            // Do nothing else, just make sure the processing of native libs
            // earlier than the processing of resources.
        },
        dependencies: ['default@ProcessLibs'],
        postDependencies: ['default@ProcessResource'],
    })
}

export default {
    system: hapTasks,  /* Built-in plugin of Hvigor. It cannot be modified. */
    plugins:[]         /* Custom plugin to extend the functionality of Hvigor. */
}
