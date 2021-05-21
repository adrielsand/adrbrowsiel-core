/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/adrbrowsiel_local_state_prefs.h"

#include "base/values.h"
#include "adrbrowsiel/browser/adrbrowsiel_stats/adrbrowsiel_stats_updater.h"
#include "adrbrowsiel/browser/metrics/metrics_reporting_util.h"
#include "adrbrowsiel/browser/themes/adrbrowsiel_dark_mode_utils.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_referrals/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/ad_block_service.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/adrbrowsiel_shields_p3a.h"
#include "adrbrowsiel/components/decentralized_dns/buildflags/buildflags.h"
#include "adrbrowsiel/components/ntp_background_images/browser/ntp_background_images_service.h"
#include "adrbrowsiel/components/ntp_background_images/browser/view_counter_service.h"
#include "adrbrowsiel/components/p3a/adrbrowsiel_p3a_service.h"
#include "adrbrowsiel/components/p3a/buildflags.h"
#include "adrbrowsiel/components/tor/buildflags/buildflags.h"
#include "chrome/common/pref_names.h"
#include "components/metrics/metrics_pref_names.h"
#include "components/prefs/pref_registry_simple.h"
#include "third_party/widevine/cdm/buildflags.h"

#if BUILDFLAG(ENABLE_adrbrowsiel_REFERRALS)
#include "adrbrowsiel/components/adrbrowsiel_referrals/browser/adrbrowsiel_referrals_service.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "adrbrowsiel/components/tor/tor_profile_service.h"
#endif

#include "adrbrowsiel/browser/ui/webui/new_tab_page/adrbrowsiel_new_tab_message_handler.h"

#if !defined(OS_ANDROID)
#include "adrbrowsiel/browser/p3a/p3a_core_metrics.h"
#include "chrome/browser/first_run/first_run.h"
#endif  // !defined(OS_ANDROID)

#if BUILDFLAG(ENABLE_WIDEVINE)
#include "adrbrowsiel/browser/widevine/widevine_utils.h"
#endif

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
#include "adrbrowsiel/components/decentralized_dns/decentralized_dns_service.h"
#endif

namespace adrbrowsiel {

void RegisterLocalStatePrefsForMigration(PrefRegistrySimple* registry) {
#if BUILDFLAG(ENABLE_WIDEVINE)
  RegisterWidevineLocalstatePrefsForMigration(registry);
#endif
}

void RegisterLocalStatePrefs(PrefRegistrySimple* registry) {
  adrbrowsiel_shields::RegisterPrefsForAdBlockService(registry);
  adrbrowsiel_stats::RegisterLocalStatePrefs(registry);
  ntp_background_images::NTPBackgroundImagesService::RegisterLocalStatePrefs(
      registry);
  ntp_background_images::ViewCounterService::RegisterLocalStatePrefs(registry);
#if BUILDFLAG(ENABLE_adrbrowsiel_REFERRALS)
  RegisterPrefsForadrbrowsielReferralsService(registry);
#endif
#if defined(OS_MAC)
  // Turn off super annoying 'Hold to quit'
  registry->SetDefaultPrefValue(prefs::kConfirmToQuitEnabled,
      base::Value(false));
#endif
#if BUILDFLAG(ENABLE_TOR)
  tor::TorProfileService::RegisterLocalStatePrefs(registry);
#endif
  registry->SetDefaultPrefValue(
      metrics::prefs::kMetricsReportingEnabled,
      base::Value(GetDefaultPrefValueForMetricsReporting()));

#if BUILDFLAG(adrbrowsiel_P3A_ENABLED)
  adrbrowsiel::adrbrowsielP3AService::RegisterPrefs(registry,
#if !defined(OS_ANDROID)
                                        first_run::IsChromeFirstRun());
#else
                                        // adrbrowsielP3AService::RegisterPrefs
                                        // doesn't use this arg on Android
                                        false);
#endif  // !defined(OS_ANDROID)

#endif  // BUILDFLAG(adrbrowsiel_P3A_ENABLED)

  adrbrowsiel_shields::RegisterShieldsP3APrefs(registry);
#if !defined(OS_ANDROID)
  adrbrowsielNewTabMessageHandler::RegisterLocalStatePrefs(registry);
  adrbrowsielWindowTracker::RegisterPrefs(registry);
  adrbrowsielUptimeTracker::RegisterPrefs(registry);
  dark_mode::RegisteradrbrowsielDarkModeLocalStatePrefs(registry);

  registry->RegisterBooleanPref(kDefaultBrowserPromptEnabled, true);
#endif

#if BUILDFLAG(ENABLE_WIDEVINE)
  RegisterWidevineLocalstatePrefs(registry);
#endif

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
  decentralized_dns::DecentralizedDnsService::RegisterLocalStatePrefs(registry);
#endif

  RegisterLocalStatePrefsForMigration(registry);
}

}  // namespace adrbrowsiel
