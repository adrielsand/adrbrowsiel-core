/*  Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>

#include "base/macros.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/adrbrowsiel_shields_util.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/common/adrbrowsiel_shield_constants.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/common/features.h"
#include "build/build_config.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/test/base/testing_profile.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/content_settings/core/common/content_settings_types.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gtest/include/gtest/gtest.h"

using adrbrowsiel_shields::ControlType;
using adrbrowsiel_shields::ControlTypeFromString;
using adrbrowsiel_shields::ControlTypeToString;
using adrbrowsiel_shields::GetPatternFromURL;
using adrbrowsiel_shields::features::kadrbrowsielDomainBlock;

class adrbrowsielShieldsUtilTest : public testing::Test {
 public:
  adrbrowsielShieldsUtilTest() = default;
  ~adrbrowsielShieldsUtilTest() override = default;

  void SetUp() override { profile_ = std::make_unique<TestingProfile>(); }

  TestingProfile* profile() { return profile_.get(); }

 private:
  content::BrowserTaskEnvironment task_environment_;
  std::unique_ptr<TestingProfile> profile_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielShieldsUtilTest);
};

class adrbrowsielShieldsUtilDomainBlockFeatureTest : public adrbrowsielShieldsUtilTest {
 public:
  adrbrowsielShieldsUtilDomainBlockFeatureTest() {
    feature_list_.InitAndDisableFeature(kadrbrowsielDomainBlock);
  }

 private:
  base::test::ScopedFeatureList feature_list_;
};

TEST_F(adrbrowsielShieldsUtilTest, GetPatternFromURL) {
  // wildcard
  auto pattern = GetPatternFromURL(GURL());
  EXPECT_EQ(ContentSettingsPattern::Wildcard(), pattern);

  // scheme is a wildcard, should match any scheme
  pattern = GetPatternFromURL(GURL("http://adrbrowsiel.com"));
  EXPECT_TRUE(pattern.Matches(GURL("http://adrbrowsiel.com")));
  EXPECT_TRUE(pattern.Matches(GURL("http://adrbrowsiel.com/path1")));
  EXPECT_TRUE(pattern.Matches(GURL("http://adrbrowsiel.com/path2")));
  EXPECT_TRUE(pattern.Matches(GURL("https://adrbrowsiel.com")));
  EXPECT_TRUE(pattern.Matches(GURL("ftp://adrbrowsiel.com")));
  EXPECT_FALSE(pattern.Matches(GURL("http://subdomain.adrbrowsiel.com")));
  EXPECT_FALSE(pattern.Matches(GURL("http://adrbrowsiel2.com")));

  // path is a wildcard
  pattern = GetPatternFromURL(GURL("http://adrbrowsiel.com/path1"));
  EXPECT_TRUE(pattern.Matches(GURL("http://adrbrowsiel.com")));
  EXPECT_TRUE(pattern.Matches(GURL("http://adrbrowsiel.com/path1")));
  EXPECT_TRUE(pattern.Matches(GURL("http://adrbrowsiel.com/path2")));
  EXPECT_FALSE(pattern.Matches(GURL("http://subdomain.adrbrowsiel.com")));
  EXPECT_FALSE(pattern.Matches(GURL("http://adrbrowsiel2.com")));

  // port is a wildcard
  pattern = GetPatternFromURL(GURL("http://adrbrowsiel.com:8080"));
  EXPECT_TRUE(pattern.Matches(GURL("http://adrbrowsiel.com")));
  EXPECT_TRUE(pattern.Matches(GURL("http://adrbrowsiel.com:8080")));
  EXPECT_TRUE(pattern.Matches(GURL("http://adrbrowsiel.com:8080/path1")));
  EXPECT_TRUE(pattern.Matches(GURL("http://adrbrowsiel.com:8080/path2")));
  EXPECT_TRUE(pattern.Matches(GURL("http://adrbrowsiel.com:5555")));
  EXPECT_TRUE(pattern.Matches(GURL("https://adrbrowsiel.com")));
  EXPECT_TRUE(pattern.Matches(GURL("https://adrbrowsiel.com:8080")));
  EXPECT_FALSE(pattern.Matches(GURL("http://subdomain.adrbrowsiel.com")));
  EXPECT_FALSE(pattern.Matches(GURL("http://adrbrowsiel2.com")));

  // with implied port
  pattern = GetPatternFromURL(GURL("https://brianbondy.com"));
  EXPECT_EQ(pattern.ToString(), "brianbondy.com");
  pattern = GetPatternFromURL(GURL("http://brianbondy.com"));
  EXPECT_EQ(pattern.ToString(), "brianbondy.com");
  // with specified port
  pattern = GetPatternFromURL(GURL("http://brianbondy.com:8080"));
  EXPECT_EQ(pattern.ToString(), "brianbondy.com");
}

TEST_F(adrbrowsielShieldsUtilTest, ControlTypeToString) {
  EXPECT_EQ("block", ControlTypeToString(ControlType::BLOCK));
  EXPECT_EQ("allow", ControlTypeToString(ControlType::ALLOW));
  EXPECT_EQ("block_third_party",
            ControlTypeToString(ControlType::BLOCK_THIRD_PARTY));
}

TEST_F(adrbrowsielShieldsUtilTest, ControlTypeFromString) {
  EXPECT_EQ(ControlType::BLOCK, ControlTypeFromString("block"));
  EXPECT_EQ(ControlType::ALLOW, ControlTypeFromString("allow"));
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY,
            ControlTypeFromString("block_third_party"));
}

/* adrbrowsiel_SHIELDS CONTROL */
TEST_F(adrbrowsielShieldsUtilTest, SetadrbrowsielShieldsEnabled_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  adrbrowsiel_shields::SetadrbrowsielShieldsEnabled(map, true, GURL("http://adrbrowsiel.com"));
  // setting should apply to origin
  auto setting = map->GetContentSetting(GURL("http://adrbrowsiel.com"), GURL(),
                                        ContentSettingsType::adrbrowsiel_SHIELDS);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // setting should apply to different scheme
  setting = map->GetContentSetting(GURL("https://adrbrowsiel.com"), GURL(),
                                   ContentSettingsType::adrbrowsiel_SHIELDS);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // setting should not apply to default
  setting = map->GetContentSetting(GURL(), GURL(),
                                   ContentSettingsType::adrbrowsiel_SHIELDS);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);
}

