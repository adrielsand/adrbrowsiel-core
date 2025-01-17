/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/settings/settings.h"

#include "base/numerics/ranges.h"
#include "bat/ads/internal/ads_client_helper.h"
#include "bat/ads/internal/features/ad_serving/ad_serving_features.h"
#include "bat/ads/pref_names.h"

namespace ads {
namespace settings {

uint64_t GetAdsPerHour() {
  uint64_t ads_per_hour =
      AdsClientHelper::Get()->GetUint64Pref(prefs::kAdsPerHour);

  if (ads_per_hour == 0) {
    ads_per_hour =
        static_cast<uint64_t>(features::GetDefaultAdNotificationsPerHour());
  }

  return base::ClampToRange(
      ads_per_hour, static_cast<uint64_t>(kMinimumAdNotificationsPerHour),
      static_cast<uint64_t>(kMaximumAdNotificationsPerHour));
}

}  // namespace settings
}  // namespace ads
