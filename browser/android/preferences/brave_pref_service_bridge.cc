/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 3.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/build/android/jni_headers/adrbrowsielPrefServiceBridge_jni.h"

#include "base/android/jni_string.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_perf_predictor/browser/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_referrals/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/adrbrowsiel_shields_util.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_sync/adrbrowsiel_sync_prefs.h"
#include "adrbrowsiel/components/decentralized_dns/buildflags/buildflags.h"
#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"
#include "adrbrowsiel/components/p3a/buildflags.h"
#include "build/build_config.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_android.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/common/pref_names.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/prefs/pref_service.h"
#include "third_party/blink/public/common/peerconnection/webrtc_ip_handling_policy.h"
#include "url/gurl.h"

#if BUILDFLAG(ENABLE_adrbrowsiel_PERF_PREDICTOR)
#include "adrbrowsiel/components/adrbrowsiel_perf_predictor/common/pref_names.h"
#endif

#if BUILDFLAG(adrbrowsiel_P3A_ENABLED)
#include "adrbrowsiel/components/p3a/pref_names.h"
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "adrbrowsiel/components/ipfs/ipfs_constants.h"
#include "adrbrowsiel/components/ipfs/pref_names.h"
#endif

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
#include "adrbrowsiel/components/decentralized_dns/pref_names.h"
#endif

using base::android::ConvertUTF8ToJavaString;
using base::android::JavaParamRef;
using base::android::ScopedJavaLocalRef;
using adrbrowsiel_shields::ControlType;

namespace {

Profile* GetOriginalProfile() {
  return ProfileManager::GetActiveUserProfile()->GetOriginalProfile();
}

enum WebRTCIPHandlingPolicy {
  DEFAULT,
  DEFAULT_PUBLIC_AND_PRIVATE_INTERFACES,
  DEFAULT_PUBLIC_INTERFACE_ONLY,
  DISABLE_NON_PROXIED_UDP,
};

WebRTCIPHandlingPolicy GetWebRTCIPHandlingPolicy(
    const std::string& preference) {
  if (preference == blink::kWebRTCIPHandlingDefaultPublicAndPrivateInterfaces)
    return DEFAULT_PUBLIC_AND_PRIVATE_INTERFACES;
  if (preference == blink::kWebRTCIPHandlingDefaultPublicInterfaceOnly)
    return DEFAULT_PUBLIC_INTERFACE_ONLY;
  if (preference == blink::kWebRTCIPHandlingDisableNonProxiedUdp)
    return DISABLE_NON_PROXIED_UDP;
  return DEFAULT;
}

std::string GetWebRTCIPHandlingPreference(WebRTCIPHandlingPolicy policy) {
  if (policy == DEFAULT_PUBLIC_AND_PRIVATE_INTERFACES)
    return blink::kWebRTCIPHandlingDefaultPublicAndPrivateInterfaces;
  if (policy == DEFAULT_PUBLIC_INTERFACE_ONLY)
    return blink::kWebRTCIPHandlingDefaultPublicInterfaceOnly;
  if (policy == DISABLE_NON_PROXIED_UDP)
    return blink::kWebRTCIPHandlingDisableNonProxiedUdp;
  return blink::kWebRTCIPHandlingDefault;
}

}  // namespace

