/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/common/pref_names.h"

const char kAdsBlocked[] = "adrbrowsiel.stats.ads_blocked";
// We no longer update this pref, but we keep it around for now because it's
// added to kAdsBlocked when being displayed.
const char kTrackersBlocked[] = "adrbrowsiel.stats.trackers_blocked";
const char kJavascriptBlocked[] = "adrbrowsiel.stats.javascript_blocked";
const char kHttpsUpgrades[] = "adrbrowsiel.stats.https_upgrades";
const char kFingerprintingBlocked[] = "adrbrowsiel.stats.fingerprinting_blocked";
const char kLastCheckYMD[] = "adrbrowsiel.stats.last_check_ymd";
const char kLastCheckWOY[] = "adrbrowsiel.stats.last_check_woy";
const char kLastCheckMonth[] = "adrbrowsiel.stats.last_check_month";
const char kFirstCheckMade[] = "adrbrowsiel.stats.first_check_made";
// Set to true if the user met the threshold requirements and successfully
// sent a ping to the stats-updater server.
const char kThresholdCheckMade[] = "adrbrowsiel.stats.threshold_check_made";
// Serialized query for to send to the stats-updater server. Needs to be saved
// in the case that the user sends the standard usage ping, stops the browser,
// meets the threshold requirements, and then starts the browser before the
// threshold ping was sent.
const char kThresholdQuery[] = "adrbrowsiel.stats.threshold_query";
const char kWeekOfInstallation[] = "adrbrowsiel.stats.week_of_installation";
const char kWidevineOptedIn[] = "adrbrowsiel.widevine_opted_in";
const char kAskWidevineInstall[] = "adrbrowsiel.ask_widevine_install";
const char kUseAlternativeSearchEngineProvider[] =
    "adrbrowsiel.use_alternate_private_search_engine";
const char kAlternativeSearchEngineProviderInTor[] =
    "adrbrowsiel.alternate_private_search_engine_in_tor";
const char kLocationBarIsWide[] = "adrbrowsiel.location_bar_is_wide";
const char kReferralDownloadID[] = "adrbrowsiel.referral.download_id";
const char kReferralTimestamp[] = "adrbrowsiel.referral.timestamp";
const char kReferralAttemptTimestamp[] =
    "adrbrowsiel.referral.referral_attempt_timestamp";
const char kReferralAttemptCount[] = "adrbrowsiel.referral.referral_attempt_count";
const char kReferralHeaders[] = "adrbrowsiel.referral.headers";
const char kReferralAndroidFirstRunTimestamp[] =
    "adrbrowsiel.referral_android_first_run_timestamp";
const char kHTTPSEVerywhereControlType[] = "adrbrowsiel.https_everywhere_default";
const char kNoScriptControlType[] = "adrbrowsiel.no_script_default";
const char kShieldsAdvancedViewEnabled[] =
    "adrbrowsiel.shields.advanced_view_enabled";
const char kShieldsStatsBadgeVisible[] =
    "adrbrowsiel.shields.stats_badge_visible";
