/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CATALOG_CATALOG_ISSUER_INFO_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CATALOG_CATALOG_ISSUER_INFO_H_

#include <string>
#include <vector>

namespace ads {

struct CatalogIssuerInfo {
  CatalogIssuerInfo();
  CatalogIssuerInfo(const CatalogIssuerInfo& info);
  ~CatalogIssuerInfo();

  bool operator==(const CatalogIssuerInfo& rhs) const;
  bool operator!=(const CatalogIssuerInfo& rhs) const;

  std::string name;
  std::string public_key;
};

using CatalogIssuerList = std::vector<CatalogIssuerInfo>;

}  // namespace ads

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CATALOG_CATALOG_ISSUER_INFO_H_
