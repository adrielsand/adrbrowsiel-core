/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/app/adrbrowsiel_main_delegate.h"

#include <memory>
#include <string>
#include <unordered_set>

#include "base/base_switches.h"
#include "base/lazy_instance.h"
#include "base/path_service.h"
#include "base/task/post_task.h"
#include "base/time/time.h"
#include "adrbrowsiel/app/adrbrowsiel_command_line_helper.h"
#include "adrbrowsiel/browser/adrbrowsiel_content_browser_client.h"
#include "adrbrowsiel/common/adrbrowsiel_switches.h"
#include "adrbrowsiel/common/resource_bundle_helper.h"
#include "adrbrowsiel/components/adrbrowsiel_ads/browser/buildflags/buildflags.h"
#include "adrbrowsiel/renderer/adrbrowsiel_content_renderer_client.h"
#include "adrbrowsiel/utility/adrbrowsiel_content_utility_client.h"
#include "build/build_config.h"
#include "chrome/browser/ui/ui_features.h"
#include "chrome/common/chrome_features.h"
#include "chrome/common/chrome_paths.h"
#include "chrome/common/chrome_paths_internal.h"
#include "chrome/common/chrome_switches.h"
#include "components/autofill/core/common/autofill_features.h"
#include "components/autofill/core/common/autofill_payments_features.h"
#include "components/embedder_support/switches.h"
#include "components/federated_learning/features/features.h"
#include "components/feed/feed_feature_list.h"
#include "components/language/core/common/language_experiments.h"
#include "components/network_time/network_time_tracker.h"
#include "components/offline_pages/core/offline_page_feature.h"
#include "components/omnibox/common/omnibox_features.h"
#include "components/password_manager/core/common/password_manager_features.h"
#include "components/security_state/core/features.h"
#include "components/sync/base/sync_base_switches.h"
#include "components/translate/core/browser/translate_prefs.h"
#include "components/variations/variations_switches.h"
#include "content/public/common/content_features.h"
#include "content/public/common/content_switches.h"
#include "google_apis/gaia/gaia_switches.h"
#include "media/base/media_switches.h"
#include "net/base/features.h"
#include "services/device/public/cpp/device_features.h"
#include "services/network/public/cpp/features.h"
#include "third_party/blink/public/common/features.h"
#include "ui/base/ui_base_features.h"

#if BUILDFLAG(adrbrowsiel_ADS_ENABLED)
#include "components/dom_distiller/core/dom_distiller_switches.h"
#endif

#if defined(OS_ANDROID)
#include "base/android/jni_android.h"
#include "adrbrowsiel/build/android/jni_headers/adrbrowsielQAPreferences_jni.h"
#include "components/signin/public/base/account_consistency_method.h"
#else
#include "chrome/browser/ui/profile_picker.h"
#endif

namespace {
// staging "https://sync-v2.adrbrowsielsoftware.com/v2" can be overriden by
// switches::kSyncServiceURL manually
const char kadrbrowsielSyncServiceStagingURL[] =
    "https://sync-v2.adrbrowsielsoftware.com/v2";
}  // namespace

#if !defined(CHROME_MULTIPLE_DLL_BROWSER)
base::LazyInstance<adrbrowsielContentRendererClient>::DestructorAtExit
    g_adrbrowsiel_content_renderer_client = LAZY_INSTANCE_INITIALIZER;
base::LazyInstance<adrbrowsielContentUtilityClient>::DestructorAtExit
    g_adrbrowsiel_content_utility_client = LAZY_INSTANCE_INITIALIZER;
#endif
#if !defined(CHROME_MULTIPLE_DLL_CHILD)
base::LazyInstance<adrbrowsielContentBrowserClient>::DestructorAtExit
    g_adrbrowsiel_content_browser_client = LAZY_INSTANCE_INITIALIZER;
#endif

const char kadrbrowsielOriginTrialsPublicKey[] =
    "bYUKPJoPnCxeNvu72j4EmPuK7tr1PAC7SHh8ld9Mw3E=,"
    "fMS4mpO6buLQ/QMd+zJmxzty/VQ6B1EUZqoCU04zoRU=";

const char kDummyUrl[] = "https://no-thanks.invalid";

adrbrowsielMainDelegate::adrbrowsielMainDelegate() : ChromeMainDelegate() {}