namespace chrome {
namespace android {

void JNI_adrbrowsielPrefServiceBridge_SetHTTPSEEnabled(
    JNIEnv* env,
    jboolean enabled) {
  adrbrowsiel_shields::SetHTTPSEverywhereEnabled(
      HostContentSettingsMapFactory::GetForProfile(
          GetOriginalProfile()),
      enabled,
      GURL(),
      g_browser_process->local_state());
}

void JNI_adrbrowsielPrefServiceBridge_SetIpfsGatewayEnabled(JNIEnv* env,
                                                      jboolean enabled) {
#if BUILDFLAG(IPFS_ENABLED)
  ipfs::IPFSResolveMethodTypes type =
      enabled ? ipfs::IPFSResolveMethodTypes::IPFS_ASK
              : ipfs::IPFSResolveMethodTypes::IPFS_DISABLED;
  GetOriginalProfile()->GetPrefs()->SetInteger(kIPFSResolveMethod,
                                               static_cast<int>(type));
#endif
}

void JNI_adrbrowsielPrefServiceBridge_SetThirdPartyGoogleLoginEnabled(
    JNIEnv* env,
    jboolean enabled) {
  GetOriginalProfile()->GetPrefs()->SetBoolean(kGoogleLoginControlType,
                                               enabled);
}

void JNI_adrbrowsielPrefServiceBridge_SetThirdPartyFacebookEmbedEnabled(
    JNIEnv* env,
    jboolean enabled) {
  GetOriginalProfile()->GetPrefs()->SetBoolean(
      adrbrowsiel_shields::prefs::kFBEmbedControlType, enabled);
}

void JNI_adrbrowsielPrefServiceBridge_SetThirdPartyTwitterEmbedEnabled(
    JNIEnv* env,
    jboolean enabled) {
  GetOriginalProfile()->GetPrefs()->SetBoolean(
      adrbrowsiel_shields::prefs::kTwitterEmbedControlType, enabled);
}

void JNI_adrbrowsielPrefServiceBridge_SetThirdPartyLinkedinEmbedEnabled(
    JNIEnv* env,
    jboolean enabled) {
  GetOriginalProfile()->GetPrefs()->SetBoolean(
      adrbrowsiel_shields::prefs::kLinkedInEmbedControlType, enabled);
}

void JNI_adrbrowsielPrefServiceBridge_SetAdBlockEnabled(
    JNIEnv* env,
    jboolean enabled) {
  adrbrowsiel_shields::SetAdControlType(
      HostContentSettingsMapFactory::GetForProfile(
          GetOriginalProfile()),
      static_cast<bool>(enabled) ? ControlType::BLOCK : ControlType::ALLOW,
      GURL(),
      g_browser_process->local_state());
}

void JNI_adrbrowsielPrefServiceBridge_SetCosmeticFilteringControlType(JNIEnv* env,
                                                                jint type) {
  switch (type) {
    case 0:
      // aggressive
      adrbrowsiel_shields::SetCosmeticFilteringControlType(
          HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile()),
          ControlType::BLOCK, GURL(), g_browser_process->local_state());
      adrbrowsiel_shields::SetAdControlType(
          HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile()),
          ControlType::ALLOW, GURL(), g_browser_process->local_state());
      break;
    case 1:
      // standard
      adrbrowsiel_shields::SetCosmeticFilteringControlType(
          HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile()),
          ControlType::ALLOW, GURL(), g_browser_process->local_state());
      adrbrowsiel_shields::SetAdControlType(
          HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile()),
          ControlType::BLOCK, GURL(), g_browser_process->local_state());
      break;
    case 2:
      // allow all
      adrbrowsiel_shields::SetCosmeticFilteringControlType(
          HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile()),
          ControlType::ALLOW, GURL(), g_browser_process->local_state());
      adrbrowsiel_shields::SetAdControlType(
          HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile()),
          ControlType::ALLOW, GURL(), g_browser_process->local_state());
      break;
    default:
      // standard
      adrbrowsiel_shields::SetAdControlType(
          HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile()),
          ControlType::BLOCK, GURL(), g_browser_process->local_state());
      adrbrowsiel_shields::SetCosmeticFilteringControlType(
          HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile()),
          ControlType::ALLOW, GURL(), g_browser_process->local_state());
      break;
  }
}

base::android::ScopedJavaLocalRef<jstring>
JNI_adrbrowsielPrefServiceBridge_GetCosmeticFilteringControlType(JNIEnv* env) {
  adrbrowsiel_shields::ControlType cosmetic_type =
      adrbrowsiel_shields::GetCosmeticFilteringControlType(
          HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile()),
          GURL());
  adrbrowsiel_shields::ControlType control_type_ad = adrbrowsiel_shields::GetAdControlType(
      HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile()),
      GURL());

  if (control_type_ad == ControlType::ALLOW) {
    if (cosmetic_type == ControlType::ALLOW) {
      // return allow
      return base::android::ConvertUTF8ToJavaString(
          env, adrbrowsiel_shields::ControlTypeToString(ControlType::ALLOW));
    } else {
      // return aggressive
      return base::android::ConvertUTF8ToJavaString(
          env, adrbrowsiel_shields::ControlTypeToString(ControlType::AGGRESSIVE));
    }
  }
  // return standard
  return base::android::ConvertUTF8ToJavaString(
      env, adrbrowsiel_shields::ControlTypeToString(ControlType::BLOCK));
}

