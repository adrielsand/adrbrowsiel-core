/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CATALOG_CATALOG_CREATIVE_INFO_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CATALOG_CATALOG_CREATIVE_INFO_H_

#include <string>

#include "bat/ads/internal/catalog/catalog_type_info.h"

namespace ads {

struct CatalogCreativeInfo {
  CatalogCreativeInfo();
  CatalogCreativeInfo(const CatalogCreativeInfo& info);
  ~CatalogCreativeInfo();

  bool operator==(const CatalogCreativeInfo& rhs) const;
  bool operator!=(const CatalogCreativeInfo& rhs) const;

  std::string creative_instance_id;
  CatalogTypeInfo type;
};

}  // namespace ads

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CATALOG_CATALOG_CREATIVE_INFO_H_
