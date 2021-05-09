/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_BUNDLE_CREATIVE_BRAVE_NEWS_AD_INFO_H_
#define BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_BUNDLE_CREATIVE_BRAVE_NEWS_AD_INFO_H_

#include <string>
#include <vector>

#include "bat/ads/internal/bundle/creative_ad_info.h"

namespace ads {

struct CreativeBraveNewsAdInfo : CreativeAdInfo {
  CreativeBraveNewsAdInfo();
  CreativeBraveNewsAdInfo(const CreativeBraveNewsAdInfo& info);
  ~CreativeBraveNewsAdInfo();

  bool operator==(const CreativeBraveNewsAdInfo& rhs) const;

  bool operator!=(const CreativeBraveNewsAdInfo& rhs) const;

  std::string title;
  std::string description;
  std::string image_url;
  std::string size;
};

using CreativeBraveNewsAdList = std::vector<CreativeBraveNewsAdInfo>;

}  // namespace ads

#endif  // BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_BUNDLE_CREATIVE_BRAVE_NEWS_AD_INFO_H_
