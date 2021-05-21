/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/about_flags.h"

#include "base/strings/string_util.h"
#include "adrbrowsiel/common/adrbrowsiel_features.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_component_updater/browser/features.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/common/features.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/common/features.h"
#include "adrbrowsiel/components/adrbrowsiel_sync/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/buildflags/buildflags.h"
#include "adrbrowsiel/components/decentralized_dns/buildflags/buildflags.h"
#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"
#include "adrbrowsiel/components/ntp_background_images/browser/features.h"
#include "adrbrowsiel/components/sidebar/buildflags/buildflags.h"
#include "adrbrowsiel/components/speedreader/buildflags.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "components/permissions/features.h"
#include "components/prefs/pref_service.h"
#include "net/base/features.h"

using adrbrowsiel_shields::features::kadrbrowsielAdblockCosmeticFiltering;
using adrbrowsiel_shields::features::kadrbrowsielAdblockCosmeticFilteringNative;
using adrbrowsiel_shields::features::kadrbrowsielAdblockCspRules;
using adrbrowsiel_shields::features::kadrbrowsielDomainBlock;
using adrbrowsiel_shields::features::kadrbrowsielExtensionNetworkBlocking;
using ntp_background_images::features::kadrbrowsielNTPBrandedWallpaper;
using ntp_background_images::features::kadrbrowsielNTPBrandedWallpaperDemo;
using ntp_background_images::features::kadrbrowsielNTPSuperReferralWallpaper;

// clang-format seems to have a lot of issues with the macros in this
// file so we turn it off for the macro sections.
// clang-format off

#if BUILDFLAG(ENABLE_SIDEBAR)
#include "adrbrowsiel/components/sidebar/features.h"

#define SIDEBAR_FEATURE_ENTRIES \
    {"sidebar",                                                            \
     flag_descriptions::kadrbrowsielSidebarName,                                 \
     flag_descriptions::kadrbrowsielSidebarDescription,                          \
     kOsMac | kOsWin | kOsLinux,                                           \
     FEATURE_VALUE_TYPE(sidebar::kSidebarFeature)},
#else
#define SIDEBAR_FEATURE_ENTRIES
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "adrbrowsiel/components/speedreader/features.h"

#define SPEEDREADER_FEATURE_ENTRIES \
    {"adrbrowsiel-speedreader",                                               \
     flag_descriptions::kadrbrowsielSpeedreaderName,                          \
     flag_descriptions::kadrbrowsielSpeedreaderDescription, kOsDesktop,       \
     FEATURE_VALUE_TYPE(speedreader::kSpeedreaderFeature)},             \
    {"speedreader-legacy-backend",                                      \
     flag_descriptions::kadrbrowsielSpeedreaderLegacyName,                    \
     flag_descriptions::kadrbrowsielSpeedreaderLegacyDescription, kOsDesktop, \
     FEATURE_VALUE_TYPE(speedreader::kSpeedreaderLegacyBackend)},
#else
#define SPEEDREADER_FEATURE_ENTRIES
#endif

#if BUILDFLAG(ENABLE_adrbrowsiel_SYNC)
#include "adrbrowsiel/components/adrbrowsiel_sync/features.h"

#define adrbrowsiel_SYNC_FEATURE_ENTRIES                                         \
    {"adrbrowsiel-sync-v2",                                                      \
     flag_descriptions::kadrbrowsielSyncName,                                    \
     flag_descriptions::kadrbrowsielSyncDescription, kOsDesktop,                 \
     FEATURE_VALUE_TYPE(adrbrowsiel_sync::features::kadrbrowsielSync)},
#else
#define adrbrowsiel_SYNC_FEATURE_ENTRIES
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "adrbrowsiel/components/ipfs/features.h"

#define adrbrowsiel_IPFS_FEATURE_ENTRIES                                         \
    {"adrbrowsiel-ipfs",                                                         \
     flag_descriptions::kadrbrowsielIpfsName,                                    \
     flag_descriptions::kadrbrowsielIpfsDescription,                             \
     kOsDesktop | kOsAndroid,                                              \
     FEATURE_VALUE_TYPE(ipfs::features::kIpfsFeature)},
