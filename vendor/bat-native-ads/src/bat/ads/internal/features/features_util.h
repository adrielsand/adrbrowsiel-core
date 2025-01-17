/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_FEATURES_FEATURES_UTIL_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_FEATURES_FEATURES_UTIL_H_

#include <string>

#include "base/feature_list.h"
#include "base/time/time.h"

namespace ads {

std::string GetFieldTrialParamByFeatureAsString(
    const base::Feature& feature,
    const std::string& param_name,
    const std::string& default_value);

base::TimeDelta GetFieldTrialParamByFeatureAsTimeDelta(
    const base::Feature& feature,
    const std::string& param_name,
    const base::TimeDelta& default_value);

}  // namespace ads

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_FEATURES_FEATURES_UTIL_H_