const char kAdControlType[] = "adrbrowsiel.ad_default";
const char kGoogleLoginControlType[] = "adrbrowsiel.google_login_default";
const char kWebTorrentEnabled[] = "adrbrowsiel.webtorrent_enabled";
const char kHangoutsEnabled[] = "adrbrowsiel.hangouts_enabled";
const char kIPFSCompanionEnabled[] = "adrbrowsiel.ipfs_companion_enabled";
const char kNewTabPageShowClock[] = "adrbrowsiel.new_tab_page.show_clock";
const char kNewTabPageClockFormat[] = "adrbrowsiel.new_tab_page.clock_format";
const char kNewTabPageShowStats[] = "adrbrowsiel.new_tab_page.show_stats";
const char kNewTabPageShowToday[] = "adrbrowsiel.new_tab_page.show_adrbrowsiel_today";
const char kNewTabPageShowRewards[] = "adrbrowsiel.new_tab_page.show_rewards";
const char kNewTabPageShowBinance[] = "adrbrowsiel.new_tab_page.show_binance";
const char kNewTabPageShowGemini[] = "adrbrowsiel.new_tab_page.show_gemini";
const char kNewTabPageShowTogether[] = "adrbrowsiel.new_tab_page.show_together";
const char kNewTabPageShowsOptions[] = "adrbrowsiel.new_tab_page.shows_options";
const char kadrbrowsielTodaySources[] = "adrbrowsiel.today.sources";
const char kadrbrowsielTodayIntroDismissed[] = "adrbrowsiel.today.intro_dismissed";
const char kadrbrowsielTodayOptedIn[] = "adrbrowsiel.today.opted_in";
const char kadrbrowsielTodayWeeklySessionCount[] =
    "adrbrowsiel.today.p3a_weekly_session_count";
const char kadrbrowsielTodayWeeklyCardViewsCount[] =
    "adrbrowsiel.today.p3a_weekly_card_views_count";
const char kadrbrowsielTodayWeeklyCardVisitsCount[] =
    "adrbrowsiel.today.p3a_weekly_card_visits_count";
const char kadrbrowsielEnabledMediaRouter[] = "adrbrowsiel.enable_media_router";
const char kBinanceAccessToken[] = "adrbrowsiel.binance.access_token";
const char kBinanceRefreshToken[] = "adrbrowsiel.binance.refresh_token";
const char kAlwaysShowBookmarkBarOnNTP[] =
    "adrbrowsiel.always_show_bookmark_bar_on_ntp";
const char kAutocompleteEnabled[] = "adrbrowsiel.autocomplete_enabled";
const char kTopSiteSuggestionsEnabled[] = "adrbrowsiel.top_site_suggestions_enabled";
const char kadrbrowsielSuggestedSiteSuggestionsEnabled[] =
    "adrbrowsiel.adrbrowsiel_suggested_site_suggestions_enabled";
const char kadrbrowsielDarkMode[] = "adrbrowsiel.dark_mode";
const char kOtherBookmarksMigrated[] = "adrbrowsiel.other_bookmarks_migrated";
const char kadrbrowsielShieldsSettingsVersion[] = "adrbrowsiel.shields_settings_version";
const char kDefaultBrowserPromptEnabled[] =
    "adrbrowsiel.default_browser_prompt_enabled";
#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
const char kadrbrowsielGCMChannelStatus[] = "adrbrowsiel.gcm.channel_status";
#endif
const char kImportDialogExtensions[] = "import_dialog_extensions";
const char kImportDialogPayments[] = "import_dialog_payments";
const char kMRUCyclingEnabled[] = "adrbrowsiel.mru_cycling_enabled";

#if defined(OS_ANDROID)
const char kDesktopModeEnabled[] = "adrbrowsiel.desktop_mode_enabled";
const char kPlayYTVideoInBrowserEnabled[] =
    "adrbrowsiel.play_yt_video_in_browser_enabled";
const char kBackgroundVideoPlaybackEnabled[] =
    "adrbrowsiel.background_video_playback";
const char kSafetynetCheckFailed[] = "safetynetcheck.failed";
const char kSafetynetStatus[] = "safetynet.status";
#endif

const char kDefaultBrowserLaunchingCount[] =
    "adrbrowsiel.default_browser.launching_count";
const char kadrbrowsielDefaultSearchVersion[] = "adrbrowsiel.search.default_version";

// deprecated
const char kadrbrowsielThemeType[] = "adrbrowsiel.theme.type";
const char kUseOverriddenadrbrowsielThemeType[] =
    "adrbrowsiel.theme.use_overridden_adrbrowsiel_theme_type";
const char kNewTabPageShowTopSites[] = "adrbrowsiel.new_tab_page.show_top_sites";
