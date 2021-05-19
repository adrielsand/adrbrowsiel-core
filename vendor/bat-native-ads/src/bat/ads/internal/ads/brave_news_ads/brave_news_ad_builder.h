/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_BRAVE_NEWS_ADS_BRAVE_NEWS_AD_BUILDER_H_
#define BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_BRAVE_NEWS_ADS_BRAVE_NEWS_AD_BUILDER_H_

#include <string>

namespace ads {

struct BraveNewsAdInfo;
struct CreativeBraveNewsAdInfo;

BraveNewsAdInfo BuildBraveNewsAd(
    const CreativeBraveNewsAdInfo& creative_ad_notification);

BraveNewsAdInfo BuildBraveNewsAd(
    const CreativeBraveNewsAdInfo& creative_ad_notification,
    const std::string& uuid);

}  // namespace ads

#endif  // BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_BRAVE_NEWS_ADS_BRAVE_NEWS_AD_BUILDER_H_