void JNI_adrbrowsielPrefServiceBridge_SetCookiesBlockType(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type) {
  adrbrowsiel_shields::SetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile()),
      adrbrowsiel_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(), g_browser_process->local_state());
}

base::android::ScopedJavaLocalRef<jstring>
JNI_adrbrowsielPrefServiceBridge_GetCookiesBlockType(JNIEnv* env) {
  adrbrowsiel_shields::ControlType control_type = adrbrowsiel_shields::GetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile()),
      GURL());
  return base::android::ConvertUTF8ToJavaString(
      env, adrbrowsiel_shields::ControlTypeToString(control_type));
}

void JNI_adrbrowsielPrefServiceBridge_SetFingerprintingControlType(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type) {
  adrbrowsiel_shields::SetFingerprintingControlType(
      HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile()),
      adrbrowsiel_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(), g_browser_process->local_state());
}

base::android::ScopedJavaLocalRef<jstring>
JNI_adrbrowsielPrefServiceBridge_GetFingerprintingControlType(JNIEnv* env) {
  adrbrowsiel_shields::ControlType control_type =
      adrbrowsiel_shields::GetFingerprintingControlType(
          HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile()),
          GURL());

  return base::android::ConvertUTF8ToJavaString(
      env, adrbrowsiel_shields::ControlTypeToString(control_type));
}

void JNI_adrbrowsielPrefServiceBridge_SetNoScriptControlType(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type) {
  adrbrowsiel_shields::SetNoScriptControlType(
      HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile()),
      adrbrowsiel_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(), g_browser_process->local_state());
}

base::android::ScopedJavaLocalRef<jstring>
JNI_adrbrowsielPrefServiceBridge_GetNoScriptControlType(JNIEnv* env) {
  adrbrowsiel_shields::ControlType control_type =
      adrbrowsiel_shields::GetNoScriptControlType(
          HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile()),
          GURL());

  return base::android::ConvertUTF8ToJavaString(
      env, adrbrowsiel_shields::ControlTypeToString(control_type));
}

void JNI_adrbrowsielPrefServiceBridge_SetPlayYTVideoInBrowserEnabled(
    JNIEnv* env,
    jboolean enabled) {
  return GetOriginalProfile()->GetPrefs()->SetBoolean(
      kPlayYTVideoInBrowserEnabled, enabled);
}

jboolean JNI_adrbrowsielPrefServiceBridge_GetPlayYTVideoInBrowserEnabled(
    JNIEnv* env) {
  return GetOriginalProfile()->GetPrefs()->GetBoolean(
      kPlayYTVideoInBrowserEnabled);
}

void JNI_adrbrowsielPrefServiceBridge_SetBackgroundVideoPlaybackEnabled(
    JNIEnv* env,
    jboolean enabled) {
  return GetOriginalProfile()->GetPrefs()->SetBoolean(
      kBackgroundVideoPlaybackEnabled,
      enabled);
}

jboolean JNI_adrbrowsielPrefServiceBridge_GetBackgroundVideoPlaybackEnabled(
    JNIEnv* env) {
  return GetOriginalProfile()->GetPrefs()->GetBoolean(
      kBackgroundVideoPlaybackEnabled);
}

void JNI_adrbrowsielPrefServiceBridge_SetDesktopModeEnabled(
    JNIEnv* env,
    jboolean enabled) {
  return GetOriginalProfile()->GetPrefs()->SetBoolean(kDesktopModeEnabled,
                                                      enabled);
}

jboolean JNI_adrbrowsielPrefServiceBridge_GetDesktopModeEnabled(
    JNIEnv* env) {
  return GetOriginalProfile()->GetPrefs()->GetBoolean(kDesktopModeEnabled);
}

jlong JNI_adrbrowsielPrefServiceBridge_GetTrackersBlockedCount(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile);
  return profile->GetPrefs()->GetUint64(kTrackersBlocked);
}

jlong JNI_adrbrowsielPrefServiceBridge_GetAdsBlockedCount(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile);
  return profile->GetPrefs()->GetUint64(kAdsBlocked);
}

jlong JNI_adrbrowsielPrefServiceBridge_GetDataSaved(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile) {
#if BUILDFLAG(ENABLE_adrbrowsiel_PERF_PREDICTOR)
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile);
  return profile->GetPrefs()->GetUint64(
      adrbrowsiel_perf_predictor::prefs::kBandwidthSavedBytes);
