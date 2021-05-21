/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/adrbrowsiel_wallet_utils.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_wayback_machine/buildflags.h"
#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/net/prediction_options.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/common/pref_names.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "components/embedder_support/pref_names.h"
#include "components/prefs/pref_service.h"
#include "components/safe_browsing/core/common/safe_browsing_prefs.h"
#include "components/spellcheck/browser/pref_names.h"
#include "components/sync/base/pref_names.h"
#include "content/public/test/browser_test.h"

#if BUILDFLAG(IPFS_ENABLED)
#include "adrbrowsiel/components/ipfs/ipfs_constants.h"
#include "adrbrowsiel/components/ipfs/pref_names.h"
#endif

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/adrbrowsiel_wallet_constants.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_adrbrowsiel_WAYBACK_MACHINE)
#include "adrbrowsiel/components/adrbrowsiel_wayback_machine/pref_names.h"
#endif

using adrbrowsielProfilePrefsBrowserTest = InProcessBrowserTest;
using adrbrowsielLocalStatePrefsBrowserTest = InProcessBrowserTest;

// Check download prompt preference is set to true by default.
IN_PROC_BROWSER_TEST_F(adrbrowsielProfilePrefsBrowserTest, DownloadPromptDefault) {
  EXPECT_TRUE(
      browser()->profile()->GetPrefs()->GetBoolean(prefs::kPromptForDownload));
}

IN_PROC_BROWSER_TEST_F(adrbrowsielProfilePrefsBrowserTest, MiscadrbrowsielPrefs) {
  EXPECT_TRUE(browser()->profile()->GetPrefs()->GetBoolean(
      kHTTPSEVerywhereControlType));
  EXPECT_FALSE(
      browser()->profile()->GetPrefs()->GetBoolean(kNoScriptControlType));
  EXPECT_FALSE(browser()->profile()->GetPrefs()->GetBoolean(
      kShieldsAdvancedViewEnabled));
  EXPECT_TRUE(
      browser()->profile()->GetPrefs()->GetBoolean(kShieldsStatsBadgeVisible));
  EXPECT_TRUE(browser()->profile()->GetPrefs()->GetBoolean(kAdControlType));
  EXPECT_TRUE(
      browser()->profile()->GetPrefs()->GetBoolean(kGoogleLoginControlType));
  EXPECT_TRUE(browser()->profile()->GetPrefs()->GetBoolean(
      adrbrowsiel_shields::prefs::kFBEmbedControlType));
  EXPECT_TRUE(browser()->profile()->GetPrefs()->GetBoolean(
      adrbrowsiel_shields::prefs::kTwitterEmbedControlType));
  EXPECT_FALSE(browser()->profile()->GetPrefs()->GetBoolean(
      adrbrowsiel_shields::prefs::kLinkedInEmbedControlType));
  EXPECT_TRUE(browser()->profile()->GetPrefs()->GetBoolean(kWebTorrentEnabled));
#if BUILDFLAG(ENABLE_adrbrowsiel_WAYBACK_MACHINE)
  EXPECT_TRUE(browser()->profile()->GetPrefs()->GetBoolean(
      kadrbrowsielWaybackMachineEnabled));
#endif
  EXPECT_TRUE(browser()->profile()->GetPrefs()->GetBoolean(kHangoutsEnabled));
  EXPECT_FALSE(browser()->profile()->GetPrefs()->GetBoolean(
      adrbrowsiel_rewards::prefs::kHideButton));
#if BUILDFLAG(IPFS_ENABLED)
  EXPECT_EQ(browser()->profile()->GetPrefs()->GetInteger(kIPFSResolveMethod),
            static_cast<int>((ipfs::IPFSResolveMethodTypes::IPFS_ASK)));
  EXPECT_TRUE(
      browser()->profile()->GetPrefs()->GetFilePath(kIPFSBinaryPath).empty());
  EXPECT_FALSE(
      browser()->profile()->GetPrefs()->GetBoolean(kIPFSAutoRedirectGateway));
#endif
  EXPECT_FALSE(
      browser()->profile()->GetPrefs()->GetBoolean(kIPFSCompanionEnabled));
#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
  EXPECT_EQ(
      browser()->profile()->GetPrefs()->GetInteger(kadrbrowsielWalletWeb3Provider),
      static_cast<int>(adrbrowsiel_wallet::IsNativeWalletEnabled()
                           ? adrbrowsielWalletWeb3ProviderTypes::adrbrowsiel_WALLET
                           : adrbrowsielWalletWeb3ProviderTypes::ASK));
  EXPECT_FALSE(browser()->profile()->GetPrefs()->GetBoolean(
      kLoadCryptoWalletsOnStartup));
  EXPECT_FALSE(
      browser()->profile()->GetPrefs()->GetBoolean(kOptedIntoCryptoWallets));
  EXPECT_TRUE(
      browser()->profile()->GetPrefs()->GetBoolean(kShowWalletIconOnToolbar));
#endif
  EXPECT_FALSE(
      browser()->profile()->GetPrefs()->GetBoolean(kMRUCyclingEnabled));
#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
  EXPECT_FALSE(
      browser()->profile()->GetPrefs()->GetBoolean(kadrbrowsielGCMChannelStatus));
#endif
}

IN_PROC_BROWSER_TEST_F(adrbrowsielProfilePrefsBrowserTest,
                       DisableGoogleServicesByDefault) {
  EXPECT_FALSE(browser()->profile()->GetPrefs()->GetBoolean(
      embedder_support::kAlternateErrorPagesEnabled));
  EXPECT_FALSE(browser()->profile()->GetPrefs()->GetBoolean(
      spellcheck::prefs::kSpellCheckUseSpellingService));
  EXPECT_FALSE(browser()->profile()->GetPrefs()->GetBoolean(
      prefs::kSafeBrowsingExtendedReportingOptInAllowed));
  EXPECT_FALSE(browser()->profile()->GetPrefs()->GetBoolean(
      prefs::kSearchSuggestEnabled));
  EXPECT_EQ(browser()->profile()->GetPrefs()->GetInteger(
                prefs::kNetworkPredictionOptions),
            chrome_browser_net::NETWORK_PREDICTION_NEVER);
  EXPECT_FALSE(browser()->profile()->GetPrefs()->GetBoolean(
      prefs::kSigninAllowedOnNextStartup));
  // Verify cloud print is disabled.
  EXPECT_FALSE(browser()->profile()->GetPrefs()->GetBoolean(
      prefs::kCloudPrintProxyEnabled));
  EXPECT_FALSE(browser()->profile()->GetPrefs()->GetBoolean(
      prefs::kCloudPrintSubmitEnabled));
  EXPECT_TRUE(browser()->profile()->GetPrefs()->GetBoolean(
      prefs::kNtpUseMostVisitedTiles));
  EXPECT_TRUE(
      browser()->profile()->GetPrefs()->GetBoolean(prefs::kHideWebStoreIcon));
}

IN_PROC_BROWSER_TEST_F(adrbrowsielLocalStatePrefsBrowserTest, DefaultLocalStateTest) {
  EXPECT_TRUE(g_browser_process->local_state()->GetBoolean(
      kDefaultBrowserPromptEnabled));
}
