/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "../../../../chrome/browser/flag_descriptions.cc"

namespace flag_descriptions {
const char kUseDevUpdaterUrlName[] =
    "Use dev updater url";
const char kUseDevUpdaterUrlDescription[] =
    "Use the dev url for the component updater. "
    "This is for internal testing only.";
const char kadrbrowsielNTPBrandedWallpaperName[] =
    "New Tab Page Branded Wallpapers";
const char kadrbrowsielNTPBrandedWallpaperDescription[] =
    "Allow New Tab Page Branded Wallpapers and user preference.";
const char kadrbrowsielNTPBrandedWallpaperDemoName[] =
    "New Tab Page Demo Branded Wallpaper";
const char kadrbrowsielNTPBrandedWallpaperDemoDescription[] =
    "Force dummy data for the Branded Wallpaper New Tab Page Experience. "
    "View rate and user opt-in conditionals will still be followed to decide "
    "when to display the Branded Wallpaper.";
const char kadrbrowsielAdblockCosmeticFilteringName[] = "Enable cosmetic filtering";
const char kadrbrowsielAdblockCosmeticFilteringDescription[] =
    "Enable support for cosmetic filtering";
const char kadrbrowsielAdblockCosmeticFilteringNativeName[] =
    "Use native implementation for cosmetic filtering";
const char kadrbrowsielAdblockCosmeticFilteringNativeDescription[] =
    "Uses native implementation for cosmetic filtering instead of extension";
const char kadrbrowsielAdblockCspRulesName[] = "Enable support for CSP rules";
const char kadrbrowsielAdblockCspRulesDescription[] =
    "Applies additional CSP rules to pages for which a $csp rule has been "
    "loaded from a filter list";
const char kadrbrowsielDomainBlockName[] = "Enable domain blocking";
const char kadrbrowsielDomainBlockDescription[] =
    "Enable support for blocking domains with an interstitial page";
const char kadrbrowsielExtensionNetworkBlockingName[] =
    "Enable extension network blocking";
const char kadrbrowsielExtensionNetworkBlockingDescription[] =
    "Enable blocking for network requests initiated by extensions";
const char kadrbrowsielSidebarName[] = "Enable Sidebar";
// TODO(simon): Use more better description.
const char kadrbrowsielSidebarDescription[] = "Enable Sidebar";
const char kadrbrowsielSpeedreaderName[] = "Enable SpeedReader";
const char kadrbrowsielSpeedreaderDescription[] =
    "Enables faster loading of simplified article-style web pages.";
const char kadrbrowsielSpeedreaderLegacyName[] =
    "Enable legacy adblock based backend for SpeedReader";
const char kadrbrowsielSpeedreaderLegacyDescription[] =
    "Enables the legacy backend for SpeedReader. Uses adblock rules to "
    "determine if pages are readable and distills using CSS selector rules.";
const char kadrbrowsielSyncName[] = "Enable adrbrowsiel Sync v2";
const char kadrbrowsielSyncDescription[] =
    "adrbrowsiel Sync v2 integrates with chromium sync engine with adrbrowsiel specific "
    "authentication flow and enforce client side encryption";
const char kadrbrowsielIpfsName[] = "Enable IPFS";
const char kadrbrowsielIpfsDescription[] =
    "Enable native support of IPFS.";
const char kadrbrowsielRewardsVerboseLoggingName[] =
    "Enable adrbrowsiel Rewards verbose logging";
const char kadrbrowsielRewardsVerboseLoggingDescription[] =
    "Enables detailed logging of adrbrowsiel Rewards system events to a log file "
    "stored on your device. Please note that this log file could include "
    "information such as browsing history and credentials such as passwords "
    "and access tokens depending on your activity. Please do not share it "
    "unless asked to by adrbrowsiel staff.";
const char kadrbrowsielRewardsBitflyerName[] = "Enable bitFlyer for adrbrowsiel Rewards";
const char kadrbrowsielRewardsBitflyerDescription[] =
    "Enables support for bitFlyer as an external wallet provider for adrbrowsiel "
    "Rewards users in Japan";
const char kNativeadrbrowsielWalletName[] = "Enable experimental adrbrowsiel native wallet";
const char kNativeadrbrowsielWalletDescription[] =
    "Experimental native cryptocurrency wallet support without the use of "
    "extensions";
const char kadrbrowsielDecentralizedDnsName[] = "Enable decentralized DNS";
const char kadrbrowsielDecentralizedDnsDescription[] =
    "Enable decentralized DNS support, such as Unstoppable Domains and "
    "Ethereum Name Service (ENS).";
const char kadrbrowsielSuperReferralName[] = "Enable adrbrowsiel Super Referral";
const char kadrbrowsielSuperReferralDescription[] =
    "Use custom theme for adrbrowsiel Super Referral";
const char kadrbrowsielEphemeralStorageName[] = "Enable Ephemeral Storage";
const char kadrbrowsielEphemeralStorageDescription[] =
    "Use ephemeral storage for third-party frames";
const char kadrbrowsielEphemeralStorageKeepAliveName[] =
    "Ephemeral Storage Keep Alive";
const char kadrbrowsielEphemeralStorageKeepAliveDescription[] =
    "Keep ephemeral storage partitions alive for a specified time after all "
    "tabs for that origin are closed";
const char kadrbrowsielPermissionLifetimeName[] = "Permission Lifetime";
const char kadrbrowsielPermissionLifetimeDescription[] =
    "Enables the option to choose a time period after which a permission will "
    "be automatically revoked";
}  // namespace flag_descriptions
