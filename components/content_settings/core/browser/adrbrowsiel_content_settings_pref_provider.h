/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_CONTENT_SETTINGS_CORE_BROWSER_adrbrowsiel_CONTENT_SETTINGS_PREF_PROVIDER_H_
#define adrbrowsiel_COMPONENTS_CONTENT_SETTINGS_CORE_BROWSER_adrbrowsiel_CONTENT_SETTINGS_PREF_PROVIDER_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "base/memory/weak_ptr.h"
#include "components/content_settings/core/browser/content_settings_observer.h"
#include "components/content_settings/core/browser/content_settings_pref_provider.h"
#include "components/prefs/pref_change_registrar.h"

namespace content_settings {

// With this subclass, shields configuration is persisted across sessions.
class adrbrowsielPrefProvider : public PrefProvider,
                          public Observer {
 public:
  adrbrowsielPrefProvider(PrefService* prefs,
                    bool off_the_record,
                    bool store_last_modified,
                    bool restore_session);
  ~adrbrowsielPrefProvider() override;

  static void CopyPluginSettingsForMigration(PrefService* prefs);
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  // content_settings::PrefProvider overrides:
  void ShutdownOnUIThread() override;
  bool SetWebsiteSetting(const ContentSettingsPattern& primary_pattern,
                         const ContentSettingsPattern& secondary_pattern,
                         ContentSettingsType content_type,
                         std::unique_ptr<base::Value>&& value,
                         const ContentSettingConstraints& constraints) override;
  std::unique_ptr<RuleIterator> GetRuleIterator(
      ContentSettingsType content_type,
      bool incognito) const override;

 private:
  friend class adrbrowsielPrefProviderTest;
  FRIEND_TEST_ALL_PREFIXES(adrbrowsielPrefProviderTest, TestShieldsSettingsMigration);
  FRIEND_TEST_ALL_PREFIXES(adrbrowsielPrefProviderTest,
                           TestShieldsSettingsMigrationVersion);
  FRIEND_TEST_ALL_PREFIXES(adrbrowsielPrefProviderTest,
                           TestShieldsSettingsMigrationFromResourceIDs);
  FRIEND_TEST_ALL_PREFIXES(adrbrowsielPrefProviderTest,
                           TestShieldsSettingsMigrationFromUnknownSettings);
  void MigrateShieldsSettings(bool incognito);
  void MigrateShieldsSettingsFromResourceIds();
  void MigrateShieldsSettingsFromResourceIdsForOneType(
      const std::string& preference_path,
      const std::string& patterns_string,
      const base::Time& expiration,
      const base::Time& last_modified,
      SessionModel session_model,
      int setting);
  void MigrateShieldsSettingsV1ToV2();
  void MigrateShieldsSettingsV1ToV2ForOneType(ContentSettingsType content_type);
  void UpdateCookieRules(ContentSettingsType content_type, bool incognito);
  void OnCookieSettingsChanged(ContentSettingsType content_type);
  void NotifyChanges(const std::vector<Rule>& rules, bool incognito);
  bool SetWebsiteSettingInternal(
      const ContentSettingsPattern& primary_pattern,
      const ContentSettingsPattern& secondary_pattern,
      ContentSettingsType content_type,
      std::unique_ptr<base::Value>&& in_value,
      const ContentSettingConstraints& constraints);

  // content_settings::Observer overrides:
  void OnContentSettingChanged(const ContentSettingsPattern& primary_pattern,
                               const ContentSettingsPattern& secondary_pattern,
                               ContentSettingsType content_type) override;
  void OnCookiePrefsChanged(const std::string& pref);

  std::map<bool /* is_incognito */, std::vector<Rule>> cookie_rules_;
  std::map<bool /* is_incognito */, std::vector<Rule>> adrbrowsiel_cookie_rules_;

  bool initialized_;
  bool store_last_modified_;
  base::WeakPtrFactory<adrbrowsielPrefProvider> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielPrefProvider);
};

}  //  namespace content_settings

#endif  // adrbrowsiel_COMPONENTS_CONTENT_SETTINGS_CORE_BROWSER_adrbrowsiel_CONTENT_SETTINGS_PREF_PROVIDER_H_