TEST_F(adrbrowsielShieldsUtilTest, SetadrbrowsielShieldsEnabled_IsNotHttpHttps) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  auto setting =
      adrbrowsiel_shields::GetadrbrowsielShieldsEnabled(map, GURL("chrome://preferences"));
  EXPECT_EQ(false, setting);
  adrbrowsiel_shields::SetadrbrowsielShieldsEnabled(map, ControlType::ALLOW,
                                        GURL("chrome://preferences"));
  setting =
      adrbrowsiel_shields::GetadrbrowsielShieldsEnabled(map, GURL("chrome://preferences"));
  EXPECT_EQ(false, setting);

  setting = adrbrowsiel_shields::GetadrbrowsielShieldsEnabled(map, GURL("about:blank"));
  EXPECT_EQ(false, setting);
  adrbrowsiel_shields::SetadrbrowsielShieldsEnabled(map, ControlType::ALLOW,
                                        GURL("about:blank"));
  setting = adrbrowsiel_shields::GetadrbrowsielShieldsEnabled(map, GURL("about:blank"));
  EXPECT_EQ(false, setting);
}

TEST_F(adrbrowsielShieldsUtilTest, GetadrbrowsielShieldsEnabled_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = adrbrowsiel_shields::GetadrbrowsielShieldsEnabled(map, GURL());
  EXPECT_EQ(true, setting);
  setting =
      adrbrowsiel_shields::GetadrbrowsielShieldsEnabled(map, GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(true, setting);
  setting =
      adrbrowsiel_shields::GetadrbrowsielShieldsEnabled(map, GURL("https://adrbrowsiel.com"));
  EXPECT_EQ(true, setting);

  /* BLOCK */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://adrbrowsiel.com/*"),
      ContentSettingsPattern::Wildcard(), ContentSettingsType::adrbrowsiel_SHIELDS,
      CONTENT_SETTING_BLOCK);
  setting =
      adrbrowsiel_shields::GetadrbrowsielShieldsEnabled(map, GURL("http://adrbrowsiel.com/*"));
  EXPECT_EQ(false, setting);
  // https in unchanged
  setting =
      adrbrowsiel_shields::GetadrbrowsielShieldsEnabled(map, GURL("https://adrbrowsiel.com"));
  EXPECT_EQ(true, setting);
  // default is unchanged
  setting = adrbrowsiel_shields::GetadrbrowsielShieldsEnabled(map, GURL());
  EXPECT_EQ(true, setting);
}

