/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CLIENT_PREFERENCES_FILTERED_AD_INFO_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CLIENT_PREFERENCES_FILTERED_AD_INFO_H_

#include <string>
#include <vector>

#include "bat/ads/result.h"

namespace ads {

struct FilteredAdInfo {
  FilteredAdInfo();
  FilteredAdInfo(const FilteredAdInfo& ad);
  ~FilteredAdInfo();

  std::string ToJson() const;
  Result FromJson(const std::string& json);

  std::string creative_instance_id;
  std::string creative_set_id;
};

using FilteredAdList = std::vector<FilteredAdInfo>;

}  // namespace ads

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CLIENT_PREFERENCES_FILTERED_AD_INFO_H_
