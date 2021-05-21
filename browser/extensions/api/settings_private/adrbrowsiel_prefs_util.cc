/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/extensions/api/settings_private/adrbrowsiel_prefs_util.h"

#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_wayback_machine/buildflags.h"
#include "adrbrowsiel/components/crypto_dot_com/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/decentralized_dns/buildflags/buildflags.h"
#include "adrbrowsiel/components/ftx/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"
#include "adrbrowsiel/components/ntp_background_images/common/pref_names.h"
#include "adrbrowsiel/components/sidebar/buildflags/buildflags.h"
#include "adrbrowsiel/components/tor/buildflags/buildflags.h"
#include "chrome/browser/extensions/api/settings_private/prefs_util.h"
#include "chrome/common/extensions/api/settings_private.h"
#include "chrome/common/pref_names.h"
#include "components/browsing_data/core/pref_names.h"
#include "components/gcm_driver/gcm_buildflags.h"
#include "components/omnibox/browser/omnibox_prefs.h"

#if BUILDFLAG(ENABLE_adrbrowsiel_WAYBACK_MACHINE)
#include "adrbrowsiel/components/adrbrowsiel_wayback_machine/pref_names.h"
#endif

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/pref_names.h"
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "adrbrowsiel/components/ipfs/pref_names.h"
#endif

#if BUILDFLAG(CRYPTO_DOT_COM_ENABLED)
#include "adrbrowsiel/components/crypto_dot_com/common/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_SIDEBAR)
#include "adrbrowsiel/components/sidebar/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "adrbrowsiel/components/tor/pref_names.h"
#endif

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
#include "adrbrowsiel/components/decentralized_dns/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_FTX)
#include "adrbrowsiel/components/ftx/common/pref_names.h"
#endif

namespace extensions {

using ntp_background_images::prefs::kNewTabPageShowBackgroundImage;
using ntp_background_images::prefs::
    kNewTabPageShowSponsoredImagesBackgroundImage;
using ntp_background_images::prefs::kNewTabPageSuperReferralThemesOption;

namespace settings_api = api::settings_private;

const PrefsUtil::TypedPrefMap& adrbrowsielPrefsUtil::GetAllowlistedKeys() {
  // Static cache, similar to parent class
  static PrefsUtil::TypedPrefMap* s_adrbrowsiel_allowlist = nullptr;
  if (s_adrbrowsiel_allowlist)
    return *s_adrbrowsiel_allowlist;
  s_adrbrowsiel_allowlist = new PrefsUtil::TypedPrefMap();
  // Start with parent class allowlist
  const auto chromium_prefs = PrefsUtil::GetAllowlistedKeys();
  s_adrbrowsiel_allowlist->insert(chromium_prefs.begin(), chromium_prefs.end());
  // Add adrbrowsiel values to the allowlist
  // import data
  (*s_adrbrowsiel_allowlist)[kImportDialogExtensions] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kImportDialogPayments] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  // Default adrbrowsiel shields
  (*s_adrbrowsiel_allowlist)[kShieldsAdvancedViewEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kShieldsStatsBadgeVisible] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kAdControlType] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kHTTPSEVerywhereControlType] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kNoScriptControlType] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kGoogleLoginControlType] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[adrbrowsiel_shields::prefs::kFBEmbedControlType] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[adrbrowsiel_shields::prefs::kTwitterEmbedControlType] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[adrbrowsiel_shields::prefs::kLinkedInEmbedControlType] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;

  // appearance prefs
  (*s_adrbrowsiel_allowlist)[kLocationBarIsWide] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kAutocompleteEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kTopSiteSuggestionsEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kadrbrowsielSuggestedSiteSuggestionsEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[adrbrowsiel_rewards::prefs::kHideButton] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kAskWidevineInstall] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kNewTabPageSuperReferralThemesOption] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
#if BUILDFLAG(ENABLE_SIDEBAR)
  (*s_adrbrowsiel_allowlist)[sidebar::kSidebarShowOption] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
