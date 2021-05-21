/*  Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>
#include <utility>

#include "base/macros.h"
#include "base/optional.h"
#include "base/values.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/common/adrbrowsiel_shield_constants.h"
#include "adrbrowsiel/components/content_settings/core/browser/adrbrowsiel_content_settings_pref_provider.h"
#include "adrbrowsiel/components/content_settings/core/browser/adrbrowsiel_content_settings_utils.h"
#include "chrome/test/base/testing_profile.h"
#include "components/content_settings/core/browser/content_settings_registry.h"
#include "components/content_settings/core/common/content_settings.h"
#include "components/content_settings/core/common/content_settings_pattern.h"
#include "components/content_settings/core/common/content_settings_types.h"
#include "components/content_settings/core/common/content_settings_utils.h"
#include "components/content_settings/core/test/content_settings_test_utils.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_task_environment.h"
#include "services/preferences/public/cpp/dictionary_value_update.h"
#include "services/preferences/public/cpp/scoped_pref_update.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"

namespace content_settings {

namespace {

constexpr char kUserProfilePluginsPath[] =
    "profile.content_settings.exceptions.plugins";

const char kExpirationPath[] = "expiration";
const char kLastModifiedPath[] = "last_modified";
const char kSessionModelPath[] = "model";
const char kSettingPath[] = "setting";
const char kPerResourcePath[] = "per_resource";

using GURLSourcePair = std::pair<GURL, ContentSettingsType>;

ContentSettingsPattern SecondaryUrlToPattern(const GURL& gurl) {
  CHECK(gurl == GURL() || gurl == GURL("https://firstParty/*"));
  if (gurl == GURL())
    return ContentSettingsPattern::Wildcard();
  else
    return ContentSettingsPattern::FromString("https://firstParty/*");
}

std::unique_ptr<prefs::DictionaryValueUpdate>
InitializeCommonSettingsAndGetPerResourceDictionary(
    prefs::DictionaryValueUpdate* dictionary,
    const base::Time& last_modified_time) {
  const uint64_t last_modified_time_in_ms =
      last_modified_time.ToDeltaSinceWindowsEpoch().InMicroseconds();

  dictionary->SetInteger(kExpirationPath, 0);
  dictionary->SetString(kLastModifiedPath,
                        base::NumberToString(last_modified_time_in_ms));
  dictionary->SetInteger(
      kSessionModelPath,
      static_cast<int>(content_settings::SessionModel::Durable));

  return dictionary->SetDictionaryWithoutPathExpansion(
      kPerResourcePath, std::make_unique<base::DictionaryValue>());
}

void InitializeAllShieldSettingsInDictionary(
    prefs::DictionaryValueUpdate* dictionary,
    const base::Time& last_modified_time,
    int value) {
  std::unique_ptr<prefs::DictionaryValueUpdate> per_resource_dict =
      InitializeCommonSettingsAndGetPerResourceDictionary(dictionary,
                                                          last_modified_time);
  per_resource_dict->SetInteger(adrbrowsiel_shields::kAds, value);
  per_resource_dict->SetInteger(adrbrowsiel_shields::kCookies, value);
  per_resource_dict->SetInteger(adrbrowsiel_shields::kCosmeticFiltering, value);
  per_resource_dict->SetInteger(adrbrowsiel_shields::kFingerprintingV2, value);
  per_resource_dict->SetInteger(adrbrowsiel_shields::kHTTPUpgradableResources, value);
  per_resource_dict->SetInteger(adrbrowsiel_shields::kReferrers, value);
  per_resource_dict->SetInteger(adrbrowsiel_shields::kTrackers, value);
}

void InitializeadrbrowsielShieldsSettingInDictionary(
    prefs::DictionaryValueUpdate* dictionary,
    const base::Time& last_modified_time,
    int value) {
  std::unique_ptr<prefs::DictionaryValueUpdate> per_resource_dict =
      InitializeCommonSettingsAndGetPerResourceDictionary(dictionary,
                                                          last_modified_time);
  per_resource_dict->SetInteger(adrbrowsiel_shields::kadrbrowsielShields, value);
}

void InitializeUnsupportedShieldSettingInDictionary(
    prefs::DictionaryValueUpdate* dictionary,
    const base::Time& last_modified_time) {
  std::unique_ptr<prefs::DictionaryValueUpdate> per_resource_dict =
      InitializeCommonSettingsAndGetPerResourceDictionary(dictionary,
                                                          last_modified_time);
  per_resource_dict->SetInteger("unknown_setting", 1);
}

void CheckMigrationFromResourceIdentifierForDictionary(
    const base::DictionaryValue* dictionary,
    const std::string& patterns_string,
    const base::Time& expected_last_modified,
    int expected_setting_value) {
  const base::DictionaryValue* settings_dict = nullptr;
  dictionary->GetDictionaryWithoutPathExpansion(patterns_string,
                                                &settings_dict);
  EXPECT_NE(settings_dict, nullptr);

  int actual_value;
  settings_dict->GetInteger(kSettingPath, &actual_value);
  EXPECT_EQ(GetTimeStampFromDictionary(settings_dict, kLastModifiedPath),
            expected_last_modified);
  EXPECT_EQ(GetSessionModelFromDictionary(settings_dict, kSessionModelPath),
            content_settings::SessionModel::Durable);
  EXPECT_EQ(actual_value, expected_setting_value);
}

class ShieldsSetting {
 public:
  ShieldsSetting(adrbrowsielPrefProvider* provider,
                 const std::vector<GURLSourcePair> urls)
      : provider_(provider), urls_(urls) {}
  virtual ~ShieldsSetting() = default;

  virtual void SetPreMigrationSettings(const ContentSettingsPattern& pattern,
                                       ContentSetting setting) {
    for (const auto& url_source : urls_) {
      provider_->SetWebsiteSetting(
          pattern, SecondaryUrlToPattern(url_source.first), url_source.second,
          ContentSettingToValue(setting), {});
    }
  }

  void CheckSettingsAreDefault(const GURL& url) const {
    CheckSettings(url, CONTENT_SETTING_DEFAULT);
  }

  void CheckSettingsWouldBlock(const GURL& url) const {
    CheckSettings(url, CONTENT_SETTING_BLOCK);
  }

  void CheckSettingsWouldAllow(const GURL& url) const {
    CheckSettings(url, CONTENT_SETTING_ALLOW);
  }

 protected:
  virtual void CheckSettings(const GURL& url, ContentSetting setting) const {
    for (const auto& url_source : urls_) {
      EXPECT_EQ(setting,
                TestUtils::GetContentSetting(provider_, url, url_source.first,
                                             url_source.second, false));
    }
  }

  adrbrowsielPrefProvider* provider_;
  const std::vector<GURLSourcePair> urls_;
};

class ShieldsCookieSetting : public ShieldsSetting {
 public:
  explicit ShieldsCookieSetting(adrbrowsielPrefProvider* provider)
      : ShieldsSetting(
            provider,
            {{GURL(), ContentSettingsType::adrbrowsiel_COOKIES},
             {GURL("https://firstParty/*"), ContentSettingsType::adrbrowsiel_COOKIES},
             {GURL(), ContentSettingsType::adrbrowsiel_REFERRERS}}) {}
};

class ShieldsFingerprintingSetting : public ShieldsSetting {
 public:
  explicit ShieldsFingerprintingSetting(adrbrowsielPrefProvider* provider)
      : ShieldsSetting(provider,
                       {{GURL(), ContentSettingsType::adrbrowsiel_FINGERPRINTING_V2},
                        {GURL("https://firstParty/*"),
                         ContentSettingsType::adrbrowsiel_FINGERPRINTING_V2}}) {}
};

class ShieldsHTTPSESetting : public ShieldsSetting {
 public:
  explicit ShieldsHTTPSESetting(adrbrowsielPrefProvider* provider)
      : ShieldsSetting(
            provider,
            {{GURL(), ContentSettingsType::adrbrowsiel_HTTP_UPGRADABLE_RESOURCES}}) {}
};

class ShieldsAdsSetting : public ShieldsSetting {
 public:
  explicit ShieldsAdsSetting(adrbrowsielPrefProvider* provider)
      : ShieldsSetting(provider,
                       {{GURL(), ContentSettingsType::adrbrowsiel_ADS},
                        {GURL(), ContentSettingsType::adrbrowsiel_TRACKERS}}) {}
};

class ShieldsEnabledSetting : public ShieldsSetting {
 public:
  explicit ShieldsEnabledSetting(adrbrowsielPrefProvider* provider)
      : ShieldsSetting(provider,
                       {{GURL(), ContentSettingsType::adrbrowsiel_SHIELDS}}) {}
};

class ShieldsScriptSetting : public ShieldsSetting {
 public:
  explicit ShieldsScriptSetting(adrbrowsielPrefProvider* provider)
      : ShieldsSetting(provider, {}) {}

  void SetPreMigrationSettings(const ContentSettingsPattern& pattern,
                               ContentSetting setting) override {
    provider_->SetWebsiteSetting(pattern, ContentSettingsPattern::Wildcard(),
                                 ContentSettingsType::JAVASCRIPT,
                                 ContentSettingToValue(setting), {});
  }

 private:
  void CheckSettings(const GURL& url, ContentSetting setting) const override {
    EXPECT_EQ(setting, TestUtils::GetContentSetting(
                           provider_, url, GURL(),
                           ContentSettingsType::JAVASCRIPT, false));
  }
};

}  // namespace

class adrbrowsielPrefProviderTest : public testing::Test {
 public:
  adrbrowsielPrefProviderTest() {
    // Ensure all content settings are initialized.
    ContentSettingsRegistry::GetInstance();
  }

  void SetUp() override {
    testing::Test::SetUp();
    testing_profile_ = TestingProfile::Builder().Build();
  }

  void TearDown() override { testing_profile_.reset(); }

  TestingProfile* testing_profile() { return testing_profile_.get(); }

 private:
  content::BrowserTaskEnvironment task_environment_;
  std::unique_ptr<TestingProfile> testing_profile_;
};

TEST_F(adrbrowsielPrefProviderTest, TestShieldsSettingsMigration) {
  adrbrowsielPrefProvider provider(
      testing_profile()->GetPrefs(), false /* incognito */,
      true /* store_last_modified */, false /* restore_session */);

  ShieldsCookieSetting cookie_settings(&provider);
  ShieldsFingerprintingSetting fp_settings(&provider);
  ShieldsHTTPSESetting httpse_settings(&provider);
  ShieldsAdsSetting ads_settings(&provider);
  ShieldsEnabledSetting enabled_settings(&provider);
  ShieldsScriptSetting script_settings(&provider);

  GURL url("http://adrbrowsiel.com:8080/");
  GURL url2("http://allowed.adrbrowsiel.com:3030");
  // Check that the settings for the url are default values.
  cookie_settings.CheckSettingsAreDefault(url);
  cookie_settings.CheckSettingsAreDefault(url2);
  fp_settings.CheckSettingsAreDefault(url);
  httpse_settings.CheckSettingsAreDefault(url);
  ads_settings.CheckSettingsAreDefault(url);
  enabled_settings.CheckSettingsAreDefault(url);
  script_settings.CheckSettingsAreDefault(url);

  // Set pre-migrtion patterns different from defaults.
  // ------------------------------------------------------
  ContentSettingsPattern pattern = ContentSettingsPattern::FromURL(url);
  ContentSettingsPattern pattern2 = ContentSettingsPattern::FromURL(url2);
  // Cookies.
  cookie_settings.SetPreMigrationSettings(pattern, CONTENT_SETTING_BLOCK);
  cookie_settings.SetPreMigrationSettings(pattern2, CONTENT_SETTING_ALLOW);
  // Pattern that doesn't need to be migrated.
  cookie_settings.SetPreMigrationSettings(
      ContentSettingsPattern::FromString("*://help.adrbrowsiel.com/*"),
      CONTENT_SETTING_BLOCK);
  // Check that settings would block adrbrowsiel.com:8080, but not adrbrowsiel.com:5555.
  cookie_settings.CheckSettingsWouldBlock(url);
  cookie_settings.CheckSettingsWouldAllow(url2);
  cookie_settings.CheckSettingsAreDefault(GURL("http://adrbrowsiel.com:5555"));

  // Finterprinting.
  fp_settings.SetPreMigrationSettings(pattern, CONTENT_SETTING_ALLOW);
  // Check that settings would allow adrbrowsiel.com:8080, but not adrbrowsiel.com:5555.
  fp_settings.CheckSettingsWouldAllow(url);
  fp_settings.CheckSettingsAreDefault(GURL("http://adrbrowsiel.com:5555"));

  // HTTPSE.
  httpse_settings.SetPreMigrationSettings(pattern, CONTENT_SETTING_BLOCK);
  // Check that settings would block adrbrowsiel.com:8080, but not adrbrowsiel.com:5555.
  httpse_settings.CheckSettingsWouldBlock(url);
  httpse_settings.CheckSettingsAreDefault(GURL("http://adrbrowsiel.com:5555"));

  // Ads.
  ads_settings.SetPreMigrationSettings(pattern, CONTENT_SETTING_ALLOW);
  // Check that settings would allow adrbrowsiel.com:8080, but not adrbrowsiel.com:5555.
  ads_settings.CheckSettingsWouldAllow(url);
  ads_settings.CheckSettingsAreDefault(GURL("http://adrbrowsiel.com:5555"));

  // Enabled.
  enabled_settings.SetPreMigrationSettings(pattern, CONTENT_SETTING_BLOCK);
  // Check that settings would block adrbrowsiel.com:8080, but not adrbrowsiel.com:5555.
  httpse_settings.CheckSettingsWouldBlock(url);
  httpse_settings.CheckSettingsAreDefault(GURL("http://adrbrowsiel.com:5555"));

  // Scripts.
  script_settings.SetPreMigrationSettings(pattern, CONTENT_SETTING_BLOCK);
  // Check that settings would block adrbrowsiel.com:8080, but not adrbrowsiel.com:5555.
  script_settings.CheckSettingsWouldBlock(url);
  script_settings.CheckSettingsAreDefault(GURL("http://adrbrowsiel.com:5555"));

  // Migrate settings.
  // ------------------------------------------------------
  testing_profile()->GetPrefs()->SetInteger(kadrbrowsielShieldsSettingsVersion, 1);
  provider.MigrateShieldsSettings(/*incognito*/ false);

  // Check post-migration settings.
  // ------------------------------------------------------
  // Cookies.
  // Check that settings would block adrbrowsiel.com with any protocol and port.
  cookie_settings.CheckSettingsWouldBlock(url);
  cookie_settings.CheckSettingsWouldBlock(GURL("http://adrbrowsiel.com:5555"));
  cookie_settings.CheckSettingsWouldBlock(GURL("https://adrbrowsiel.com"));
  // Check that settings would allow allow.adrbrowsiel.com with any protocol and port.
  cookie_settings.CheckSettingsWouldAllow(url2);
  cookie_settings.CheckSettingsWouldAllow(GURL("https://allowed.adrbrowsiel.com"));
  // Check the pattern that didn't need to be migrated.
  cookie_settings.CheckSettingsWouldBlock(
      GURL("https://help.adrbrowsiel.com/article1.html"));
  // Would not block a different domain.
  cookie_settings.CheckSettingsAreDefault(GURL("http://adrbrowsiel2.com"));

  // Fingerprinting.
  // Check that settings would allow adrbrowsiel.com with any protocol and port.
  fp_settings.CheckSettingsWouldAllow(url);
  fp_settings.CheckSettingsWouldAllow(GURL("http://adrbrowsiel.com:5555"));
  fp_settings.CheckSettingsWouldAllow(GURL("https://adrbrowsiel.com"));
  // Would not allow a different domain.
  fp_settings.CheckSettingsAreDefault(GURL("http://adrbrowsiel2.com"));

  // HTTPSE.
  // Check that settings would block adrbrowsiel.com with any protocol and port.
  httpse_settings.CheckSettingsWouldBlock(url);
  httpse_settings.CheckSettingsWouldBlock(GURL("http://adrbrowsiel.com:5555"));
  // Would not block a different domain.
  httpse_settings.CheckSettingsAreDefault(GURL("http://adrbrowsiel2.com"));

  // Ads.
  // Check that settings would allow adrbrowsiel.com with any protocol and port.
  ads_settings.CheckSettingsWouldAllow(url);
  ads_settings.CheckSettingsWouldAllow(GURL("http://adrbrowsiel.com:5555"));
  ads_settings.CheckSettingsWouldAllow(GURL("https://adrbrowsiel.com"));
  // Would not allow a different domain.
  ads_settings.CheckSettingsAreDefault(GURL("http://adrbrowsiel2.com"));

  // Enabled.
  // Check that settings would block adrbrowsiel.com with any protocol and port.
  httpse_settings.CheckSettingsWouldBlock(url);
  httpse_settings.CheckSettingsWouldBlock(GURL("http://adrbrowsiel.com:5555"));
  httpse_settings.CheckSettingsWouldBlock(GURL("https://adrbrowsiel.com"));
  // Would not block a different domain.
  httpse_settings.CheckSettingsAreDefault(GURL("http://adrbrowsiel2.com"));

  // Scripts.
  // Check that settings would block adrbrowsiel.com with any protocol and port.
  script_settings.CheckSettingsWouldBlock(url);
  script_settings.CheckSettingsWouldBlock(GURL("http://adrbrowsiel.com:5555"));
  script_settings.CheckSettingsWouldBlock(GURL("https://adrbrowsiel.com"));
  // Would not block a different domain.
  script_settings.CheckSettingsAreDefault(GURL("http://adrbrowsiel2.com"));

  provider.ShutdownOnUIThread();
}