#endif
  return 0;
}

void JNI_adrbrowsielPrefServiceBridge_SetOldTrackersBlockedCount(JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile,
    jlong count) {
  if (count <= 0) {
    return;
  }
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile);
  profile->GetPrefs()->SetUint64(kTrackersBlocked,
    count + profile->GetPrefs()->GetUint64(kTrackersBlocked));
}

void JNI_adrbrowsielPrefServiceBridge_SetOldAdsBlockedCount(JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile,
    jlong count) {
  if (count <= 0) {
    return;
  }
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile);
  profile->GetPrefs()->SetUint64(kAdsBlocked,
    count + profile->GetPrefs()->GetUint64(kAdsBlocked));
}

void JNI_adrbrowsielPrefServiceBridge_SetOldHttpsUpgradesCount(JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile,
    jlong count) {
  if (count <= 0) {
    return;
  }
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile);
  profile->GetPrefs()->SetUint64(kHttpsUpgrades,
    count + profile->GetPrefs()->GetUint64(kHttpsUpgrades));
}

void JNI_adrbrowsielPrefServiceBridge_SetSafetynetCheckFailed(
    JNIEnv* env,
    jboolean value) {
  GetOriginalProfile()->GetPrefs()->SetBoolean(kSafetynetCheckFailed, value);
}

jboolean JNI_adrbrowsielPrefServiceBridge_GetSafetynetCheckFailed(JNIEnv* env) {
  return GetOriginalProfile()->GetPrefs()->GetBoolean(kSafetynetCheckFailed);
}

void JNI_adrbrowsielPrefServiceBridge_SetSafetynetStatus(
    JNIEnv* env,
    const JavaParamRef<jstring>& status) {
  g_browser_process->local_state()->SetString(
      kSafetynetStatus, ConvertJavaStringToUTF8(env, status));
}

void JNI_adrbrowsielPrefServiceBridge_SetUseRewardsStagingServer(
    JNIEnv* env,
    jboolean enabled) {
  GetOriginalProfile()->GetPrefs()->SetBoolean(
      adrbrowsiel_rewards::prefs::kUseRewardsStagingServer, enabled);
}

void JNI_adrbrowsielPrefServiceBridge_ResetPromotionLastFetchStamp(JNIEnv* env) {
  GetOriginalProfile()->GetPrefs()->SetUint64(
      adrbrowsiel_rewards::prefs::kPromotionLastFetchStamp, 0);
}

jboolean JNI_adrbrowsielPrefServiceBridge_GetUseRewardsStagingServer(JNIEnv* env) {
  return GetOriginalProfile()->GetPrefs()->GetBoolean(
      adrbrowsiel_rewards::prefs::kUseRewardsStagingServer);
}

jboolean JNI_adrbrowsielPrefServiceBridge_GetBooleanForContentSetting(JNIEnv* env,
    jint type) {
  HostContentSettingsMap* content_settings =
      HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile());
  switch (content_settings->GetDefaultContentSetting((ContentSettingsType)type,
      nullptr)) {
    case CONTENT_SETTING_ALLOW:
      return true;
    case CONTENT_SETTING_BLOCK:
    default:
      return false;
  }

  return false;
}

void JNI_adrbrowsielPrefServiceBridge_SetReferralAndroidFirstRunTimestamp(
    JNIEnv* env,
    jlong time) {
  return g_browser_process->local_state()->SetTime(
      kReferralAndroidFirstRunTimestamp, base::Time::FromJavaTime(time));
}

void JNI_adrbrowsielPrefServiceBridge_SetReferralCheckedForPromoCodeFile(
    JNIEnv* env,
    jboolean value) {
  return g_browser_process->local_state()->SetBoolean(
      kReferralCheckedForPromoCodeFile, value);
}

void JNI_adrbrowsielPrefServiceBridge_SetReferralInitialization(
    JNIEnv* env,
    jboolean value) {
  return g_browser_process->local_state()->SetBoolean(
      kReferralInitialization, value);
}

void JNI_adrbrowsielPrefServiceBridge_SetReferralPromoCode(
    JNIEnv* env,
    const JavaParamRef<jstring>& promoCode) {
  return g_browser_process->local_state()->SetString(
      kReferralPromoCode, ConvertJavaStringToUTF8(env, promoCode));
}

