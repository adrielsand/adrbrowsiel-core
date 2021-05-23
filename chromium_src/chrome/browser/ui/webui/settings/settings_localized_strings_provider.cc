/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ui/webui/settings/settings_localized_strings_provider.h"

#include "base/stl_util.h"
#include "base/strings/utf_string_conversions.h"
#include "adrbrowsiel/browser/ui/webui/settings/adrbrowsiel_privacy_handler.h"
#include "adrbrowsiel/components/ipfs/ipfs_constants.h"
#include "adrbrowsiel/components/ipfs/pref_names.h"
#include "adrbrowsiel/components/sidebar/buildflags/buildflags.h"
#include "adrbrowsiel/components/version_info/version_info.h"
#include "chrome/browser/ui/webui/webui_util.h"
#include "chrome/common/pref_names.h"
#include "components/grit/adrbrowsiel_components_strings.h"
#include "components/prefs/pref_service.h"

namespace settings {
void adrbrowsielAddLocalizedStrings(content::WebUIDataSource*, Profile*);
}  // namespace settings

// Override some chromium strings
#include "adrbrowsiel/grit/adrbrowsiel_generated_resources.h"
#include "chrome/grit/chromium_strings.h"
#include "chrome/grit/generated_resources.h"
#include "extensions/common/extension_urls.h"

#undef IDS_SETTINGS_CUSTOMIZE_PROFILE
#define IDS_SETTINGS_CUSTOMIZE_PROFILE IDS_SETTINGS_adrbrowsiel_EDIT_PROFILE
#undef IDS_SETTINGS_CUSTOMIZE_YOUR_CHROME_PROFILE
#define IDS_SETTINGS_CUSTOMIZE_YOUR_CHROME_PROFILE \
  IDS_SETTINGS_adrbrowsiel_EDIT_PROFILE
#undef IDS_SETTINGS_SAFEBROWSING_STANDARD_BULLET_TWO
#define IDS_SETTINGS_SAFEBROWSING_STANDARD_BULLET_TWO \
  IDS_SETTINGS_adrbrowsiel_SAFEBROWSING_STANDARD_BULLET_TWO
#undef IDS_SETTINGS_SAFEBROWSING_NONE_DESC
#define IDS_SETTINGS_SAFEBROWSING_NONE_DESC \
  IDS_SETTINGS_adrbrowsiel_SAFEBROWSING_NONE_DESC

#define GetVersionNumber GetadrbrowsielVersionNumberForDisplay

#include "../../../../../../../chrome/browser/ui/webui/settings/settings_localized_strings_provider.cc"
#undef GetVersionNumber

