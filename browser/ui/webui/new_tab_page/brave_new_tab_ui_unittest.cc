/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/webui/new_tab_page/adrbrowsiel_new_tab_ui_utils.h"
#include "components/history/core/browser/top_sites_impl.h"
#include "components/ntp_tiles/constants.h"
#include "testing/gtest/include/gtest/gtest.h"

TEST(adrbrowsielNewTabUITest, ConstantsTest) {
  // Make sure history/ntp_tiles module has proper constants for our NTP
  // requirements.
  constexpr size_t kadrbrowsielMaxTopSitesNumber = 12;
  constexpr size_t kTopSitesNumber = history::TopSitesImpl::kTopSitesNumber;

  EXPECT_EQ(kadrbrowsielMaxTopSitesNumber, kTopSitesNumber);
  EXPECT_EQ(kadrbrowsielMaxTopSitesNumber, ntp_tiles::kMaxNumCustomLinks);
  EXPECT_EQ(kadrbrowsielMaxTopSitesNumber, ntp_tiles::kMaxNumMostVisited);
  EXPECT_EQ(static_cast<int>(kadrbrowsielMaxTopSitesNumber), ntp_tiles::kMaxNumTiles);
}

TEST(adrbrowsielNewTabUITest, TopSiteURLValidation) {
  std::string url = "a";
  EXPECT_TRUE(GetValidURLStringForTopSite(&url));
  EXPECT_EQ("https://a", url);

  url = "http://a";
  EXPECT_TRUE(GetValidURLStringForTopSite(&url));
  EXPECT_EQ("http://a", url);

  url = "https://a";
  EXPECT_TRUE(GetValidURLStringForTopSite(&url));
  EXPECT_EQ("https://a", url);

  url = "https://www.a.com";
  EXPECT_TRUE(GetValidURLStringForTopSite(&url));
  EXPECT_EQ("https://www.a.com", url);

  // Check failed to make vaile url.
  url = "!@";
  EXPECT_FALSE(GetValidURLStringForTopSite(&url));

  url = "";
  EXPECT_FALSE(GetValidURLStringForTopSite(&url));
}