TEST_F(adrbrowsielShieldsUtilTest, GetadrbrowsielShieldsEnabled_IsNotHttpHttps) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  auto setting =
      adrbrowsiel_shields::GetadrbrowsielShieldsEnabled(map, GURL("chrome://preferences"));
  EXPECT_EQ(false, setting);

  setting = adrbrowsiel_shields::GetadrbrowsielShieldsEnabled(map, GURL("about:blank"));
  EXPECT_EQ(false, setting);
}

/* AD CONTROL */
TEST_F(adrbrowsielShieldsUtilTest, SetAdControlType_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  // settings should be default
  auto setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::adrbrowsiel_ADS);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);
  setting = map->GetContentSetting(GURL("http://adrbrowsiel.com"), GURL(),
                                   ContentSettingsType::adrbrowsiel_ADS);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);

  /* ALLOW */
  adrbrowsiel_shields::SetAdControlType(map, ControlType::ALLOW, GURL());
  setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::adrbrowsiel_ADS);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // override should apply to all origins
  setting = map->GetContentSetting(GURL("http://adrbrowsiel.com"), GURL(),
                                   ContentSettingsType::adrbrowsiel_ADS);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  /* BLOCK */
  adrbrowsiel_shields::SetAdControlType(map, ControlType::BLOCK, GURL());
  setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::adrbrowsiel_ADS);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  // override should apply to all origins
  setting = map->GetContentSetting(GURL("http://adrbrowsiel.com"), GURL(),
                                   ContentSettingsType::adrbrowsiel_ADS);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
}

TEST_F(adrbrowsielShieldsUtilTest, SetAdControlType_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  adrbrowsiel_shields::SetAdControlType(map, ControlType::ALLOW,
                                  GURL("http://adrbrowsiel.com"));
  // setting should apply to origin
  auto setting = map->GetContentSetting(GURL("http://adrbrowsiel.com"), GURL(),
                                        ContentSettingsType::adrbrowsiel_ADS);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // setting should also apply to different scheme
  setting = map->GetContentSetting(GURL("https://adrbrowsiel.com"), GURL(),
                                   ContentSettingsType::adrbrowsiel_ADS);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // setting should not apply to default
  setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::adrbrowsiel_ADS);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);
}

TEST_F(adrbrowsielShieldsUtilTest, GetAdControlType_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = adrbrowsiel_shields::GetAdControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);

  /* ALLOW */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::adrbrowsiel_ADS, CONTENT_SETTING_ALLOW);
  setting = adrbrowsiel_shields::GetAdControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);

  /* BLOCK */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::adrbrowsiel_ADS, CONTENT_SETTING_BLOCK);
  setting = adrbrowsiel_shields::GetAdControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);
}

