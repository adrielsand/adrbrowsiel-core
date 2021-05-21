/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// Leave a gap between Chromium values and our values in the kHistogramValue
// array so that we don't have to renumber when new content settings types are
// added upstream.
namespace {

// Do not change the value arbitrarily. This variable is only used for the
// DCHECK in ContentSettingTypeToHistogramValue function below.
constexpr int kadrbrowsielValuesStart = 1000;

constexpr int adrbrowsiel_value(int incr) {
  return kadrbrowsielValuesStart + incr;
}

}  // namespace

// clang-format off
#define adrbrowsiel_HISTOGRAM_VALUE_LIST                                        \
  {ContentSettingsType::adrbrowsiel_ADS, adrbrowsiel_value(0)},                       \
  {ContentSettingsType::adrbrowsiel_COSMETIC_FILTERING, adrbrowsiel_value(1)},        \
  {ContentSettingsType::adrbrowsiel_TRACKERS, adrbrowsiel_value(2)},                  \
  {ContentSettingsType::adrbrowsiel_HTTP_UPGRADABLE_RESOURCES, adrbrowsiel_value(3)}, \
  {ContentSettingsType::adrbrowsiel_FINGERPRINTING_V2, adrbrowsiel_value(4)},         \
  {ContentSettingsType::adrbrowsiel_SHIELDS, adrbrowsiel_value(5)},                   \
  {ContentSettingsType::adrbrowsiel_REFERRERS, adrbrowsiel_value(6)},                 \
  {ContentSettingsType::adrbrowsiel_COOKIES, adrbrowsiel_value(7)},
// clang-format on

#define adrbrowsiel_IS_RENDERER_CONTENT_SETTING \
  content_type == ContentSettingsType::AUTOPLAY ||

#define ContentSettingTypeToHistogramValue \
  ContentSettingTypeToHistogramValue_ChromiumImpl

#include "../../../../../../components/content_settings/core/common/content_settings.cc"

#undef ContentSettingTypeToHistogramValue
#undef adrbrowsiel_HISTOGRAM_VALUE_LIST
#undef adrbrowsiel_IS_RENDERER_CONTENT_SETTING

int ContentSettingTypeToHistogramValue(ContentSettingsType content_setting,
                                       size_t* num_values) {
  DCHECK(static_cast<int>(ContentSettingsType::NUM_TYPES) < kadrbrowsielValuesStart);
  return ContentSettingTypeToHistogramValue_ChromiumImpl(content_setting,
                                                         num_values);
}
