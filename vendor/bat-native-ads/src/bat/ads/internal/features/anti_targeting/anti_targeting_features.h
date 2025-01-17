/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_FEATURES_ANTI_TARGETING_ANTI_TARGETING_FEATURES_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_FEATURES_ANTI_TARGETING_ANTI_TARGETING_FEATURES_H_

#include <cstdint>

#include "base/feature_list.h"

namespace ads {
namespace features {

extern const base::Feature kAntiTargeting;

bool IsAntiTargetingEnabled();

int GetAntiTargetingResourceVersion();

}  // namespace features
}  // namespace ads

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_FEATURES_ANTI_TARGETING_ANTI_TARGETING_FEATURES_H_
