/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_adrbrowsiel_STATS_adrbrowsiel_STATS_TAB_HELPER_H_
#define adrbrowsiel_BROWSER_adrbrowsiel_STATS_adrbrowsiel_STATS_TAB_HELPER_H_

#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

namespace content {
class NavigationHandle;
class WebContents;
}  // namespace content

namespace adrbrowsiel_stats {

class adrbrowsielStatsUpdater;

class adrbrowsielStatsTabHelper
    : public content::WebContentsObserver,
      public content::WebContentsUserData<adrbrowsielStatsTabHelper> {
 public:
  explicit adrbrowsielStatsTabHelper(content::WebContents*);
  ~adrbrowsielStatsTabHelper() override;
  adrbrowsielStatsTabHelper(const adrbrowsielStatsTabHelper&) = delete;
  adrbrowsielStatsTabHelper& operator=(const adrbrowsielStatsTabHelper&) = delete;

  void NotifyStatsUpdater();

 private:
  void DidStartNavigation(content::NavigationHandle*) override;

  friend class content::WebContentsUserData<adrbrowsielStatsTabHelper>;
  WEB_CONTENTS_USER_DATA_KEY_DECL();
};

}  // namespace adrbrowsiel_stats
#endif  // adrbrowsiel_BROWSER_adrbrowsiel_STATS_adrbrowsiel_STATS_TAB_HELPER_H_
