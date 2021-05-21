/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/adrbrowsiel_search/browser/adrbrowsiel_search_host.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"

namespace adrbrowsiel_search {

TEST(adrbrowsielSearchHost, GetBackupResultURL) {
  GURL base_url("https://www.google.com/search/");
  ASSERT_EQ(
      adrbrowsielSearchHost::GetBackupResultURL(base_url, "test", "en", "ca", "32,32",
                                          true),
      GURL("https://www.google.com/search/?q=test&hl=en&gl=ca&safe=active"));
}

TEST(adrbrowsielSearchHost, GetBackupResultURLNoLang) {
  GURL base_url("https://www.google.com/search/");
  ASSERT_EQ(adrbrowsielSearchHost::GetBackupResultURL(base_url, "test", "", "ca",
                                                "32,32", true),
            GURL("https://www.google.com/search/?q=test&gl=ca&safe=active"));
}

TEST(adrbrowsielSearchHost, GetBackupResultURLNoCountry) {
  GURL base_url("https://www.google.com/search/");
  ASSERT_EQ(adrbrowsielSearchHost::GetBackupResultURL(base_url, "test", "en", "",
                                                "32,32", true),
            GURL("https://www.google.com/search/?q=test&hl=en&safe=active"));
}

TEST(adrbrowsielSearchHost, GetBackupResultURLNoFilter) {
  GURL base_url("https://www.google.com/search/");
  ASSERT_EQ(adrbrowsielSearchHost::GetBackupResultURL(base_url, "test", "en", "ca",
                                                "32,32", false),
            GURL("https://www.google.com/search/?q=test&hl=en&gl=ca"));
}

TEST(adrbrowsielSearchHost, GetBackupResultURLMinimal) {
  GURL base_url("https://www.google.com/search/");
  ASSERT_EQ(
      adrbrowsielSearchHost::GetBackupResultURL(base_url, "test", "", "", "", false),
      GURL("https://www.google.com/search/?q=test"));
}

}  // namespace adrbrowsiel_search
