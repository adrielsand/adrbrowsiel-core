/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/feature_list.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/common/adrbrowsiel_shield_constants.h"

#define adrbrowsiel_GET_RENDER_CONTENT_SETTING_RULES                             \
  map->GetSettingsForOneType(ContentSettingsType::AUTOPLAY,                \
                             &(rules->autoplay_rules));                    \
  map->GetSettingsForOneType(ContentSettingsType::adrbrowsiel_FINGERPRINTING_V2, \
                             &(rules->fingerprinting_rules));              \
  map->GetSettingsForOneType(ContentSettingsType::adrbrowsiel_SHIELDS,           \
                             &(rules->adrbrowsiel_shields_rules));

#include "../../../../../../components/content_settings/core/browser/content_settings_utils.cc"

#undef adrbrowsiel_adrbrowsiel_GET_RENDER_CONTENT_SETTING_RULES
