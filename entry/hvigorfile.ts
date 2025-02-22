import { hapTasks, OhosHapContext, OhosPluginId } from '@ohos/hvigor-ohos-plugin';
import { getNode, hvigor } from '@ohos/hvigor';

const param = hvigor.getParameter();
const buildAbiOverride = process.env['BUILD_ABI'] ?? param.getExtParam('buildABI');
const supportedAbis = ['arm64-v8a', 'x86_64']

const rootNode = getNode(__filename);
rootNode.afterNodeEvaluate(node => {
    const appContext = node.getContext(OhosPluginId.OHOS_HAP_PLUGIN) as OhosHapContext;
    const buildProfileOpt = appContext.getBuildProfileOpt();
    buildProfileOpt['buildOption']['externalNativeOptions']['abiFilters'] =
        buildAbiOverride?.split(',') ?? supportedAbis;
    appContext.setBuildProfileOpt(buildProfileOpt)
})

export default {
    system: hapTasks,  /* Built-in plugin of Hvigor. It cannot be modified. */
    plugins:[]         /* Custom plugin to extend the functionality of Hvigor. */
}
