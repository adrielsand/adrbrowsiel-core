/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/ads/brave_news_ads/brave_news_ad_builder.h"

#include "base/guid.h"
#include "bat/ads/brave_news_ad_info.h"
#include "bat/ads/internal/bundle/creative_brave_news_ad_info.h"

namespace ads {

BraveNewsAdInfo BuildBraveNewsAd(
    const CreativeBraveNewsAdInfo& creative_brave_news_ad) {
  const std::string uuid = base::GenerateGUID();
  return BuildBraveNewsAd(creative_brave_news_ad, uuid);
}

BraveNewsAdInfo BuildBraveNewsAd(
    const CreativeBraveNewsAdInfo& creative_brave_news_ad,
    const std::string& uuid) {
  BraveNewsAdInfo brave_news_ad;

  brave_news_ad.type = AdType::kBraveNewsAd;
  brave_news_ad.uuid = uuid;
  brave_news_ad.creative_instance_id =
      creative_brave_news_ad.creative_instance_id;
  brave_news_ad.creative_set_id = creative_brave_news_ad.creative_set_id;
  brave_news_ad.campaign_id = creative_brave_news_ad.campaign_id;
  brave_news_ad.advertiser_id = creative_brave_news_ad.advertiser_id;
  brave_news_ad.segment = creative_brave_news_ad.segment;
  brave_news_ad.title = creative_brave_news_ad.title;
  brave_news_ad.description = creative_brave_news_ad.description;
  brave_news_ad.image_url = creative_brave_news_ad.image_url;
  brave_news_ad.size = creative_brave_news_ad.size;
  brave_news_ad.target_url = creative_brave_news_ad.target_url;

  return brave_news_ad;
}

}  // namespace ads
