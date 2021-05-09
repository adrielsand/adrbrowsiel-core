/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CATALOG_CATALOG_CREATIVE_BRAVE_NEWS_AD_INFO_H_
#define BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CATALOG_CATALOG_CREATIVE_BRAVE_NEWS_AD_INFO_H_

#include <vector>

#include "bat/ads/internal/catalog/catalog_brave_news_ad_payload_info.h"
#include "bat/ads/internal/catalog/catalog_creative_info.h"

namespace ads {

struct CatalogCreativeBraveNewsAdInfo : CatalogCreativeInfo {
  CatalogCreativeBraveNewsAdInfo();
  CatalogCreativeBraveNewsAdInfo(const CatalogCreativeBraveNewsAdInfo& info);
  ~CatalogCreativeBraveNewsAdInfo();

  bool operator==(const CatalogCreativeBraveNewsAdInfo& rhs) const;
  bool operator!=(const CatalogCreativeBraveNewsAdInfo& rhs) const;

  CatalogBraveNewsAdPayloadInfo payload;
};

using CatalogCreativeBraveNewsAdList =
    std::vector<CatalogCreativeBraveNewsAdInfo>;

}  // namespace ads

#endif  // BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CATALOG_CATALOG_CREATIVE_BRAVE_NEWS_AD_INFO_H_
