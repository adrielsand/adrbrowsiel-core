/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CATALOG_CATALOG_CREATIVE_AD_NOTIFICATION_INFO_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CATALOG_CATALOG_CREATIVE_AD_NOTIFICATION_INFO_H_

#include <vector>

#include "bat/ads/internal/catalog/catalog_ad_notification_payload_info.h"
#include "bat/ads/internal/catalog/catalog_creative_info.h"

namespace ads {

struct CatalogCreativeAdNotificationInfo : CatalogCreativeInfo {
  CatalogCreativeAdNotificationInfo();
  CatalogCreativeAdNotificationInfo(
      const CatalogCreativeAdNotificationInfo& info);
  ~CatalogCreativeAdNotificationInfo();

  bool operator==(const CatalogCreativeAdNotificationInfo& rhs) const;
  bool operator!=(const CatalogCreativeAdNotificationInfo& rhs) const;

  CatalogAdNotificationPayloadInfo payload;
};

using CatalogCreativeAdNotificationList =
    std::vector<CatalogCreativeAdNotificationInfo>;

}  // namespace ads

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CATALOG_CATALOG_CREATIVE_AD_NOTIFICATION_INFO_H_