#endif
  // new tab prefs
  (*s_adrbrowsiel_allowlist)[kNewTabPageShowSponsoredImagesBackgroundImage] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kNewTabPageShowBackgroundImage] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kNewTabPageShowClock] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kNewTabPageShowStats] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kNewTabPageShowRewards] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kNewTabPageShowToday] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kNewTabPageShowBinance] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kNewTabPageShowTogether] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kNewTabPageShowGemini] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kNewTabPageShowsOptions] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
#if BUILDFLAG(CRYPTO_DOT_COM_ENABLED)
  (*s_adrbrowsiel_allowlist)[kCryptoDotComNewTabPageShowCryptoDotCom] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#endif
#if BUILDFLAG(ENABLE_FTX)
  (*s_adrbrowsiel_allowlist)[kFTXNewTabPageShowFTX] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#endif
  // adrbrowsiel today prefs
  (*s_adrbrowsiel_allowlist)[kadrbrowsielTodayOptedIn] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kadrbrowsielTodaySources] =
      settings_api::PrefType::PREF_TYPE_DICTIONARY;
  // Clear browsing data on exit prefs.
  (*s_adrbrowsiel_allowlist)[browsing_data::prefs::kDeleteBrowsingHistoryOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[browsing_data::prefs::kDeleteDownloadHistoryOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[browsing_data::prefs::kDeleteCacheOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[browsing_data::prefs::kDeleteCookiesOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[browsing_data::prefs::kDeletePasswordsOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[browsing_data::prefs::kDeleteFormDataOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[browsing_data::prefs::kDeleteSiteSettingsOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[browsing_data::prefs::kDeleteHostedAppsDataOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kAlwaysShowBookmarkBarOnNTP] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kMRUCyclingEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  // WebTorrent pref
  (*s_adrbrowsiel_allowlist)[kWebTorrentEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#if BUILDFLAG(ENABLE_adrbrowsiel_WAYBACK_MACHINE)
  (*s_adrbrowsiel_allowlist)[kadrbrowsielWaybackMachineEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#endif
  // Hangouts pref
  (*s_adrbrowsiel_allowlist)[kHangoutsEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  // IPFS Companion pref
  (*s_adrbrowsiel_allowlist)[kIPFSCompanionEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  // adrbrowsiel Wallet pref
#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
  (*s_adrbrowsiel_allowlist)[kadrbrowsielWalletWeb3Provider] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
  (*s_adrbrowsiel_allowlist)[kLoadCryptoWalletsOnStartup] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kShowWalletIconOnToolbar] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#endif
  // IPFS pref
#if BUILDFLAG(IPFS_ENABLED)
  (*s_adrbrowsiel_allowlist)[kIPFSResolveMethod] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
  (*s_adrbrowsiel_allowlist)[kIPFSAutoFallbackToGateway] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kIPFSPublicGatewayAddress] =
      settings_api::PrefType::PREF_TYPE_STRING;
  (*s_adrbrowsiel_allowlist)[kIPFSAutoRedirectGateway] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kIPFSAutoRedirectDNSLink] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_adrbrowsiel_allowlist)[kIpfsStorageMax] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
#endif
  // Media Router Pref
  (*s_adrbrowsiel_allowlist)[kadrbrowsielEnabledMediaRouter] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;

#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
  // Push Messaging Pref
  (*s_adrbrowsiel_allowlist)[kadrbrowsielGCMChannelStatus] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#endif
  // Omnibox pref
  (*s_adrbrowsiel_allowlist)[omnibox::kPreventUrlElisionsInOmnibox] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#if BUILDFLAG(ENABLE_TOR)
  (*s_adrbrowsiel_allowlist)[tor::prefs::kAutoOnionRedirect] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#endif
  (*s_adrbrowsiel_allowlist)[prefs::kWebRTCIPHandlingPolicy] =
      settings_api::PrefType::PREF_TYPE_STRING;

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
  (*s_adrbrowsiel_allowlist)[decentralized_dns::kUnstoppableDomainsResolveMethod] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
  (*s_adrbrowsiel_allowlist)[decentralized_dns::kENSResolveMethod] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
#endif

  return *s_adrbrowsiel_allowlist;
}

}  // namespace extensions