#else
#define adrbrowsiel_IPFS_FEATURE_ENTRIES
#endif

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/features.h"

#define adrbrowsiel_NATIVE_WALLET_FEATURE_ENTRIES                                  \
    {"native-adrbrowsiel-wallet",                                                  \
     flag_descriptions::kNativeadrbrowsielWalletName,                              \
     flag_descriptions::kNativeadrbrowsielWalletDescription,                       \
     kOsDesktop | flags_ui::kOsAndroid,                                      \
     FEATURE_VALUE_TYPE(adrbrowsiel_wallet::features::kNativeadrbrowsielWalletFeature)},
#else
#define adrbrowsiel_NATIVE_WALLET_FEATURE_ENTRIES
#endif

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
#include "adrbrowsiel/components/decentralized_dns/features.h"

#define adrbrowsiel_DECENTRALIZED_DNS_FEATURE_ENTRIES                             \
    {"adrbrowsiel-decentralized-dns",                                             \
     flag_descriptions::kadrbrowsielDecentralizedDnsName,                         \
     flag_descriptions::kadrbrowsielDecentralizedDnsDescription,                  \
     kOsDesktop | kOsAndroid,                                               \
     FEATURE_VALUE_TYPE(decentralized_dns::features::kDecentralizedDns)},
#else
#define adrbrowsiel_DECENTRALIZED_DNS_FEATURE_ENTRIES
#endif

