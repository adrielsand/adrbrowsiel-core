/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_REWARDS_BROWSER_TEST_COMMON_REWARDS_BROWSERTEST_CONTEXT_HELPER_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_REWARDS_BROWSER_TEST_COMMON_REWARDS_BROWSERTEST_CONTEXT_HELPER_H_

#include <vector>

#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/test/common/rewards_browsertest_util.h"
#include "build/build_config.h"
#include "chrome/browser/ui/browser.h"
#include "url/gurl.h"

namespace content {
class WebContents;
}  // namespace content

namespace rewards_browsertest {

class RewardsBrowserTestContextHelper {
 public:
  explicit RewardsBrowserTestContextHelper(Browser* browser);
  ~RewardsBrowserTestContextHelper();

  content::WebContents* OpenRewardsPopup();

  content::WebContents* OpenSiteBanner(
      rewards_browsertest_util::TipAction tip_action);

  void VisitPublisher(
      const GURL& url,
      const bool verified,
      const bool last_add = false);

  void LoadURL(GURL url);

  void ReloadCurrentSite();

 private:
  void OpenPopup();

  void OpenPopupFirstTime();

  Browser* browser_;  // NOT OWNED
  bool loaded_ = false;
};

}  // namespace rewards_browsertest

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_REWARDS_BROWSER_TEST_COMMON_REWARDS_BROWSERTEST_CONTEXT_HELPER_H_
