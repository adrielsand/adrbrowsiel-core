/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CATALOG_CATALOG_DAYPART_INFO_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CATALOG_CATALOG_DAYPART_INFO_H_

#include <string>
#include <vector>

#include "base/time/time.h"

namespace ads {

struct CatalogDaypartInfo {
  CatalogDaypartInfo();
  CatalogDaypartInfo(const CatalogDaypartInfo& info);
  ~CatalogDaypartInfo();

  bool operator==(const CatalogDaypartInfo& rhs) const;
  bool operator!=(const CatalogDaypartInfo& rhs) const;

  std::string dow = "0123456";
  int start_minute = 0;
  int end_minute = (base::Time::kMinutesPerHour * base::Time::kHoursPerDay) - 1;
};

using CatalogDaypartList = std::vector<CatalogDaypartInfo>;

}  // namespace ads

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_CATALOG_CATALOG_DAYPART_INFO_H_
