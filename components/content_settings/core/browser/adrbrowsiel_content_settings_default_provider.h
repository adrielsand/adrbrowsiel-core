/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_CONTENT_SETTINGS_CORE_BROWSER_adrbrowsiel_CONTENT_SETTINGS_DEFAULT_PROVIDER_H_
#define adrbrowsiel_COMPONENTS_CONTENT_SETTINGS_CORE_BROWSER_adrbrowsiel_CONTENT_SETTINGS_DEFAULT_PROVIDER_H_

#include <memory>

#include "base/macros.h"
#include "base/synchronization/lock.h"
#include "components/content_settings/core/browser/content_settings_default_provider.h"
#include "components/content_settings/core/browser/content_settings_rule.h"
#include "components/content_settings/core/common/content_settings_types.h"

class PrefService;

namespace content_settings {

class adrbrowsielDefaultProvider : public DefaultProvider {
 public:
  adrbrowsielDefaultProvider(PrefService* prefs, bool off_the_record);
  ~adrbrowsielDefaultProvider() override;

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  // ProviderInterface implementations.
  std::unique_ptr<RuleIterator> GetRuleIterator(
      ContentSettingsType content_type,
      bool off_the_record) const override;

 private:
  DISALLOW_COPY_AND_ASSIGN(adrbrowsielDefaultProvider);
};

}  // namespace content_settings

#endif  // adrbrowsiel_COMPONENTS_CONTENT_SETTINGS_CORE_BROWSER_adrbrowsiel_CONTENT_SETTINGS_DEFAULT_PROVIDER_H_