TEST_F(adrbrowsielPrefProviderTest, TestShieldsSettingsMigrationVersion) {
  PrefService* prefs = testing_profile()->GetPrefs();
  adrbrowsielPrefProvider provider(prefs, false /* incognito */,
                             true /* store_last_modified */,
                             false /* restore_session */);

  // Should have migrated when constructed (with profile).
  EXPECT_EQ(2, prefs->GetInteger(kadrbrowsielShieldsSettingsVersion));

  // Reset and check that migration runs.
  prefs->SetInteger(kadrbrowsielShieldsSettingsVersion, 1);
  provider.MigrateShieldsSettings(/*incognito*/ false);
  EXPECT_EQ(2, prefs->GetInteger(kadrbrowsielShieldsSettingsVersion));

  // Test that migration doesn't run for another version.
  prefs->SetInteger(kadrbrowsielShieldsSettingsVersion, 5);
  provider.MigrateShieldsSettings(/*incognito*/ false);
  EXPECT_EQ(5, prefs->GetInteger(kadrbrowsielShieldsSettingsVersion));

  provider.ShutdownOnUIThread();
}

TEST_F(adrbrowsielPrefProviderTest, TestShieldsSettingsMigrationFromResourceIDs) {
  PrefService* pref_service = testing_profile()->GetPrefs();
  adrbrowsielPrefProvider provider(pref_service, false /* incognito */,
                             true /* store_last_modified */,
                             false /* restore_session */);

  // Manually write settings under the PLUGINS type using the no longer existing
  // ResourceIdentifier names, and then perform the migration.
  prefs::ScopedDictionaryPrefUpdate plugins_pref_update(
      pref_service, kUserProfilePluginsPath);
  std::unique_ptr<prefs::DictionaryValueUpdate> plugins_dictionary =
      plugins_pref_update.Get();
  EXPECT_NE(plugins_dictionary, nullptr);

  base::Time expected_last_modified = base::Time::Now();

  // Seed global shield settings with non-default values.
  std::unique_ptr<prefs::DictionaryValueUpdate> global_settings_dict =
      plugins_dictionary->SetDictionaryWithoutPathExpansion(
          "*,*", std::make_unique<base::DictionaryValue>());

  const int expected_global_settings_value = 1;
  InitializeAllShieldSettingsInDictionary(global_settings_dict.get(),
                                          expected_last_modified,
                                          expected_global_settings_value);

  // Change all of those global settings for www.example.com.
  std::unique_ptr<prefs::DictionaryValueUpdate> example_settings_dict =
      plugins_dictionary->SetDictionaryWithoutPathExpansion(
          "www.example.com,*", std::make_unique<base::DictionaryValue>());

  const int expected_example_com_settings_value = 1;
  InitializeAllShieldSettingsInDictionary(example_settings_dict.get(),
                                          expected_last_modified,
                                          expected_example_com_settings_value);

  // Disable adrbrowsiel Shields for www.adrbrowsiel.com.
  std::unique_ptr<prefs::DictionaryValueUpdate> adrbrowsiel_settings_dict =
      plugins_dictionary->SetDictionaryWithoutPathExpansion(
          "www.adrbrowsiel.com,*", std::make_unique<base::DictionaryValue>());

  const int expected_adrbrowsiel_com_settings_value = 1;
  InitializeadrbrowsielShieldsSettingInDictionary(adrbrowsiel_settings_dict.get(),
                                            expected_last_modified,
                                            expected_adrbrowsiel_com_settings_value);

  provider.MigrateShieldsSettingsFromResourceIds();

  // Check migration for all the settings has been properly done.
  for (auto content_type : GetShieldsContentSettingsTypes()) {
    const base::DictionaryValue* adrbrowsiel_shields_dict =
        pref_service->GetDictionary(GetShieldsSettingUserPrefsPath(
            GetShieldsContentTypeName(content_type)));
    EXPECT_NE(adrbrowsiel_shields_dict, nullptr);

    if (content_type == ContentSettingsType::adrbrowsiel_SHIELDS) {
      // We only changed the value of adrbrowsiel_SHIELDS in www.adrbrowsiel.com.
      CheckMigrationFromResourceIdentifierForDictionary(
          adrbrowsiel_shields_dict, "www.adrbrowsiel.com,*", expected_last_modified,
          expected_adrbrowsiel_com_settings_value);
    } else {
      // All the other settings we changed them globally and in www.example.com.
      CheckMigrationFromResourceIdentifierForDictionary(
          adrbrowsiel_shields_dict, "*,*", expected_last_modified,
          expected_global_settings_value);
      CheckMigrationFromResourceIdentifierForDictionary(
          adrbrowsiel_shields_dict, "www.example.com,*", expected_last_modified,
          expected_example_com_settings_value);
    }
  }

  provider.ShutdownOnUIThread();
}

