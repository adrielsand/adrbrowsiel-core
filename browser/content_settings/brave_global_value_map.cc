/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/content_settings/adrbrowsiel_global_value_map.h"

#include <memory>

#include "adrbrowsiel/components/content_settings/core/browser/adrbrowsiel_content_settings_utils.h"
#include "components/content_settings/core/browser/content_settings_rule.h"
#include "components/content_settings/core/common/content_settings.h"

namespace content_settings {

adrbrowsielGlobalValueMap::adrbrowsielGlobalValueMap() {}

adrbrowsielGlobalValueMap::~adrbrowsielGlobalValueMap() {}

std::unique_ptr<RuleIterator> adrbrowsielGlobalValueMap::GetRuleIterator(
    ContentSettingsType content_type) const {
  if (content_settings::IsShieldsContentSettingsType(content_type))
    return nullptr;
  return GlobalValueMap::GetRuleIterator(content_type);
}

void adrbrowsielGlobalValueMap::SetContentSetting(ContentSettingsType content_type,
                                            ContentSetting setting) {
  return GlobalValueMap::SetContentSetting(content_type, setting);
}

ContentSetting adrbrowsielGlobalValueMap::GetContentSetting(
    ContentSettingsType content_type) const {
  return GlobalValueMap::GetContentSetting(content_type);
}

}  // namespace content_settings
