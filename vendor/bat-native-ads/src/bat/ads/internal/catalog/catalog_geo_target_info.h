/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CATALOG_CATALOG_GEO_TARGET_INFO_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CATALOG_CATALOG_GEO_TARGET_INFO_H_

#include <string>
#include <vector>

namespace ads {

struct CatalogGeoTargetInfo {
  CatalogGeoTargetInfo();
  CatalogGeoTargetInfo(const CatalogGeoTargetInfo& info);
  ~CatalogGeoTargetInfo();

  bool operator==(const CatalogGeoTargetInfo& rhs) const;
  bool operator!=(const CatalogGeoTargetInfo& rhs) const;

  std::string code;
  std::string name;
};

using CatalogGeoTargetList = std::vector<CatalogGeoTargetInfo>;

}  // namespace ads

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CATALOG_CATALOG_GEO_TARGET_INFO_H_
