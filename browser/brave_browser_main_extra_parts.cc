/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/adrbrowsiel_browser_main_extra_parts.h"

#include "base/metrics/histogram_macros.h"
#include "adrbrowsiel/browser/adrbrowsiel_browser_process_impl.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/adrbrowsiel_shields_p3a.h"
#include "adrbrowsiel/components/p3a/adrbrowsiel_p3a_service.h"
#include "adrbrowsiel/components/p3a/buildflags.h"
#include "components/metrics/metrics_pref_names.h"
#include "components/prefs/pref_service.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"

#if !defined(OS_ANDROID)
#include "adrbrowsiel/browser/importer/adrbrowsiel_importer_p3a.h"
#include "adrbrowsiel/browser/p3a/p3a_core_metrics.h"
#include "adrbrowsiel/browser/ui/webui/new_tab_page/adrbrowsiel_new_tab_message_handler.h"
#include "chrome/browser/first_run/first_run.h"
#endif  // !defined(OS_ANDROID)

namespace {

// Records default values for some histograms because we want these stats to be
// uploaded anyways. Corresponding components will write new values according
// to their usage scenarios.
//
// For profile specific values, see browser/profiles/profile_util.cc
void RecordInitialP3AValues() {
#if !defined(OS_ANDROID)
  if (first_run::IsChromeFirstRun()) {
    RecordImporterP3A(importer::ImporterType::TYPE_UNKNOWN);
  }

  adrbrowsielNewTabMessageHandler::RecordInitialP3AValues(
      g_browser_process->local_state());
#endif  // !defined(OS_ANDROID)

  adrbrowsiel_shields::MaybeRecordShieldsUsageP3A(adrbrowsiel_shields::kNeverClicked,
                                            g_browser_process->local_state());

  // Record crash reporting status stats.
  const bool crash_reports_enabled = g_browser_process->local_state()->
      GetBoolean(metrics::prefs::kMetricsReportingEnabled);
  UMA_HISTOGRAM_BOOLEAN("adrbrowsiel.Core.CrashReportsEnabled",
                        crash_reports_enabled);
}

}  // namespace

adrbrowsielBrowserMainExtraParts::adrbrowsielBrowserMainExtraParts() {
}

adrbrowsielBrowserMainExtraParts::~adrbrowsielBrowserMainExtraParts() {
}

void adrbrowsielBrowserMainExtraParts::PostBrowserStart() {
  g_adrbrowsiel_browser_process->StartadrbrowsielServices();
}

void adrbrowsielBrowserMainExtraParts::PreMainMessageLoopRun() {
  // Disabled on mobile platforms, see for instance issues/6176
#if BUILDFLAG(adrbrowsiel_P3A_ENABLED)
  // TODO(iefremov): Maybe find a better place for this initialization.
  g_adrbrowsiel_browser_process->adrbrowsiel_p3a_service()->Init(
      g_browser_process->shared_url_loader_factory());
#endif  // BUILDFLAG(adrbrowsiel_P3A_ENABLED)

  RecordInitialP3AValues();

  // The code below is not supported on android.
#if !defined(OS_ANDROID)
  adrbrowsiel::adrbrowsielWindowTracker::CreateInstance(g_browser_process->local_state());
  adrbrowsiel::adrbrowsielUptimeTracker::CreateInstance(g_browser_process->local_state());
#endif  // !defined(OS_ANDROID)
}
