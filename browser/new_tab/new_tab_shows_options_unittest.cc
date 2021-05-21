/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/new_tab/new_tab_shows_options.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/common/webui_url_constants.h"
#include "chrome/common/pref_names.h"
#include "chrome/test/base/testing_browser_process.h"
#include "chrome/test/base/testing_profile.h"
#include "chrome/test/base/testing_profile_manager.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/url_constants.h"

class adrbrowsielNewTabTest : public testing::Test {
 public:
  adrbrowsielNewTabTest() : manager_(TestingBrowserProcess::GetGlobal()) {}
  ~adrbrowsielNewTabTest() override {}

 protected:
  void SetUp() override {
    ASSERT_TRUE(manager_.SetUp());
  }

  TestingProfileManager* manager() { return &manager_; }

 private:
  content::BrowserTaskEnvironment task_environment_;
  TestingProfileManager manager_;
};

TEST_F(adrbrowsielNewTabTest, BasicTest) {
  Profile* profile = manager()->CreateTestingProfile("Test 1");
  Profile* otr_profile = profile->GetPrimaryOTRProfile();
  ASSERT_TRUE(profile);
  ASSERT_TRUE(otr_profile);

  auto* prefs = profile->GetPrefs();

  // Check NTP url is empty for DASHBOARD.
  prefs->SetInteger(
      kNewTabPageShowsOptions,
      static_cast<int>(adrbrowsiel::NewTabPageShowsOptions::kDashboard));
  EXPECT_EQ(GURL(), adrbrowsiel::GetNewTabPageURL(profile));
  EXPECT_EQ(GURL(), adrbrowsiel::GetNewTabPageURL(otr_profile));
  EXPECT_TRUE(adrbrowsiel::ShouldUseNewTabURLForNewTab(profile));
  EXPECT_TRUE(adrbrowsiel::ShouldNewTabShowDashboard(profile));

  // Check NTP url is empty when option is HOMEPAGE and kHomePageIsNewTabPage
  // is true.
  prefs->SetInteger(
      kNewTabPageShowsOptions,
      static_cast<int>(adrbrowsiel::NewTabPageShowsOptions::kHomepage));
  prefs->SetString(prefs::kHomePage, "https://www.adrbrowsiel.com/");
  prefs->SetBoolean(prefs::kHomePageIsNewTabPage, true);
  EXPECT_EQ(GURL(), adrbrowsiel::GetNewTabPageURL(profile));
  EXPECT_EQ(GURL(), adrbrowsiel::GetNewTabPageURL(otr_profile));
  EXPECT_TRUE(adrbrowsiel::ShouldUseNewTabURLForNewTab(profile));
  EXPECT_TRUE(adrbrowsiel::ShouldNewTabShowDashboard(profile));

  // Check NTP url is configured url when option is HOMEPAGE and
  // kHomePageIsNewTabPage is false.
  prefs->SetBoolean(prefs::kHomePageIsNewTabPage, false);
  EXPECT_EQ(GURL("https://www.adrbrowsiel.com/"), adrbrowsiel::GetNewTabPageURL(profile));
  EXPECT_EQ(GURL(), adrbrowsiel::GetNewTabPageURL(otr_profile));
  EXPECT_FALSE(adrbrowsiel::ShouldUseNewTabURLForNewTab(profile));
  EXPECT_FALSE(adrbrowsiel::ShouldNewTabShowDashboard(profile));

  // If homepage url is newtab url, dashboard settings should be shown.
  prefs->SetString(prefs::kHomePage, "chrome://newtab/");
  EXPECT_TRUE(adrbrowsiel::ShouldUseNewTabURLForNewTab(profile));
  EXPECT_TRUE(adrbrowsiel::ShouldNewTabShowDashboard(profile));

  // Check NTP url is used when option is BLANKPAGE.
  // Blank page will go NTP route and adrbrowsielNewTabUI will handle it.
  prefs->SetInteger(
      kNewTabPageShowsOptions,
      static_cast<int>(adrbrowsiel::NewTabPageShowsOptions::kBlankpage));
  EXPECT_EQ(GURL(), adrbrowsiel::GetNewTabPageURL(profile));
  EXPECT_EQ(GURL(), adrbrowsiel::GetNewTabPageURL(otr_profile));
  EXPECT_TRUE(adrbrowsiel::ShouldUseNewTabURLForNewTab(profile));
  EXPECT_FALSE(adrbrowsiel::ShouldNewTabShowDashboard(profile));
  EXPECT_TRUE(adrbrowsiel::ShouldNewTabShowBlankpage(profile));
  EXPECT_FALSE(adrbrowsiel::ShouldNewTabShowBlankpage(otr_profile));
}