adrbrowsielMainDelegate::adrbrowsielMainDelegate(base::TimeTicks exe_entry_point_ticks)
    : ChromeMainDelegate(exe_entry_point_ticks) {}

adrbrowsielMainDelegate::~adrbrowsielMainDelegate() {}

content::ContentBrowserClient* adrbrowsielMainDelegate::CreateContentBrowserClient() {
#if defined(CHROME_MULTIPLE_DLL_CHILD)
  return NULL;
#else
  if (chrome_content_browser_client_ == nullptr) {
    chrome_content_browser_client_ =
        std::make_unique<adrbrowsielContentBrowserClient>();
  }
  return chrome_content_browser_client_.get();
#endif
}

content::ContentRendererClient*
adrbrowsielMainDelegate::CreateContentRendererClient() {
#if defined(CHROME_MULTIPLE_DLL_BROWSER)
  return NULL;
#else
  return g_adrbrowsiel_content_renderer_client.Pointer();
#endif
}

content::ContentUtilityClient* adrbrowsielMainDelegate::CreateContentUtilityClient() {
#if defined(CHROME_MULTIPLE_DLL_BROWSER)
  return NULL;
#else
  return g_adrbrowsiel_content_utility_client.Pointer();
#endif
}

void adrbrowsielMainDelegate::PreSandboxStartup() {
  ChromeMainDelegate::PreSandboxStartup();
#if defined(OS_LINUX) || defined(OS_MAC)
  // Setup NativeMessagingHosts to point to the default Chrome locations
  // because that's where native apps will create them
  base::FilePath chrome_user_data_dir;
  base::FilePath native_messaging_dir;
#if defined(OS_MAC)
  base::PathService::Get(base::DIR_APP_DATA, &chrome_user_data_dir);
  chrome_user_data_dir = chrome_user_data_dir.Append("Google/Chrome");
  native_messaging_dir = base::FilePath(
      FILE_PATH_LITERAL("/Library/Google/Chrome/NativeMessagingHosts"));
#else
  chrome::GetDefaultUserDataDirectory(&chrome_user_data_dir);
  native_messaging_dir = base::FilePath(
      FILE_PATH_LITERAL("/etc/opt/chrome/native-messaging-hosts"));
#endif  // defined(OS_MAC)
  base::PathService::OverrideAndCreateIfNeeded(
      chrome::DIR_USER_NATIVE_MESSAGING,
      chrome_user_data_dir.Append(FILE_PATH_LITERAL("NativeMessagingHosts")),
      false, true);
  base::PathService::OverrideAndCreateIfNeeded(
      chrome::DIR_NATIVE_MESSAGING, native_messaging_dir, false, true);
#endif  // defined(OS_LINUX) || defined(OS_MAC)

#if defined(OS_POSIX) && !defined(OS_MAC)
  base::PathService::Override(
      chrome::DIR_POLICY_FILES,
      base::FilePath(FILE_PATH_LITERAL("/etc/adrbrowsiel/policies")));
#endif

  if (adrbrowsiel::SubprocessNeedsResourceBundle()) {
    adrbrowsiel::InitializeResourceBundle();
  }
}

