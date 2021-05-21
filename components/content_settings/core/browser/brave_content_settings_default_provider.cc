/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/content_settings/core/browser/adrbrowsiel_content_settings_default_provider.h"

#include "adrbrowsiel/components/content_settings/core/browser/adrbrowsiel_content_settings_utils.h"
#include "build/build_config.h"
#include "components/prefs/pref_service.h"

namespace content_settings {

// static
void adrbrowsielDefaultProvider::RegisterProfilePrefs(
    user_prefs::PrefRegistrySyncable* registry) {
  DefaultProvider::RegisterProfilePrefs(registry);
}

adrbrowsielDefaultProvider::adrbrowsielDefaultProvider(PrefService* prefs,
                                           bool off_the_record)
    : DefaultProvider(prefs, off_the_record) {}

adrbrowsielDefaultProvider::~adrbrowsielDefaultProvider() {}

std::unique_ptr<RuleIterator> adrbrowsielDefaultProvider::GetRuleIterator(
    ContentSettingsType content_type,
    bool off_the_record) const {
  if (content_settings::IsShieldsContentSettingsType(content_type))
    return nullptr;

  return DefaultProvider::GetRuleIterator(content_type, off_the_record);
}

}  // namespace content_settings
