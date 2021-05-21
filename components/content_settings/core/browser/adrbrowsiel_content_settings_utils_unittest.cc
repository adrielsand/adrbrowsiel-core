/*  Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>

#include "base/macros.h"
#include "base/optional.h"
#include "adrbrowsiel/components/content_settings/core/browser/adrbrowsiel_content_settings_utils.h"
#include "chrome/test/base/testing_profile.h"
#include "components/content_settings/core/common/content_settings_pattern.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"

using content_settings::ConvertPatternToWildcardSchemeAndPort;

class adrbrowsielContentSettingsUtilsTest : public testing::Test {
 public:
  adrbrowsielContentSettingsUtilsTest() = default;
  ~adrbrowsielContentSettingsUtilsTest() override = default;

  void SetUp() override { profile_ = std::make_unique<TestingProfile>(); }

  TestingProfile* profile() { return profile_.get(); }

 private:
  content::BrowserTaskEnvironment task_environment_;
  std::unique_ptr<TestingProfile> profile_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielContentSettingsUtilsTest);
};

TEST_F(adrbrowsielContentSettingsUtilsTest,
       TestConvertPatternToWildcardSchemeAndPort) {
  // Full wildcard pattern.
  EXPECT_EQ(base::nullopt, ConvertPatternToWildcardSchemeAndPort(
                               ContentSettingsPattern::Wildcard()));

  // adrbrowsiel first party placeholder pattern.
  EXPECT_EQ(base::nullopt,
            ConvertPatternToWildcardSchemeAndPort(
                ContentSettingsPattern::FromString("https://firstParty/*")));

  // file:// scheme pattern.
  EXPECT_EQ(base::nullopt,
            ConvertPatternToWildcardSchemeAndPort(
                ContentSettingsPattern::FromString("file:///a/b/c.zip")));

  // Wildcard host pattern.
  EXPECT_EQ(base::nullopt,
            ConvertPatternToWildcardSchemeAndPort(
                ContentSettingsPattern::FromString("http://*:8080/*")));

  // Wildcard scheme, no port.
  EXPECT_EQ(base::nullopt,
            ConvertPatternToWildcardSchemeAndPort(
                ContentSettingsPattern::FromString("*://adrbrowsiel.com/*")));
  EXPECT_EQ(base::nullopt,
            ConvertPatternToWildcardSchemeAndPort(
                ContentSettingsPattern::FromString("*://adrbrowsiel.com:*/")));

  // Wildcard scheme, has port.
  auto pattern = ConvertPatternToWildcardSchemeAndPort(
      ContentSettingsPattern::FromString("*://adrbrowsiel.com:8080/*"));
  EXPECT_NE(base::nullopt, pattern);
  EXPECT_EQ(pattern->ToString(), "adrbrowsiel.com");
  EXPECT_TRUE(pattern->Matches(GURL("http://adrbrowsiel.com:80/path1")));
  EXPECT_TRUE(pattern->Matches(GURL("https://adrbrowsiel.com/path2")));
  EXPECT_FALSE(pattern->Matches(GURL("http://adrbrowsiel2.com:8080")));
  pattern.reset();

  // Scheme, no port.
  pattern = ConvertPatternToWildcardSchemeAndPort(
      ContentSettingsPattern::FromString("http://adrbrowsiel.com/"));
  EXPECT_NE(base::nullopt, pattern);
  EXPECT_EQ(pattern->ToString(), "adrbrowsiel.com");
  EXPECT_TRUE(pattern->Matches(GURL("ftp://adrbrowsiel.com:80/path1")));
  EXPECT_TRUE(pattern->Matches(GURL("https://adrbrowsiel.com/path2")));
  EXPECT_FALSE(pattern->Matches(GURL("http://adrbrowsiel2.com:8080")));
  pattern.reset();

  // Scheme and port.
  pattern = ConvertPatternToWildcardSchemeAndPort(
      ContentSettingsPattern::FromString("https://adrbrowsiel.com:56558/"));
  EXPECT_NE(base::nullopt, pattern);
  EXPECT_EQ(pattern->ToString(), "adrbrowsiel.com");
  EXPECT_TRUE(pattern->Matches(GURL("wss://adrbrowsiel.com:80/path1")));
  EXPECT_TRUE(pattern->Matches(GURL("https://adrbrowsiel.com/path2")));
  EXPECT_FALSE(pattern->Matches(GURL("http://adrbrowsiel2.com:8080")));
}
