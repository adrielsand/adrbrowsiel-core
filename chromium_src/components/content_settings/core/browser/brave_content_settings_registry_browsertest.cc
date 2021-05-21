/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/adrbrowsiel_shields/common/adrbrowsiel_shield_constants.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/content_settings/core/common/content_settings_pattern.h"
#include "components/content_settings/core/common/content_settings_types.h"
#include "content/public/test/browser_test.h"

const GURL& GetadrbrowsielURL() {
  static const GURL kadrbrowsielURL("https://www.adrbrowsiel.com");
  return kadrbrowsielURL;
}

class adrbrowsielContentSettingsRegistryBrowserTest : public InProcessBrowserTest {
 public:
  using InProcessBrowserTest::InProcessBrowserTest;

  HostContentSettingsMap* content_settings() {
    return HostContentSettingsMapFactory::GetForProfile(browser()->profile());
  }

  HostContentSettingsMap* private_content_settings() {
    return HostContentSettingsMapFactory::GetForProfile(
        browser()->profile()->GetPrimaryOTRProfile());
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(adrbrowsielContentSettingsRegistryBrowserTest);
};

IN_PROC_BROWSER_TEST_F(adrbrowsielContentSettingsRegistryBrowserTest,
                       WithoutWildcardContentSetting) {
  ContentSetting adrbrowsiel_url_shields_setting =
      content_settings()->GetContentSetting(GetadrbrowsielURL(), GetadrbrowsielURL(),
                                            ContentSettingsType::adrbrowsiel_SHIELDS);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, adrbrowsiel_url_shields_setting);

  ContentSetting adrbrowsiel_url_shields_setting_private =
      private_content_settings()->GetContentSetting(
          GetadrbrowsielURL(), GetadrbrowsielURL(), ContentSettingsType::adrbrowsiel_SHIELDS);
  EXPECT_EQ(CONTENT_SETTING_DEFAULT, adrbrowsiel_url_shields_setting_private);
}

IN_PROC_BROWSER_TEST_F(adrbrowsielContentSettingsRegistryBrowserTest,
                       WithadrbrowsielShieldsContentSetting) {
  ContentSettingsPattern adrbrowsiel_url_pattern =
      ContentSettingsPattern::FromURL(GetadrbrowsielURL());

  content_settings()->SetContentSettingCustomScope(
      adrbrowsiel_url_pattern, adrbrowsiel_url_pattern, ContentSettingsType::adrbrowsiel_SHIELDS,
      CONTENT_SETTING_ALLOW);

  ContentSetting adrbrowsiel_url_shields_setting =
      content_settings()->GetContentSetting(GetadrbrowsielURL(), GetadrbrowsielURL(),
                                            ContentSettingsType::adrbrowsiel_SHIELDS);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, adrbrowsiel_url_shields_setting);

  ContentSetting adrbrowsiel_url_shields_setting_private =
      private_content_settings()->GetContentSetting(
          GetadrbrowsielURL(), GetadrbrowsielURL(), ContentSettingsType::adrbrowsiel_SHIELDS);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, adrbrowsiel_url_shields_setting_private);
}