TEST_F(adrbrowsielShieldsUtilTest, GetAdControlType_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = adrbrowsiel_shields::GetAdControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);
  setting = adrbrowsiel_shields::GetAdControlType(map, GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  setting = adrbrowsiel_shields::GetAdControlType(map, GURL("https://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::BLOCK, setting);

  /* ALLOW */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://adrbrowsiel.com/*"),
      ContentSettingsPattern::Wildcard(), ContentSettingsType::adrbrowsiel_ADS,
      CONTENT_SETTING_ALLOW);
  setting = adrbrowsiel_shields::GetAdControlType(map, GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);

  // https in unchanged
  setting = adrbrowsiel_shields::GetAdControlType(map, GURL("https://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  // default is unchanged
  setting = adrbrowsiel_shields::GetAdControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);

  /* BLOCK */
  // change default to allow
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::adrbrowsiel_ADS, CONTENT_SETTING_ALLOW);
  setting = adrbrowsiel_shields::GetAdControlType(map, GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting = adrbrowsiel_shields::GetAdControlType(map, GURL("https://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting = adrbrowsiel_shields::GetAdControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);

  // set override to block
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://adrbrowsiel.com/*"),
      ContentSettingsPattern::Wildcard(), ContentSettingsType::adrbrowsiel_ADS,
      CONTENT_SETTING_BLOCK);
  setting = adrbrowsiel_shields::GetAdControlType(map, GURL("http://adrbrowsiel.com/*"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  // https in unchanged
  setting = adrbrowsiel_shields::GetAdControlType(map, GURL("https://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  // default is unchanged
  setting = adrbrowsiel_shields::GetAdControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);
}

/* COOKIE CONTROL */
TEST_F(adrbrowsielShieldsUtilTest, SetCookieControlType_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  // setting should be default to start with
  auto setting = map->GetContentSetting(GURL(), GURL(),
                                        ContentSettingsType::adrbrowsiel_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);
  setting = map->GetContentSetting(GURL(), GURL("https://firstParty"),
                                   ContentSettingsType::adrbrowsiel_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);
  setting = map->GetContentSetting(GURL("http://adrbrowsiel.com"), GURL(),
                                   ContentSettingsType::adrbrowsiel_COOKIES);
  setting = map->GetContentSetting(GURL("http://adrbrowsiel.com"),
                                   GURL("https://firstParty"),
                                   ContentSettingsType::adrbrowsiel_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);

  /* ALLOW */
  adrbrowsiel_shields::SetCookieControlType(map, ControlType::ALLOW, GURL());
  setting = map->GetContentSetting(GURL(), GURL(),
                                   ContentSettingsType::adrbrowsiel_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
  setting = map->GetContentSetting(GURL(), GURL("https://firstParty"),
                                   ContentSettingsType::adrbrowsiel_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
  // setting should apply to all urls
  setting = map->GetContentSetting(GURL("http://adrbrowsiel.com"), GURL(),
                                   ContentSettingsType::adrbrowsiel_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
  setting = map->GetContentSetting(GURL("http://adrbrowsiel.com"),
                                   GURL("https://firstParty"),
                                   ContentSettingsType::adrbrowsiel_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  /* BLOCK */
  adrbrowsiel_shields::SetCookieControlType(map, ControlType::BLOCK, GURL());
  setting = map->GetContentSetting(GURL(), GURL(),
                                   ContentSettingsType::adrbrowsiel_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
  setting = map->GetContentSetting(GURL(), GURL("https://firstParty"),
                                   ContentSettingsType::adrbrowsiel_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
  // setting should apply to all urls
  setting = map->GetContentSetting(GURL("http://adrbrowsiel.com"), GURL(),
                                   ContentSettingsType::adrbrowsiel_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
  setting = map->GetContentSetting(GURL("http://adrbrowsiel.com"),
                                   GURL("https://firstParty"),
                                   ContentSettingsType::adrbrowsiel_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  /* BLOCK_THIRD_PARTY */
  adrbrowsiel_shields::SetCookieControlType(map, ControlType::BLOCK_THIRD_PARTY,
                                      GURL());
  setting = map->GetContentSetting(GURL(), GURL(),
                                   ContentSettingsType::adrbrowsiel_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
  setting = map->GetContentSetting(GURL(), GURL("https://firstParty"),
                                   ContentSettingsType::adrbrowsiel_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // setting should apply to all urls
  setting = map->GetContentSetting(GURL("http://adrbrowsiel.com"), GURL(),
                                   ContentSettingsType::adrbrowsiel_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
  setting = map->GetContentSetting(GURL("http://adrbrowsiel.com"),
                                   GURL("https://firstParty"),
                                   ContentSettingsType::adrbrowsiel_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
}

TEST_F(adrbrowsielShieldsUtilTest, SetCookieControlType_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  adrbrowsiel_shields::SetCookieControlType(map, ControlType::ALLOW,
                                      GURL("http://adrbrowsiel.com"));
  // override should apply to origin
  auto setting = map->GetContentSetting(GURL("http://adrbrowsiel.com"), GURL(),
                                        ContentSettingsType::adrbrowsiel_COOKIES);
  setting = map->GetContentSetting(GURL("http://adrbrowsiel.com"),
                                   GURL("https://firstParty"),
                                   ContentSettingsType::adrbrowsiel_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // override should also apply to different scheme
  setting = map->GetContentSetting(GURL("https://adrbrowsiel.com"), GURL(),
                                   ContentSettingsType::adrbrowsiel_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
  setting = map->GetContentSetting(GURL("https://adrbrowsiel.com"),
                                   GURL("https://firstParty"),
                                   ContentSettingsType::adrbrowsiel_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // override should not apply to default
  setting = map->GetContentSetting(GURL(), GURL(),
                                   ContentSettingsType::adrbrowsiel_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);
  // override should not apply to default
  setting = map->GetContentSetting(GURL(), GURL("https://firstParty"),
                                   ContentSettingsType::adrbrowsiel_COOKIES);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);
}

TEST_F(adrbrowsielShieldsUtilTest, GetCookieControlType_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = adrbrowsiel_shields::GetCookieControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);
  setting = adrbrowsiel_shields::GetCookieControlType(map, GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);

  /* ALLOW */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::adrbrowsiel_COOKIES, CONTENT_SETTING_ALLOW);
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(),
      ContentSettingsPattern::FromString("https://firstParty/*"),
      ContentSettingsType::adrbrowsiel_COOKIES, CONTENT_SETTING_ALLOW);
  setting = adrbrowsiel_shields::GetCookieControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting = adrbrowsiel_shields::GetCookieControlType(map, GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);

  /* BLOCK */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::adrbrowsiel_COOKIES, CONTENT_SETTING_BLOCK);
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(),
      ContentSettingsPattern::FromString("https://firstParty/*"),
      ContentSettingsType::adrbrowsiel_COOKIES, CONTENT_SETTING_BLOCK);
  setting = adrbrowsiel_shields::GetCookieControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);
  setting = adrbrowsiel_shields::GetCookieControlType(map, GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::BLOCK, setting);

  /* BLOCK_THIRD_PARTY */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::adrbrowsiel_COOKIES, CONTENT_SETTING_BLOCK);
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(),
      ContentSettingsPattern::FromString("https://firstParty/*"),
      ContentSettingsType::adrbrowsiel_COOKIES, CONTENT_SETTING_ALLOW);
  setting = adrbrowsiel_shields::GetCookieControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);
  setting = adrbrowsiel_shields::GetCookieControlType(map, GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);
}

TEST_F(adrbrowsielShieldsUtilTest, GetCookieControlType_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting =
      adrbrowsiel_shields::GetCookieControlType(map, GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);

  /* ALLOW */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://adrbrowsiel.com/*"),
      ContentSettingsPattern::Wildcard(), ContentSettingsType::adrbrowsiel_COOKIES,
      CONTENT_SETTING_ALLOW);
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://adrbrowsiel.com/*"),
      ContentSettingsPattern::FromString("https://firstParty/*"),
      ContentSettingsType::adrbrowsiel_COOKIES, CONTENT_SETTING_ALLOW);
  setting = adrbrowsiel_shields::GetCookieControlType(map, GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting = adrbrowsiel_shields::GetCookieControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);

  /* BLOCK */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://adrbrowsiel.com/*"),
      ContentSettingsPattern::Wildcard(), ContentSettingsType::adrbrowsiel_COOKIES,
      CONTENT_SETTING_BLOCK);
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://adrbrowsiel.com/*"),
      ContentSettingsPattern::FromString("https://firstParty/*"),
      ContentSettingsType::adrbrowsiel_COOKIES, CONTENT_SETTING_BLOCK);
  setting = adrbrowsiel_shields::GetCookieControlType(map, GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  setting = adrbrowsiel_shields::GetCookieControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);

  /* BLOCK_THIRD_PARTY */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://adrbrowsiel.com/*"),
      ContentSettingsPattern::Wildcard(), ContentSettingsType::adrbrowsiel_COOKIES,
      CONTENT_SETTING_BLOCK);
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://adrbrowsiel.com/*"),
      ContentSettingsPattern::FromString("https://firstParty/*"),
      ContentSettingsType::adrbrowsiel_COOKIES, CONTENT_SETTING_ALLOW);
  setting = adrbrowsiel_shields::GetCookieControlType(map, GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);
  setting = adrbrowsiel_shields::GetCookieControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);
}

/* FINGERPRINTING CONTROL */
TEST_F(adrbrowsielShieldsUtilTest, SetFingerprintingControlType_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  // setting should be default to start with
  auto type = adrbrowsiel_shields::GetFingerprintingControlType(map, GURL());
  EXPECT_EQ(ControlType::DEFAULT, type);
  type = adrbrowsiel_shields::GetFingerprintingControlType(map,
                                                     GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::DEFAULT, type);

  /* ALLOW */
  adrbrowsiel_shields::SetFingerprintingControlType(map, ControlType::ALLOW, GURL());
  type = adrbrowsiel_shields::GetFingerprintingControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, type);

  // setting should apply to all urls
  type = adrbrowsiel_shields::GetFingerprintingControlType(map,
                                                     GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::ALLOW, type);

  /* BLOCK */
  adrbrowsiel_shields::SetFingerprintingControlType(map, ControlType::BLOCK, GURL());
  type = adrbrowsiel_shields::GetFingerprintingControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, type);

  // setting should apply to all urls
  type = adrbrowsiel_shields::GetFingerprintingControlType(map,
                                                     GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::BLOCK, type);

  /* DEFAULT */
  adrbrowsiel_shields::SetFingerprintingControlType(map, ControlType::DEFAULT,
                                              GURL());
  type = adrbrowsiel_shields::GetFingerprintingControlType(map, GURL());
  EXPECT_EQ(ControlType::DEFAULT, type);

  // setting should apply to all urls
  type = adrbrowsiel_shields::GetFingerprintingControlType(map,
                                                     GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::DEFAULT, type);

  /* Global ALLOW and Site explicit DEFAULT */
  adrbrowsiel_shields::SetFingerprintingControlType(map, ControlType::ALLOW, GURL());
  adrbrowsiel_shields::SetFingerprintingControlType(map, ControlType::DEFAULT,
                                              GURL("http://adrbrowsiel.com"));
  // Site should have DEFAULT if it's explicitly set.
  type = adrbrowsiel_shields::GetFingerprintingControlType(map,
                                                     GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::DEFAULT, type);

  /* Global BLOCK and Site explicit DEFAULT */
  adrbrowsiel_shields::SetFingerprintingControlType(map, ControlType::BLOCK, GURL());
  // Site should have DEFAULT if it's explicitly set.
  type = adrbrowsiel_shields::GetFingerprintingControlType(map,
                                                     GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::DEFAULT, type);
}

TEST_F(adrbrowsielShieldsUtilTest, SetFingerprintingControlType_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  adrbrowsiel_shields::SetFingerprintingControlType(map, ControlType::ALLOW,
                                              GURL("http://adrbrowsiel.com"));
  auto type = adrbrowsiel_shields::GetFingerprintingControlType(
      map, GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::ALLOW, type);
  // override should also apply to different scheme
  type = adrbrowsiel_shields::GetFingerprintingControlType(map,
                                                     GURL("https://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::ALLOW, type);

  adrbrowsiel_shields::SetFingerprintingControlType(map, ControlType::BLOCK,
                                              GURL("http://adrbrowsiel.com"));
  type = adrbrowsiel_shields::GetFingerprintingControlType(map,
                                                     GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::BLOCK, type);
  // override should also apply to different scheme
  type = adrbrowsiel_shields::GetFingerprintingControlType(map,
                                                     GURL("https://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::BLOCK, type);

  // override should not apply to default
  type = adrbrowsiel_shields::GetFingerprintingControlType(map, GURL());
  EXPECT_EQ(ControlType::DEFAULT, type);
}

/* HTTPSEVERYWHERE CONTROL */
TEST_F(adrbrowsielShieldsUtilTest, SetHTTPSEverywhereEnabled_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  // settings should be default
  auto setting = map->GetContentSetting(
      GURL(), GURL(), ContentSettingsType::adrbrowsiel_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);
  setting = map->GetContentSetting(
      GURL("http://adrbrowsiel.com"), GURL(),
      ContentSettingsType::adrbrowsiel_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);

  /* disabled */
  adrbrowsiel_shields::SetHTTPSEverywhereEnabled(map, false, GURL());
  setting = map->GetContentSetting(
      GURL(), GURL(), ContentSettingsType::adrbrowsiel_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // override should apply to all origins
  setting = map->GetContentSetting(
      GURL("http://adrbrowsiel.com"), GURL(),
      ContentSettingsType::adrbrowsiel_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  /* enabled */
  adrbrowsiel_shields::SetHTTPSEverywhereEnabled(map, true, GURL());
  setting = map->GetContentSetting(
      GURL(), GURL(), ContentSettingsType::adrbrowsiel_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  // override should apply to all origins
  setting = map->GetContentSetting(
      GURL("http://adrbrowsiel.com"), GURL(),
      ContentSettingsType::adrbrowsiel_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
}

TEST_F(adrbrowsielShieldsUtilTest, SetHTTPSEverywhereEnabled_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  adrbrowsiel_shields::SetHTTPSEverywhereEnabled(map, false,
                                           GURL("http://adrbrowsiel.com"));
  // setting should apply to origin
  auto setting = map->GetContentSetting(
      GURL("http://adrbrowsiel.com"), GURL(),
      ContentSettingsType::adrbrowsiel_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // setting should apply to different scheme
  setting = map->GetContentSetting(
      GURL("https://adrbrowsiel.com"), GURL(),
      ContentSettingsType::adrbrowsiel_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // setting should not apply to default
  setting = map->GetContentSetting(
      GURL(), GURL(), ContentSettingsType::adrbrowsiel_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, setting);
}

TEST_F(adrbrowsielShieldsUtilTest, GetHTTPSEverywhereEnabled_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = adrbrowsiel_shields::GetHTTPSEverywhereEnabled(map, GURL());
  EXPECT_EQ(true, setting);

  /* ALLOW */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::adrbrowsiel_HTTP_UPGRADABLE_RESOURCES,
      CONTENT_SETTING_ALLOW);
  setting = adrbrowsiel_shields::GetHTTPSEverywhereEnabled(map, GURL());
  EXPECT_EQ(false, setting);

  /* BLOCK */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::adrbrowsiel_HTTP_UPGRADABLE_RESOURCES,
      CONTENT_SETTING_BLOCK);
  setting = adrbrowsiel_shields::GetHTTPSEverywhereEnabled(map, GURL());
  EXPECT_EQ(true, setting);
}

TEST_F(adrbrowsielShieldsUtilTest, GetHTTPSEverywhereEnabled_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = adrbrowsiel_shields::GetHTTPSEverywhereEnabled(map, GURL());
  EXPECT_EQ(true, setting);
  setting =
      adrbrowsiel_shields::GetHTTPSEverywhereEnabled(map, GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(true, setting);
  setting =
      adrbrowsiel_shields::GetHTTPSEverywhereEnabled(map, GURL("https://adrbrowsiel.com"));
  EXPECT_EQ(true, setting);

  /* ALLOW */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://adrbrowsiel.com/*"),
      ContentSettingsPattern::Wildcard(),
      ContentSettingsType::adrbrowsiel_HTTP_UPGRADABLE_RESOURCES,
      CONTENT_SETTING_ALLOW);
  setting =
      adrbrowsiel_shields::GetHTTPSEverywhereEnabled(map, GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);

  // https in unchanged
  setting =
      adrbrowsiel_shields::GetHTTPSEverywhereEnabled(map, GURL("https://adrbrowsiel.com"));
  EXPECT_EQ(true, setting);
  // default is unchanged
  setting = adrbrowsiel_shields::GetHTTPSEverywhereEnabled(map, GURL());
  EXPECT_EQ(true, setting);

  /* BLOCK */
  // change default to allow
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::adrbrowsiel_HTTP_UPGRADABLE_RESOURCES,
      CONTENT_SETTING_ALLOW);
  setting =
      adrbrowsiel_shields::GetHTTPSEverywhereEnabled(map, GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting =
      adrbrowsiel_shields::GetHTTPSEverywhereEnabled(map, GURL("https://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting = adrbrowsiel_shields::GetHTTPSEverywhereEnabled(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);

  // set override to block
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://adrbrowsiel.com/*"),
      ContentSettingsPattern::Wildcard(),
      ContentSettingsType::adrbrowsiel_HTTP_UPGRADABLE_RESOURCES,
      CONTENT_SETTING_BLOCK);
  setting =
      adrbrowsiel_shields::GetHTTPSEverywhereEnabled(map, GURL("http://adrbrowsiel.com/*"));
  EXPECT_EQ(true, setting);
  // https in unchanged
  setting =
      adrbrowsiel_shields::GetHTTPSEverywhereEnabled(map, GURL("https://adrbrowsiel.com"));
  EXPECT_EQ(false, setting);
  // default is unchanged
  setting = adrbrowsiel_shields::GetHTTPSEverywhereEnabled(map, GURL());
  EXPECT_EQ(false, setting);
}

/* NOSCRIPT CONTROL */
TEST_F(adrbrowsielShieldsUtilTest, SetNoScriptControlType_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  // settings should be default
  auto setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
  setting = map->GetContentSetting(GURL("http://adrbrowsiel.com"), GURL(),
                                   ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  /* BLOCK */
  adrbrowsiel_shields::SetNoScriptControlType(map, ControlType::BLOCK, GURL());
  setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  // override should apply to all origins
  setting = map->GetContentSetting(GURL("http://adrbrowsiel.com"), GURL(),
                                   ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  /* ALLOW */
  adrbrowsiel_shields::SetNoScriptControlType(map, ControlType::ALLOW, GURL());
  setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // override should apply to all origins
  setting = map->GetContentSetting(GURL("http://adrbrowsiel.com"), GURL(),
                                   ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
}

TEST_F(adrbrowsielShieldsUtilTest, SetNoScriptControlType_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  adrbrowsiel_shields::SetNoScriptControlType(map, ControlType::BLOCK,
                                        GURL("http://adrbrowsiel.com"));
  // setting should apply to origin
  auto setting = map->GetContentSetting(GURL("http://adrbrowsiel.com"), GURL(),
                                        ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  // setting should also apply to different scheme
  setting = map->GetContentSetting(GURL("https://adrbrowsiel.com"), GURL(),
                                   ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  // setting should not apply to default
  setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
}

TEST_F(adrbrowsielShieldsUtilTest, GetNoScriptControlType_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = adrbrowsiel_shields::GetNoScriptControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);

  /* BLOCK */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::JAVASCRIPT, CONTENT_SETTING_BLOCK);
  setting = adrbrowsiel_shields::GetNoScriptControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);

  /* ALLOW */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::JAVASCRIPT, CONTENT_SETTING_ALLOW);
  setting = adrbrowsiel_shields::GetNoScriptControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);
}

TEST_F(adrbrowsielShieldsUtilTest, GetNoScriptControlType_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = adrbrowsiel_shields::GetNoScriptControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting =
      adrbrowsiel_shields::GetNoScriptControlType(map, GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting =
      adrbrowsiel_shields::GetNoScriptControlType(map, GURL("https://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);

  /* BLOCK */
  // set override to block
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://adrbrowsiel.com/*"),
      ContentSettingsPattern::Wildcard(), ContentSettingsType::JAVASCRIPT,
      CONTENT_SETTING_BLOCK);
  setting =
      adrbrowsiel_shields::GetNoScriptControlType(map, GURL("http://adrbrowsiel.com/*"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  // https in unchanged
  setting =
      adrbrowsiel_shields::GetNoScriptControlType(map, GURL("https://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  // default is unchanged
  setting = adrbrowsiel_shields::GetNoScriptControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);

  /* ALLOW */
  // change default to block
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::JAVASCRIPT, CONTENT_SETTING_BLOCK);
  setting =
      adrbrowsiel_shields::GetNoScriptControlType(map, GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  setting =
      adrbrowsiel_shields::GetNoScriptControlType(map, GURL("https://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  setting = adrbrowsiel_shields::GetNoScriptControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);

  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://adrbrowsiel.com/*"),
      ContentSettingsPattern::Wildcard(), ContentSettingsType::JAVASCRIPT,
      CONTENT_SETTING_ALLOW);
  setting =
      adrbrowsiel_shields::GetNoScriptControlType(map, GURL("http://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);

  // https in unchanged
  setting =
      adrbrowsiel_shields::GetNoScriptControlType(map, GURL("https://adrbrowsiel.com"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  // default is unchanged
  setting = adrbrowsiel_shields::GetNoScriptControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);
}

// Should not do domain blocking if domain blocking feature is disabled
TEST_F(adrbrowsielShieldsUtilDomainBlockFeatureTest, ShouldDoDomainBlocking) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  auto setting =
      adrbrowsiel_shields::ShouldDoDomainBlocking(map, GURL("https://adrbrowsiel.com"));
  EXPECT_EQ(false, setting);
}

// Should not do domain blocking if adrbrowsiel Shields is down
TEST_F(adrbrowsielShieldsUtilTest, ShouldDoDomainBlocking_ShieldsDown) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  GURL url = GURL("https://adrbrowsiel.com");
  adrbrowsiel_shields::SetadrbrowsielShieldsEnabled(map, false, url);
  auto setting = adrbrowsiel_shields::ShouldDoDomainBlocking(map, url);
  EXPECT_EQ(false, setting);
}

// Should not do domain blocking on non-HTTP(S) URLs
TEST_F(adrbrowsielShieldsUtilTest, ShouldDoDomainBlocking_IsNotHttpHttps) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  auto setting =
      adrbrowsiel_shields::ShouldDoDomainBlocking(map, GURL("chrome://preferences"));
  EXPECT_EQ(false, setting);

  setting = adrbrowsiel_shields::ShouldDoDomainBlocking(map, GURL("about:blank"));
  EXPECT_EQ(false, setting);
}

// Should not do domain blocking unless ad blocking is "aggressive"
TEST_F(adrbrowsielShieldsUtilTest, ShouldDoDomainBlocking_ControlTypes) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  GURL url = GURL("https://adrbrowsiel.com");

  adrbrowsiel_shields::SetAdControlType(map, ControlType::ALLOW, url);
  auto setting = adrbrowsiel_shields::ShouldDoDomainBlocking(map, url);
  EXPECT_EQ(false, setting);

  adrbrowsiel_shields::SetAdControlType(map, ControlType::BLOCK, url);
  setting = adrbrowsiel_shields::ShouldDoDomainBlocking(map, url);
  EXPECT_EQ(false, setting);

  adrbrowsiel_shields::SetCosmeticFilteringControlType(map, ControlType::BLOCK, url);
  setting = adrbrowsiel_shields::ShouldDoDomainBlocking(map, url);
  EXPECT_EQ(true, setting);
}
