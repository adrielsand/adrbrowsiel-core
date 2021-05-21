/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_CONTENT_SETTINGS_adrbrowsiel_GLOBAL_VALUE_MAP_H_
#define adrbrowsiel_BROWSER_CONTENT_SETTINGS_adrbrowsiel_GLOBAL_VALUE_MAP_H_

#include <memory>

#include "components/content_settings/core/browser/content_settings_global_value_map.h"
#include "components/content_settings/core/browser/content_settings_provider.h"
#include "components/content_settings/core/common/content_settings_types.h"

namespace content_settings {

class RuleIterator;

class adrbrowsielGlobalValueMap : public GlobalValueMap {
 public:
  adrbrowsielGlobalValueMap();
  ~adrbrowsielGlobalValueMap();

  // Returns nullptr to indicate the RuleIterator is empty.
  std::unique_ptr<RuleIterator> GetRuleIterator(
      ContentSettingsType content_type) const;
  void SetContentSetting(ContentSettingsType content_type,
                         ContentSetting setting);
  ContentSetting GetContentSetting(ContentSettingsType content_type) const;

 private:
  DISALLOW_COPY_AND_ASSIGN(adrbrowsielGlobalValueMap);
};

}  // namespace content_settings

#endif  // adrbrowsiel_BROWSER_CONTENT_SETTINGS_adrbrowsiel_GLOBAL_VALUE_MAP_H_
