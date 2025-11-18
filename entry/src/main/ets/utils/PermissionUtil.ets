import { abilityAccessCtrl, bundleManager, common, PermissionRequestResult, Permissions } from '@kit.AbilityKit'
import { BusinessError } from '@kit.BasicServicesKit'

export namespace PermissionUtil {

  export function checkAccessToken(permission: Permissions): abilityAccessCtrl.GrantStatus {
    const atManager: abilityAccessCtrl.AtManager = abilityAccessCtrl.createAtManager()
    let grantStatus: abilityAccessCtrl.GrantStatus = abilityAccessCtrl.GrantStatus.PERMISSION_DENIED

    let tokenId: number = 0
    try {
      let bundleInfo: bundleManager.BundleInfo =
        bundleManager.getBundleInfoForSelfSync(bundleManager.BundleFlag.GET_BUNDLE_INFO_WITH_APPLICATION)
      let appInfo: bundleManager.ApplicationInfo = bundleInfo.appInfo
      tokenId = appInfo.accessTokenId
    } catch (error) {
      const err: BusinessError = error as BusinessError
      console.error(`Failed to get bundle info for self. Code is ${err.code}, message is ${err.message}`)
    }

    try {
      grantStatus = atManager.checkAccessTokenSync(tokenId, permission)
    } catch (error) {
      const err: BusinessError = error as BusinessError
      console.error(`Failed to check access token. Code is ${err.code}, message is ${err.message}`)
    }

    return grantStatus
  }

  export function requestPermissions(context: common.UIAbilityContext, permissions: Permissions | Array<Permissions>,
    requestResult?: Callback<PermissionRequestResult>): void {
    const atManager: abilityAccessCtrl.AtManager = abilityAccessCtrl.createAtManager()
    const ps = Array.isArray(permissions) ? [...permissions] : [permissions]
    atManager.requestPermissionsFromUser(context, ps).then((result) => {
      if (requestResult) requestResult(result)
    })
  }
}