#define adrbrowsiel_FEATURE_ENTRIES                                               \
    {"use-dev-updater-url",                                                 \
     flag_descriptions::kUseDevUpdaterUrlName,                              \
     flag_descriptions::kUseDevUpdaterUrlDescription, kOsAll,               \
     FEATURE_VALUE_TYPE(adrbrowsiel_component_updater::kUseDevUpdaterUrl)},       \
    {"adrbrowsiel-ntp-branded-wallpaper",                                         \
     flag_descriptions::kadrbrowsielNTPBrandedWallpaperName,                      \
     flag_descriptions::kadrbrowsielNTPBrandedWallpaperDescription, kOsAll,       \
     FEATURE_VALUE_TYPE(kadrbrowsielNTPBrandedWallpaper)},                        \
    {"adrbrowsiel-ntp-branded-wallpaper-demo",                                    \
     flag_descriptions::kadrbrowsielNTPBrandedWallpaperDemoName,                  \
     flag_descriptions::kadrbrowsielNTPBrandedWallpaperDemoDescription, kOsAll,   \
     FEATURE_VALUE_TYPE(kadrbrowsielNTPBrandedWallpaperDemo)},                    \
    {"adrbrowsiel-adblock-cosmetic-filtering",                                    \
     flag_descriptions::kadrbrowsielAdblockCosmeticFilteringName,                 \
     flag_descriptions::kadrbrowsielAdblockCosmeticFilteringDescription, kOsAll,  \
     FEATURE_VALUE_TYPE(kadrbrowsielAdblockCosmeticFiltering)},                   \
    {"adrbrowsiel-adblock-cosmetic-filtering-native",                             \
     flag_descriptions::kadrbrowsielAdblockCosmeticFilteringNativeName,           \
     flag_descriptions::kadrbrowsielAdblockCosmeticFilteringNativeDescription,    \
     kOsMac | kOsWin | kOsLinux,                                            \
     FEATURE_VALUE_TYPE(kadrbrowsielAdblockCosmeticFilteringNative)},             \
    {"adrbrowsiel-adblock-csp-rules",                                             \
     flag_descriptions::kadrbrowsielAdblockCspRulesName,                          \
     flag_descriptions::kadrbrowsielAdblockCspRulesDescription, kOsAll,           \
     FEATURE_VALUE_TYPE(kadrbrowsielAdblockCspRules)},                            \
    {"adrbrowsiel-domain-block",                                                  \
     flag_descriptions::kadrbrowsielDomainBlockName,                              \
     flag_descriptions::kadrbrowsielDomainBlockDescription, kOsAll,               \
     FEATURE_VALUE_TYPE(kadrbrowsielDomainBlock)},                                \
    {"adrbrowsiel-extension-network-blocking",                                    \
     flag_descriptions::kadrbrowsielExtensionNetworkBlockingName,                 \
     flag_descriptions::kadrbrowsielExtensionNetworkBlockingDescription, kOsAll,  \
     FEATURE_VALUE_TYPE(kadrbrowsielExtensionNetworkBlocking)},                   \
    SPEEDREADER_FEATURE_ENTRIES                                             \
    adrbrowsiel_SYNC_FEATURE_ENTRIES                                              \
    adrbrowsiel_IPFS_FEATURE_ENTRIES                                              \
    adrbrowsiel_NATIVE_WALLET_FEATURE_ENTRIES                                     \
    SIDEBAR_FEATURE_ENTRIES                                                 \
    adrbrowsiel_DECENTRALIZED_DNS_FEATURE_ENTRIES                                 \
    {"adrbrowsiel-super-referral",                                                \
     flag_descriptions::kadrbrowsielSuperReferralName,                            \
     flag_descriptions::kadrbrowsielSuperReferralDescription,                     \
     flags_ui::kOsMac | flags_ui::kOsWin | flags_ui::kOsAndroid,            \
     FEATURE_VALUE_TYPE(kadrbrowsielNTPSuperReferralWallpaper)},                  \
    {"adrbrowsiel-ephemeral-storage",                                             \
     flag_descriptions::kadrbrowsielEphemeralStorageName,                         \
     flag_descriptions::kadrbrowsielEphemeralStorageDescription, kOsAll,          \
     FEATURE_VALUE_TYPE(net::features::kadrbrowsielEphemeralStorage)},            \
    {"adrbrowsiel-ephemeral-storage-keep-alive",                                  \
     flag_descriptions::kadrbrowsielEphemeralStorageKeepAliveName,                \
     flag_descriptions::kadrbrowsielEphemeralStorageKeepAliveDescription, kOsAll, \
     FEATURE_VALUE_TYPE(net::features::kadrbrowsielEphemeralStorageKeepAlive)},   \
    {"adrbrowsiel-permission-lifetime",                                           \
     flag_descriptions::kadrbrowsielPermissionLifetimeName,                       \
     flag_descriptions::kadrbrowsielPermissionLifetimeDescription, kOsAll,        \
     FEATURE_VALUE_TYPE(permissions::features::kPermissionLifetime)},       \
    {"adrbrowsiel-rewards-verbose-logging",                                       \
     flag_descriptions::kadrbrowsielRewardsVerboseLoggingName,                    \
     flag_descriptions::kadrbrowsielRewardsVerboseLoggingDescription, kOsDesktop, \
     FEATURE_VALUE_TYPE(adrbrowsiel_rewards::features::kVerboseLoggingFeature)},  \
    {"adrbrowsiel-rewards-bitflyer",                                              \
     flag_descriptions::kadrbrowsielRewardsBitflyerName,                          \
     flag_descriptions::kadrbrowsielRewardsBitflyerDescription, kOsDesktop,       \
     FEATURE_VALUE_TYPE(adrbrowsiel_rewards::features::kBitflyerFeature)},


#define SetFeatureEntryEnabled SetFeatureEntryEnabled_ChromiumImpl
#include "../../../../chrome/browser/about_flags.cc"  // NOLINT
#undef SetFeatureEntryEnabled
#undef adrbrowsiel_FEATURE_ENTRIES

// clang-format on

namespace about_flags {

void UpdateadrbrowsielMediaRouterPref(const std::string& internal_name,
                                Profile* profile) {
  bool enable = false;
  if (base::EndsWith(internal_name, "@1", base::CompareCase::SENSITIVE)) {
    enable = true;
  }
  if (base::StartsWith(internal_name, "load-media-router-component-extension",
                       base::CompareCase::SENSITIVE)) {
    profile->GetPrefs()->SetBoolean(kadrbrowsielEnabledMediaRouter, enable);
  }
}

void SetFeatureEntryEnabled(flags_ui::FlagsStorage* flags_storage,
                            const std::string& internal_name,
                            bool enable) {
  UpdateadrbrowsielMediaRouterPref(internal_name,
                             ProfileManager::GetActiveUserProfile());
  SetFeatureEntryEnabled_ChromiumImpl(flags_storage, internal_name, enable);
}

}  // namespace about_flags
