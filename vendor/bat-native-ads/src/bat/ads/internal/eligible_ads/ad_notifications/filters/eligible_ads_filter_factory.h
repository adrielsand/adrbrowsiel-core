/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ELIGIBLE_ADS_AD_NOTIFICATIONS_FILTERS_ELIGIBLE_ADS_FILTER_FACTORY_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ELIGIBLE_ADS_AD_NOTIFICATIONS_FILTERS_ELIGIBLE_ADS_FILTER_FACTORY_H_

#include <memory>

#include "bat/ads/internal/eligible_ads/ad_notifications/filters/eligible_ads_filter.h"

namespace ads {

class EligibleAdsFilterFactory {
 public:
  static std::unique_ptr<EligibleAdsFilter> Build(
      const EligibleAdsFilter::Type type);
};

}  // namespace ads

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ELIGIBLE_ADS_AD_NOTIFICATIONS_FILTERS_ELIGIBLE_ADS_FILTER_FACTORY_H_