void JNI_adrbrowsielPrefServiceBridge_SetReferralDownloadId(
    JNIEnv* env,
    const JavaParamRef<jstring>& downloadId) {
  return g_browser_process->local_state()->SetString(
      kReferralDownloadID, ConvertJavaStringToUTF8(env, downloadId));
}

jint JNI_adrbrowsielPrefServiceBridge_GetWebrtcPolicy(JNIEnv* env) {
  return static_cast<int>(
      GetWebRTCIPHandlingPolicy(GetOriginalProfile()->GetPrefs()->GetString(
          prefs::kWebRTCIPHandlingPolicy)));
}

void JNI_adrbrowsielPrefServiceBridge_SetWebrtcPolicy(JNIEnv* env, jint policy) {
  GetOriginalProfile()->GetPrefs()->SetString(
      prefs::kWebRTCIPHandlingPolicy,
      GetWebRTCIPHandlingPreference((WebRTCIPHandlingPolicy)policy));
}

#if BUILDFLAG(adrbrowsiel_P3A_ENABLED)
void JNI_adrbrowsielPrefServiceBridge_SetP3AEnabled(
    JNIEnv* env,
    jboolean value) {
  /* Saving pref value to the disk as soon as the pref value
   * is set to avoid delay in pref value update.*/
  g_browser_process->local_state()->SetBoolean(
       adrbrowsiel::kP3AEnabled, value);
  g_browser_process->local_state()->CommitPendingWrite();
}

jboolean JNI_adrbrowsielPrefServiceBridge_GetP3AEnabled(
    JNIEnv* env) {
  return g_browser_process->local_state()->GetBoolean(
      adrbrowsiel::kP3AEnabled);
}

jboolean JNI_adrbrowsielPrefServiceBridge_HasPathP3AEnabled(
    JNIEnv* env) {
  return g_browser_process->local_state()->HasPrefPath(adrbrowsiel::kP3AEnabled);
}

void JNI_adrbrowsielPrefServiceBridge_SetP3ANoticeAcknowledged(
    JNIEnv* env,
    jboolean value) {
  return g_browser_process->local_state()->SetBoolean(
      adrbrowsiel::kP3ANoticeAcknowledged, value);
}

jboolean JNI_adrbrowsielPrefServiceBridge_GetP3ANoticeAcknowledged(
    JNIEnv* env) {
  return g_browser_process->local_state()->GetBoolean(
      adrbrowsiel::kP3ANoticeAcknowledged);
}

#else

void JNI_adrbrowsielPrefServiceBridge_SetP3AEnabled(JNIEnv* env, jboolean value) {}

jboolean JNI_adrbrowsielPrefServiceBridge_GetP3AEnabled(JNIEnv* env) {
  return false;
}

jboolean JNI_adrbrowsielPrefServiceBridge_HasPathP3AEnabled(JNIEnv* env) {}

void JNI_adrbrowsielPrefServiceBridge_SetP3ANoticeAcknowledged(JNIEnv* env,
    jboolean value) {}

jboolean JNI_adrbrowsielPrefServiceBridge_GetP3ANoticeAcknowledged(JNIEnv* env) {
  return false;
}
#endif  // BUILDFLAG(adrbrowsiel_P3A_ENABLED)

void JNI_adrbrowsielPrefServiceBridge_SetUnstoppableDomainsResolveMethod(
    JNIEnv* env,
    jint method) {
#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
  g_browser_process->local_state()->SetInteger(
      decentralized_dns::kUnstoppableDomainsResolveMethod, method);
#endif
}

jint JNI_adrbrowsielPrefServiceBridge_GetUnstoppableDomainsResolveMethod(
    JNIEnv* env) {
#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
  return g_browser_process->local_state()->GetInteger(
      decentralized_dns::kUnstoppableDomainsResolveMethod);
#else
  return 0;
#endif
}

void JNI_adrbrowsielPrefServiceBridge_SetENSResolveMethod(JNIEnv* env, jint method) {
#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
  g_browser_process->local_state()->SetInteger(
      decentralized_dns::kENSResolveMethod, method);
#endif
}

jint JNI_adrbrowsielPrefServiceBridge_GetENSResolveMethod(JNIEnv* env) {
#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
  return g_browser_process->local_state()->GetInteger(
      decentralized_dns::kENSResolveMethod);
#else
  return 0;
#endif
}

}  // namespace android
}  // namespace chrome