TEST_F(adrbrowsielPrefProviderTest, TestShieldsSettingsMigrationFromUnknownSettings) {
  PrefService* pref_service = testing_profile()->GetPrefs();
  adrbrowsielPrefProvider provider(pref_service, false /* incognito */,
                             true /* store_last_modified */,
                             false /* restore_session */);

  // Manually write invalid settings under the PLUGINS type using the no longer
  // existing ResourceIdentifier names, to attempt the migration.
  prefs::ScopedDictionaryPrefUpdate plugins_pref_update(
      pref_service, kUserProfilePluginsPath);
  std::unique_ptr<prefs::DictionaryValueUpdate> plugins_dictionary =
      plugins_pref_update.Get();
  EXPECT_NE(plugins_dictionary, nullptr);

  // Seed both global and per-site shield settings preferences using unsupported
  // names, so that we can test that adrbrowsiel doesn't crash while attempting the
  // migration and simply ignore those unsupported names instead.
  //
  // For a list of supported names, see |kadrbrowsielContentSettingstypes| inside the
  // components/content_settings/core/browser/content_settings_registry.cc
  // override, in the chromium_src/ directory.
  std::unique_ptr<prefs::DictionaryValueUpdate> global_settings_dict =
      plugins_dictionary->SetDictionaryWithoutPathExpansion(
          "*,*", std::make_unique<base::DictionaryValue>());
  InitializeUnsupportedShieldSettingInDictionary(global_settings_dict.get(),
                                                 base::Time::Now());
  std::unique_ptr<prefs::DictionaryValueUpdate> example_settings_dict =
      plugins_dictionary->SetDictionaryWithoutPathExpansion(
          "www.example.com,*", std::make_unique<base::DictionaryValue>());
  InitializeUnsupportedShieldSettingInDictionary(example_settings_dict.get(),
                                                 base::Time::Now());

  // Doing the migration below should NOT get a crash due to invalid settings.
  provider.MigrateShieldsSettingsFromResourceIds();

  // New Shields-specific content settings types should have been created due to
  // the migration, but all should be empty since only invalid data was fed.
  for (auto content_type : GetShieldsContentSettingsTypes()) {
    const base::DictionaryValue* adrbrowsiel_shields_dict =
        pref_service->GetDictionary(GetShieldsSettingUserPrefsPath(
            GetShieldsContentTypeName(content_type)));
    EXPECT_NE(adrbrowsiel_shields_dict, nullptr);
    EXPECT_TRUE(adrbrowsiel_shields_dict->empty());
  }

  provider.ShutdownOnUIThread();
}

}  //  namespace content_settings
