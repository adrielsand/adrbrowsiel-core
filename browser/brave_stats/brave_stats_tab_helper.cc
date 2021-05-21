/* Copyright 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/adrbrowsiel_stats/adrbrowsiel_stats_tab_helper.h"

#include "adrbrowsiel/browser/adrbrowsiel_browser_process.h"
#include "adrbrowsiel/browser/adrbrowsiel_stats/adrbrowsiel_stats_updater.h"
#include "content/public/browser/navigation_handle.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_user_data.h"
#include "ui/base/page_transition_types.h"

namespace adrbrowsiel_stats {

adrbrowsielStatsTabHelper::adrbrowsielStatsTabHelper(content::WebContents *web_contents)
    : content::WebContentsObserver(web_contents) {}

adrbrowsielStatsTabHelper::~adrbrowsielStatsTabHelper() = default;

void adrbrowsielStatsTabHelper::DidStartNavigation(
        content::NavigationHandle* handle) {
  if (!handle || !handle->IsInMainFrame() || handle->IsDownload())
    return;

  auto transition = handle->GetPageTransition();

  switch (ui::PageTransitionStripQualifier(transition)) {
    case ui::PAGE_TRANSITION_TYPED:
    case ui::PAGE_TRANSITION_AUTO_BOOKMARK:
    case ui::PAGE_TRANSITION_GENERATED: {
      auto url = handle->GetURL();
      if (!(url.SchemeIs("chrome") || url.SchemeIs("adrbrowsiel")))
        NotifyStatsUpdater();
      break;
    }
    default:
      break;
  }
}

void adrbrowsielStatsTabHelper::NotifyStatsUpdater() {
  if (g_adrbrowsiel_browser_process->adrbrowsiel_stats_updater()->MaybeDoThresholdPing(1))
    web_contents()->RemoveUserData(UserDataKey());
}

WEB_CONTENTS_USER_DATA_KEY_IMPL(adrbrowsielStatsTabHelper)

}  //  namespace adrbrowsiel_stats