bool adrbrowsielMainDelegate::BasicStartupComplete(int* exit_code) {
  adrbrowsielCommandLineHelper command_line(base::CommandLine::ForCurrentProcess());
#if BUILDFLAG(adrbrowsiel_ADS_ENABLED)
  command_line.AppendSwitch(switches::kEnableDomDistiller);
#endif
  command_line.AppendSwitch(switches::kDisableDomainReliability);
  command_line.AppendSwitch(switches::kNoPings);

  if (!base::CommandLine::ForCurrentProcess()->HasSwitch(
          embedder_support::kOriginTrialPublicKey)) {
    command_line.AppendSwitchASCII(embedder_support::kOriginTrialPublicKey,
                                   kadrbrowsielOriginTrialsPublicKey);
  }

  std::string adrbrowsiel_sync_service_url = adrbrowsiel_SYNC_ENDPOINT;
#if defined(OS_ANDROID)
  AdjustSyncServiceUrlForAndroid(&adrbrowsiel_sync_service_url);
#endif  // defined(OS_ANDROID)

  // adrbrowsiel's sync protocol does not use the sync service url
  command_line.AppendSwitchASCII(switches::kSyncServiceURL,
                                 adrbrowsiel_sync_service_url.c_str());

  command_line.AppendSwitchASCII(switches::kLsoUrl, kDummyUrl);

  // adrbrowsiel variations
  std::string kVariationsServerURL = adrbrowsiel_VARIATIONS_SERVER_URL;
  command_line.AppendSwitchASCII(variations::switches::kVariationsServerURL,
                                 kVariationsServerURL.c_str());

  // Enabled features.
  std::unordered_set<const char*> enabled_features = {
    // Upgrade all mixed content
    blink::features::kMixedContentAutoupgrade.name,
    password_manager::features::kPasswordImport.name,
    net::features::kLegacyTLSEnforced.name,
    // Enable webui dark theme: @media (prefers-color-scheme: dark) is gated
    // on this feature.
    features::kWebUIDarkMode.name,
    blink::features::kPrefetchPrivacyChanges.name,
    blink::features::kReducedReferrerGranularity.name,
#if defined(OS_WIN)
    features::kWinrtGeolocationImplementation.name,
#endif
    security_state::features::kSafetyTipUI.name,
  };

  // Disabled features.
  std::unordered_set<const char*> disabled_features = {
    autofill::features::kAutofillEnableAccountWalletStorage.name,
    autofill::features::kAutofillServerCommunication.name,
    blink::features::kFledgeInterestGroupAPI.name,
    blink::features::kFledgeInterestGroups.name,
    blink::features::kHandwritingRecognitionWebPlatformApi.name,
    blink::features::kHandwritingRecognitionWebPlatformApiFinch.name,
    blink::features::kInterestCohortAPIOriginTrial.name,
    blink::features::kInterestCohortFeaturePolicy.name,
    blink::features::kTextFragmentAnchor.name,
    features::kDirectSockets.name,
    features::kIdleDetection.name,
    features::kLangClientHintHeader.name,
    features::kNotificationTriggers.name,
    features::kSignedExchangePrefetchCacheForNavigations.name,
    features::kSignedExchangeSubresourcePrefetch.name,
    features::kSubresourceWebBundles.name,
    features::kWebOTP.name,
    federated_learning::kFederatedLearningOfCohorts.name,
    federated_learning::kFlocIdComputedEventLogging.name,
    media::kLiveCaption.name,
    net::features::kFirstPartySets.name,
    network::features::kTrustTokens.name,
    network_time::kNetworkTimeServiceQuerying.name,
#if defined(OS_ANDROID)
    features::kWebNfc.name,
    feed::kInterestFeedContentSuggestions.name,
    feed::kInterestFeedV2.name,
    offline_pages::kPrefetchingOfflinePagesFeature.name,
    signin::kMobileIdentityConsistency.name,
    translate::kTranslate.name,
#else
    kEnableProfilePickerOnStartupFeature.name,
#endif
  };

#if defined(OS_WIN) || defined(OS_MAC) || defined(OS_ANDROID)
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableDnsOverHttps)) {
    disabled_features.insert(features::kDnsOverHttps.name);
  }
#else
  if (!base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableDnsOverHttps)) {
    enabled_features.insert(features::kDnsOverHttps.name);
  }
#endif

  command_line.AppendFeatures(enabled_features, disabled_features);

  bool ret = ChromeMainDelegate::BasicStartupComplete(exit_code);

  return ret;
}

#if defined(OS_ANDROID)
void adrbrowsielMainDelegate::AdjustSyncServiceUrlForAndroid(
    std::string* adrbrowsiel_sync_service_url) {
  DCHECK_NE(adrbrowsiel_sync_service_url, nullptr);
  const char kProcessTypeSwitchName[] = "type";

  // On Android we can detect data dir only on host process, and we cannot
  // for example on renderer or gpu-process, because JNI is not initialized
  // And no sense to override sync service url for them in anyway
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          kProcessTypeSwitchName)) {
    // This is something other than browser process
    return;
  }

  JNIEnv* env = base::android::AttachCurrentThread();
  bool b_use_staging_sync_server =
      Java_adrbrowsielQAPreferences_isSyncStagingUsed(env);
  if (b_use_staging_sync_server) {
    *adrbrowsiel_sync_service_url = kadrbrowsielSyncServiceStagingURL;
  }
}
#endif  // defined(OS_ANDROID)