#include "adrbrowsiel/browser/ui/webui/adrbrowsiel_settings_ui.h"
namespace settings {

const char kWebRTCLearnMoreURL[] =
    "https://support.adrbrowsiel.com/hc/en-us/articles/"
    "360017989132-How-do-I-change-my-Privacy-Settings-#webrtc";
const char kadrbrowsielBuildInstructionsUrl[] =
    "https://github.com/adrielsand/adrbrowsiel-browser/wiki";
const char kadrbrowsielLicenseUrl[] = "https://mozilla.org/MPL/2.0/";
const char kadrbrowsielReleaseTagPrefix[] =
    "https://github.com/adrielsand/adrbrowsiel-browser/releases/tag/v";
const char kGoogleLoginLearnMoreURL[] =
    "https://github.com/adrielsand/adrbrowsiel-browser/wiki/"
    "Allow-Google-login---Third-Parties-and-Extensions";
const char kDNSLinkLearnMoreURL[] = "https://docs.ipfs.io/concepts/dnslink/";
const char kUnstoppableDomainsLearnMoreURL[] =
    "https://github.com/adrielsand/adrbrowsiel-browser/wiki/"
    "Resolve-Methods-for-Unstoppable-Domains";

void adrbrowsielAddCommonStrings(content::WebUIDataSource* html_source,
                           Profile* profile) {
  webui::LocalizedString localized_strings[] = {
    {"importExtensions", IDS_SETTINGS_IMPORT_EXTENSIONS_CHECKBOX},
    {"importPayments", IDS_SETTINGS_IMPORT_PAYMENTS_CHECKBOX},
    {"siteSettingsAutoplay", IDS_SETTINGS_SITE_SETTINGS_AUTOPLAY},
    {"siteSettingsCategoryAutoplay", IDS_SETTINGS_SITE_SETTINGS_AUTOPLAY},
    {"adrbrowsielGetStartedTitle", IDS_SETTINGS_adrbrowsiel_GET_STARTED_TITLE},
    {"adrbrowsielAdditionalSettingsTitle", IDS_SETTINGS_adrbrowsiel_ADDITIONAL_SETTINGS},
    {"appearanceSettingsadrbrowsielTheme",
     IDS_SETTINGS_APPEARANCE_SETTINGS_adrbrowsiel_THEMES},
    {"appearanceSettingsLocationBarIsWide",
     IDS_SETTINGS_APPEARANCE_SETTINGS_LOCATION_BAR_IS_WIDE},
    {"appearanceSettingsHideadrbrowsielRewardsButtonLabel",
     IDS_SETTINGS_HIDE_adrbrowsiel_REWARDS_BUTTON_LABEL},
    {"appearanceSettingsAlwaysShowBookmarkBarOnNTP",
     IDS_SETTINGS_ALWAYS_SHOW_BOOKMARK_BAR_ON_NTP},
    {"appearanceSettingsShowAutocompleteInAddressBar",
     IDS_SETTINGS_APPEARANCE_SETTINGS_SHOW_AUTOCOMPLETE_IN_ADDRESS_BAR},
    {"appearanceSettingsUseTopSiteSuggestions",
     IDS_SETTINGS_APPEARANCE_SETTINGS_USE_AUTOCOMPLETE_TOP_SITES},
    {"appearanceSettingsUseadrbrowsielSuggestedSiteSuggestions",
     IDS_SETTINGS_APPEARANCE_SETTINGS_USE_AUTOCOMPLETE_adrbrowsiel_SUGGESTED_SITES},
    {"appearanceSettingsGetMoreThemes",
     IDS_SETTINGS_APPEARANCE_SETTINGS_GET_MORE_THEMES},
    {"appearanceadrbrowsielDefaultImagesOptionLabel",
     IDS_SETTINGS_APPEARANCE_SETTINGS_adrbrowsiel_DEFAULT_IMAGES_OPTION_LABEL},
#if BUILDFLAG(ENABLE_SIDEBAR)
    {"appearanceSettingsShowOptionTitle", IDS_SIDEBAR_SHOW_OPTION_TITLE},
    {"appearanceSettingsShowOptionAlways", IDS_SIDEBAR_SHOW_OPTION_ALWAYS},
    {"appearanceSettingsShowOptionMouseOver",
     IDS_SIDEBAR_SHOW_OPTION_MOUSEOVER},
    {"appearanceSettingsShowOptionOnClick", IDS_SIDEBAR_SHOW_OPTION_ONCLICK},
    {"appearanceSettingsShowOptionNever", IDS_SIDEBAR_SHOW_OPTION_NEVER},
    {"appearanceSettingsSidebarEnabledDesc",
     IDS_SETTINGS_APPEARANCE_SETTINGS_SIDEBAR_ENABLED_DESC},
    {"appearanceSettingsSidebarDisabledDesc",
     IDS_SETTINGS_APPEARANCE_SETTINGS_SIDEBAR_DISABLED_DESC},
#endif
    {"mruCyclingSettingLabel", IDS_SETTINGS_adrbrowsiel_MRU_CYCLING_LABEL},
    {"adrbrowsielShieldsTitle", IDS_SETTINGS_adrbrowsiel_SHIELDS_TITLE},
    {"adrbrowsielShieldsDefaultsSectionTitle",
     IDS_SETTINGS_adrbrowsiel_SHIELDS_DEFAULTS_TITLE},
    {"adrbrowsielShieldsDefaultsSectionDescription1",
     IDS_SETTINGS_adrbrowsiel_SHIELDS_DEFAULTS_DESCRIPTION_1},
    {"adrbrowsielShieldsDefaultsSectionDescription2",
     IDS_SETTINGS_adrbrowsiel_SHIELDS_DEFAULTS_DESCRIPTION_2},
    {"adrbrowsielShieldsDefaultsSectionDescription3",
     IDS_SETTINGS_adrbrowsiel_SHIELDS_DEFAULTS_DESCRIPTION_3},
    {"socialBlocking", IDS_SETTINGS_SOCIAL_BLOCKING_DEFAULTS_TITLE},
    {"defaultView", IDS_SETTINGS_adrbrowsiel_SHIELDS_DEFAULTS_DEFAULT_VIEW_LABEL},
    {"simpleView", IDS_SETTINGS_adrbrowsiel_SHIELDS_DEFAULTS_SIMPLE_VIEW_LABEL},
    {"advancedView", IDS_SETTINGS_adrbrowsiel_SHIELDS_DEFAULTS_ADVANCED_VIEW_LABEL},
    {"adControlLabel", IDS_SETTINGS_adrbrowsiel_SHIELDS_AD_CONTROL_LABEL},
    {"cookieControlLabel", IDS_SETTINGS_adrbrowsiel_SHIELDS_COOKIE_CONTROL_LABEL},
    {"fingerprintingControlLabel",
     IDS_SETTINGS_adrbrowsiel_SHIELDS_FINGERPRINTING_CONTROL_LABEL},
    {"httpsEverywhereControlLabel",
     IDS_SETTINGS_adrbrowsiel_SHIELDS_HTTPS_EVERYWHERE_CONTROL_LABEL},
    {"noScriptControlLabel",
     IDS_SETTINGS_adrbrowsiel_SHIELDS_NO_SCRIPT_CONTROL_LABEL},
    {"shieldsLookFeelTitle", IDS_SETTINGS_adrbrowsiel_SHIELDS_LOOK_AND_FEEL_TITLE},
    {"showStatsBlockedBadgeLabel",
     IDS_SETTINGS_adrbrowsiel_SHIELDS_SHOW_STATS_BLOCKED_BADGE_LABEL},
    {"googleLoginControlLabel", IDS_SETTINGS_adrbrowsiel_SHIELDS_GOOGLE_LOGIN_LABEL},
    {"fbEmbedControlLabel",
     IDS_SETTINGS_adrbrowsiel_SHIELDS_FACEBOOK_EMBEDDED_POSTS_LABEL},
    {"twitterEmbedControlLabel",
     IDS_SETTINGS_adrbrowsiel_SHIELDS_TWITTER_EMBEDDED_TWEETS_LABEL},
    {"linkedInEmbedControlLabel",
     IDS_SETTINGS_adrbrowsiel_SHIELDS_LINKEDIN_EMBEDDED_POSTS_LABEL},
    {"blockAdsTrackersAggressive", IDS_SETTINGS_BLOCK_ADS_TRACKERS_AGGRESSIVE},
    {"blockAdsTrackersStandard", IDS_SETTINGS_BLOCK_ADS_TRACKERS_STANDARD},
    {"allowAdsTrackers", IDS_SETTINGS_ALLOW_ADS_TRACKERS},
    {"block3rdPartyCookies", IDS_SETTINGS_BLOCK_3RD_PARTY_COOKIES},
    {"allowAllCookies", IDS_SETTINGS_ALLOW_ALL_COOKIES},
    {"blockAllCookies", IDS_SETTINGS_BLOCK_ALL_COOKIES},
    {"standardFingerprinting", IDS_SETTINGS_STANDARD_FINGERPRINTING},
    {"allowAllFingerprinting", IDS_SETTINGS_ALLOW_ALL_FINGERPRINTING},
    {"strictFingerprinting", IDS_SETTINGS_STRICT_FINGERPRINTING},
    {"webRTCPolicyLabel", IDS_SETTINGS_WEBRTC_POLICY_LABEL},
    {"webRTCPolicySubLabel", IDS_SETTINGS_WEBRTC_POLICY_SUB_LABEL},
    {"webRTCDefault", IDS_SETTINGS_WEBRTC_POLICY_DEFAULT},
    {"pushMessagingLabel", IDS_SETTINGS_PUSH_MESSAGING},
    {"defaultPublicAndPrivateInterfaces",
     IDS_SETTINGS_WEBRTC_POLICY_DEFAULT_PUBLIC_AND_PRIVATE_INTERFACES},
    {"defaultPublicInterfaceOnly",
     IDS_SETTINGS_WEBRTC_POLICY_DEFAULT_PUBLIC_INTERFACE_ONLY},
    {"disableNonProxiedUdp",
     IDS_SETTINGS_WEBRTC_POLICY_DISABLE_NON_PROXIED_UDP},
    {"adrbrowsielSync", IDS_SETTINGS_adrbrowsiel_SYNC_TITLE},
    {"adrbrowsielSyncSetupActionLabel", IDS_SETTINGS_adrbrowsiel_SYNC_SETUP_ACTION_LABEL},
    {"adrbrowsielSyncSetupTitle", IDS_SETTINGS_adrbrowsiel_SYNC_SETUP_TITLE},
    {"adrbrowsielSyncSetupSubtitle", IDS_SETTINGS_adrbrowsiel_SYNC_SETUP_SUBTITLE},
    {"adrbrowsielSyncManageActionLabel", IDS_SETTINGS_adrbrowsiel_SYNC_MANAGE_ACTION_LABEL},
    {"adrbrowsielSyncWordCount", IDS_SETTINGS_adrbrowsiel_SYNC_WORD_COUNT},
    {"adrbrowsielSyncCopied", IDS_SETTINGS_adrbrowsiel_SYNC_COPIED_TEXT},
    {"adrbrowsielSyncQRCodeAlt", IDS_SETTINGS_adrbrowsiel_SYNC_QR_IMAGE_ALT},
    {"adrbrowsielSyncQRError", IDS_SETTINGS_adrbrowsiel_SYNC_QR_ERROR},
    {"adrbrowsielSyncManagerTitle", IDS_SETTINGS_adrbrowsiel_SYNC_MANAGER_TITLE},
    {"adrbrowsielSyncSettingsTitle", IDS_SETTINGS_adrbrowsiel_SYNC_SETTINGS_TITLE},
    {"adrbrowsielSyncSettingsSubtitle", IDS_SETTINGS_adrbrowsiel_SYNC_SETTINGS_SUBTITLE},
    {"adrbrowsielSyncDeviceListTitle", IDS_SETTINGS_adrbrowsiel_SYNC_DEVICE_LIST_TITLE},
    {"adrbrowsielSyncDeviceListSubtitle",
     IDS_SETTINGS_adrbrowsiel_SYNC_DEVICE_LIST_SUBTITLE},
    {"adrbrowsielSyncDeviceListNameColumn",
     IDS_SETTINGS_adrbrowsiel_SYNC_DEVICE_LIST_NAME_COLUMN},
    {"adrbrowsielSyncDeviceListNameThisDevice",
     IDS_SETTINGS_adrbrowsiel_SYNC_DEVICE_LIST_NAME_THIS_DEVICE},
    {"adrbrowsielSyncDeviceListLastActiveColumn",
     IDS_SETTINGS_adrbrowsiel_SYNC_DEVICE_LIST_LAST_ACTIVE_COLUMN},
    {"adrbrowsielSyncDeviceListRemoveColumn",
     IDS_SETTINGS_adrbrowsiel_SYNC_DEVICE_LIST_REMOVE_COLUMN},
    {"adrbrowsielSyncSetupTitle", IDS_adrbrowsiel_SYNC_SETUP_TITLE},
    {"adrbrowsielSyncSetupDesc", IDS_adrbrowsiel_SYNC_SETUP_DESCRIPTION},
    {"adrbrowsielSyncStartNewChain", IDS_adrbrowsiel_SYNC_START_NEW_CHAIN_BUTTON},
    {"adrbrowsielSyncEnterCode", IDS_adrbrowsiel_SYNC_ENTER_CODE_BUTTON},
    {"adrbrowsielSyncChooseDeviceMobileTitle",
     IDS_adrbrowsiel_SYNC_CHOOSE_DEVICE_MOBILE_TITLE},
    {"adrbrowsielSyncChooseDeviceComputerTitle",
     IDS_adrbrowsiel_SYNC_CHOOSE_DEVICE_COMPUTER_TITLE},
    {"adrbrowsielSyncScanCodeTitle", IDS_adrbrowsiel_SYNC_SCAN_CODE_TITLE},
    {"adrbrowsielSyncScanCodeDesc1", IDS_adrbrowsiel_SYNC_SCAN_CODE_DESCRIPTION_PARTIAL_1},
    {"adrbrowsielSyncScanCodeDesc2", IDS_adrbrowsiel_SYNC_SCAN_CODE_DESCRIPTION_PARTIAL_2},
    {"adrbrowsielSyncScanCodeDesc3", IDS_adrbrowsiel_SYNC_SCAN_CODE_DESCRIPTION_PARTIAL_3},
    {"adrbrowsielSyncViewCodeTitle", IDS_adrbrowsiel_SYNC_VIEW_CODE_TITLE},
    {"adrbrowsielSyncViewCodeDesc1", IDS_adrbrowsiel_SYNC_VIEW_CODE_DESCRIPTION_PARTIAL_1},
    {"adrbrowsielSyncViewCodeDesc2", IDS_adrbrowsiel_SYNC_VIEW_CODE_DESCRIPTION_PARTIAL_2},
    {"adrbrowsielSyncViewCodeDesc3", IDS_adrbrowsiel_SYNC_VIEW_CODE_DESCRIPTION_PARTIAL_3},
    {"adrbrowsielSyncCodeWarning", IDS_adrbrowsiel_SYNC_CODE_WARNING},
    {"adrbrowsielSyncViewCodeQRCodeButton", IDS_adrbrowsiel_SYNC_VIEW_CODE_QR_CODE_BUTTON},
    {"adrbrowsielSyncEnterCodeTitle", IDS_adrbrowsiel_SYNC_ENTER_CODE_TITLE},
    {"adrbrowsielSyncEnterCodeDesc", IDS_adrbrowsiel_SYNC_ENTER_CODE_DESCRIPTION},
    {"adrbrowsielSyncViewCodeButton", IDS_adrbrowsiel_SYNC_VIEW_CODE_BUTTON},
    {"adrbrowsielSyncAddDevice", IDS_adrbrowsiel_SYNC_ADD_DEVICE_BUTTON},
    {"adrbrowsielSyncChooseDeviceTitle", IDS_adrbrowsiel_SYNC_CHOOSE_DEVICE_TITLE},
    {"adrbrowsielSyncChooseDeviceDesc", IDS_adrbrowsiel_SYNC_CHOOSE_DEVICE_DESCRIPTION},
    {"adrbrowsielSyncInvalidSyncCodeTitle", IDS_adrbrowsiel_SYNC_INVALID_SYNC_CODE_TITLE},
    {"adrbrowsielSyncResetButton", IDS_adrbrowsiel_SYNC_RESET_BUTTON},
    {"adrbrowsielSyncResetConfirmation", IDS_adrbrowsiel_SYNC_RESET_CONFIRMATION},
    {"adrbrowsielSyncDeleteDeviceConfirmation",
     IDS_adrbrowsiel_SYNC_DELETE_DEVICE_CONFIRMATION},
    {"adrbrowsielIPFS", IDS_adrbrowsiel_IPFS_SETTINGS_SECTION},
    {"adrbrowsielWallet", IDS_adrbrowsiel_WALLET_SETTINGS_SECTION},
    {"adrbrowsielHelpTips", IDS_SETTINGS_HELP_TIPS},
    {"adrbrowsielHelpTipsWaybackMachineLabel",
     IDS_SETTINGS_HELP_TIPS_SHOW_adrbrowsiel_WAYBACK_MACHINE_PROMPT},
    // New Tab Page
    {"adrbrowsielNewTab", IDS_SETTINGS_NEW_TAB},
    {"adrbrowsielNewTabadrbrowsielRewards", IDS_SETTINGS_NEW_TAB_adrbrowsiel_REWARDS},
    {"adrbrowsielNewTabNewTabPageShows", IDS_SETTINGS_NEW_TAB_NEW_TAB_PAGE_SHOWS},
    {"adrbrowsielNewTabNewTabCustomizeWidgets",
     IDS_SETTINGS_NEW_TAB_NEW_TAB_CUSTOMIZE_WIDGETS},
    // Misc (TODO: Organize this)
    {"onExitPageTitle", IDS_SETTINGS_adrbrowsiel_ON_EXIT},
    {"adrbrowsielDefaultExtensions", IDS_SETTINGS_adrbrowsiel_DEFAULT_EXTENSIONS_TITLE},
    {"webTorrentEnabledDesc", IDS_SETTINGS_WEBTORRENT_ENABLED_DESC},
    {"adrbrowsielWeb3ProviderDesc", IDS_SETTINGS_adrbrowsiel_WEB3_PROVIDER_DESC},
    {"loadCryptoWalletsOnStartupDesc",
     IDS_SETTINGS_LOAD_CRYPTO_WALLETS_ON_STARTUP},
    {"loadCryptoWalletsOnStartupDescDeprecated",
     IDS_SETTINGS_LOAD_CRYPTO_WALLETS_ON_STARTUP_DEPRECATED},
    {"showadrbrowsielwalletIconOnToolbar",
     IDS_SETTINGS_SHOW_adrbrowsiel_WALLET_ICON_ON_TOOLBAR},
    {"googleLoginForExtensionsDesc", IDS_SETTINGS_GOOGLE_LOGIN_FOR_EXTENSIONS},
    {"hangoutsEnabledDesc", IDS_SETTINGS_HANGOUTS_ENABLED_DESC},
    {"resolveUnstoppableDomainsDesc",
     IDS_SETTINGS_RESOLVE_UNSTOPPABLE_DOMAINS_DESC},
    {"resolveENSDesc", IDS_SETTINGS_RESOLVE_ENS_DESC},
    {"resolveIPFSURLDesc", IDS_SETTINGS_RESOLVE_IPFS_URLS_DESC},
    {"ipfsPublicGatewayDesc", IDS_SETTINGS_IPFS_PUBLIC_GATEWAY_DESC},
    {"ipfsChangeGatewayButtonLabel",
     IDS_SETTINGS_IPFS_CHANGE_GATEWAY_BUTTON_LABEL},
    {"changeIpfsGatewayDialogTitle",
     IDS_SETTINGS_CHANGE_IPFS_GATEWAY_DIALOG_TITLE},
    {"changeIpfsGatewayDialogLabel",
     IDS_SETTINGS_CHANGE_IPFS_GATEWAY_DIALOG_LABEL},
    {"changeIpfsStorageMaxLabel", IDS_SETTINGS_CHANGE_IPFS_STORAGE_MAX_LABEL},
    {"changeIpfsStorageMaxDesc", IDS_SETTINGS_CHANGE_IPFS_STORAGE_MAX_DESC},
    {"ipfsErrorInvalidAddress", IDS_SETTINGS_IPFS_ERROR_INVALID_ADDRESS},
    {"ipfsAutoFallbackToGatewayLabel",
     IDS_SETTINGS_IPFS_AUTO_FALLBACK_TO_GATEWAY_LABEL},
    {"ipfsAutoFallbackToGatewayDesc",
     IDS_SETTINGS_IPFS_AUTO_FALLBACK_TO_GATEWAY_DESC},
    {"ipfsAutoRedirectGatewayLabel",
     IDS_SETTINGS_IPFS_AUTO_REDIRECT_GATEWAY_LABEL},
    {"ipfsAutoRedirectGatewayDesc",
     IDS_SETTINGS_IPFS_AUTO_REDIRECT_GATEWAY_DESC},
    {"ipfsAutoRedirectDNSLinkLabel",
     IDS_SETTINGS_IPFS_AUTO_REDIRECT_DNSLINK_RESOURCES_LABEL},
    {"ipfsAutoRedirectDNSLinkDesc",
     IDS_SETTINGS_IPFS_AUTO_REDIRECT_DNSLINK_RESOURCES_DESC},
    {"ipfsCompanionEnabledDesc", IDS_SETTINGS_IPFS_COMPANION_ENABLED_DESC},
    {"mediaRouterEnabledDesc", IDS_SETTINGS_MEDIA_ROUTER_ENABLED_DESC},
    {"torEnabledLabel", IDS_SETTINGS_ENABLE_TOR_TITLE},
    {"torEnabledDesc", IDS_SETTINGS_ENABLE_TOR_DESC},
    {"autoOnionLocationLabel", IDS_SETTINGS_AUTO_ONION_LOCATION_TITLE},
    {"autoOnionLocationDesc", IDS_SETTINGS_AUTO_ONION_LOCATION_DESC},
    {"widevineEnabledDesc", IDS_SETTINGS_ENABLE_WIDEVINE_DESC},
    {"restartNotice", IDS_SETTINGS_RESTART_NOTICE},
    {"relaunchButtonLabel", IDS_SETTINGS_RELAUNCH_BUTTON_LABEL},
    {"manageExtensionsLabel", IDS_SETTINGS_MANAGE_EXTENSIONS_LABEL},
    {"keyboardShortcuts", IDS_EXTENSIONS_SIDEBAR_KEYBOARD_SHORTCUTS},
    {"getMoreExtensionsLabel", IDS_adrbrowsiel_SETTINGS_GET_MORE_EXTENSIONS_LABEL},
    {"getMoreExtensionsSubLabel", IDS_EXTENSIONS_SIDEBAR_OPEN_CHROME_WEB_STORE},
    {"p3aEnableTitle", IDS_adrbrowsiel_P3A_ENABLE_SETTING},
    {"p3aEnabledDesc", IDS_adrbrowsiel_P3A_ENABLE_SETTING_SUBITEM},
    {"siteSettings", IDS_SETTINGS_SITE_AND_SHIELDS_SETTINGS},
    {"resetRewardsData", IDS_SETTINGS_RESET_REWARDS_DATA},
    {"showFullUrls", IDS_CONTEXT_MENU_SHOW_FULL_URLS},
    {"ipfsIpnsKeysLinkTitle", IDS_SETTINGS_IPNS_KEYS_EDITOR_LINK},
    {"ipfsIpnsKeysLinkTitleDesc", IDS_SETTINGS_IPNS_KEYS_EDITOR_LINK_DESC},
    {"ipfsKeysListTitle", IDS_SETTINGS_IPNS_KEYS_LIST_TITLE},
    {"ipfsAddKeyDialogTitle", IDS_SETTINGS_IPNS_ADD_KEY_DIALOG_TITLE},
    {"ipfsAddKeyDialogError", IDS_SETTINGS_IPNS_ADD_KEY_DIALOG_ERROR},
    {"ipfsDeleteKeyConfirmation", IDS_SETTINGS_IPNS_DELETE_KEY_CONFIRMATION},
    {"ipfsKeysNodeNotLaunched", IDS_SETTINGS_IPNS_KEYS_NODE_NOT_LAUNCHED},
    {"ipfsKeysStartNode", IDS_SETTINGS_IPNS_KEYS_START_NODE},
    {"ipfsKeysNodeLaunchError", IDS_SETTINGS_IPNS_KEYS_START_NODE_ERROR},
    {"ipfsKeyImport", IDS_SETTINGS_IPNS_KEYS_IMPORT_BUTTON_TITLE},
    {"ipfsKeyGenerate", IDS_SETTINGS_IPNS_KEYS_GENERATE_BUTTON_TITLE},
    {"ipfsImporKeysError", IDS_SETTINGS_IPNS_KEYS_IMPORT_ERROR},
  };
  html_source->AddLocalizedStrings(localized_strings);
  html_source->AddString("webRTCLearnMoreURL",
                         base::ASCIIToUTF16(kWebRTCLearnMoreURL));
  html_source->AddString("googleLoginLearnMoreURL",
                         base::ASCIIToUTF16(kGoogleLoginLearnMoreURL));
  html_source->AddString("ipfsDNSLinkLearnMoreURL",
                         base::UTF8ToUTF16(kDNSLinkLearnMoreURL));
  html_source->AddString(
      "getMoreExtensionsUrl",
      base::ASCIIToUTF16(
          google_util::AppendGoogleLocaleParam(
              GURL(extension_urls::GetWebstoreExtensionsCategoryURL()),
              g_browser_process->GetApplicationLocale())
              .spec()));
  html_source->AddString(
      "ipfsStorageMaxValue",
      std::to_string(profile->GetPrefs()->GetInteger(kIpfsStorageMax)));

  std::u16string ipfs_method_desc = l10n_util::GetStringFUTF16(
      IDS_SETTINGS_IPFS_METHOD_DESC,
      base::ASCIIToUTF16(ipfs::kIPFSLearnMorePrivacyURL));
  html_source->AddString("ipfsMethodDesc", ipfs_method_desc);

  html_source->AddString(
      "resolveUnstoppableDomainsSubDesc",
      l10n_util::GetStringFUTF16(
          IDS_SETTINGS_RESOLVE_UNSTOPPABLE_DOMAINS_SUB_DESC,
          base::ASCIIToUTF16(kUnstoppableDomainsLearnMoreURL)));
}

void adrbrowsielAddResources(content::WebUIDataSource* html_source,
                       Profile* profile) {
  adrbrowsielSettingsUI::AddResources(html_source, profile);
}

void adrbrowsielAddAboutStrings(content::WebUIDataSource* html_source,
                          Profile* profile) {
  std::u16string license = l10n_util::GetStringFUTF16(
      IDS_adrbrowsiel_VERSION_UI_LICENSE, base::ASCIIToUTF16(kadrbrowsielLicenseUrl),
      base::ASCIIToUTF16(chrome::kChromeUICreditsURL),
      base::ASCIIToUTF16(kadrbrowsielBuildInstructionsUrl),
      base::ASCIIToUTF16(kadrbrowsielReleaseTagPrefix) +
          base::UTF8ToUTF16(
              version_info::GetadrbrowsielVersionWithoutChromiumMajorVersion()));
  html_source->AddString("aboutProductLicense", license);
}

void adrbrowsielAddSocialBlockingLoadTimeData(content::WebUIDataSource* html_source,
                                        Profile* profile) {
  html_source->AddBoolean(
      "signInAllowedOnNextStartupInitialValue",
      profile->GetPrefs()->GetBoolean(prefs::kSigninAllowedOnNextStartup));
}

void adrbrowsielAddLocalizedStrings(content::WebUIDataSource* html_source,
                              Profile* profile) {
  adrbrowsielAddCommonStrings(html_source, profile);
  adrbrowsielAddResources(html_source, profile);
  adrbrowsielAddAboutStrings(html_source, profile);
  adrbrowsielPrivacyHandler::AddLoadTimeData(html_source, profile);
  adrbrowsielAddSocialBlockingLoadTimeData(html_source, profile);
}

}  // namespace settings
