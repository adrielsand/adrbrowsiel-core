/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/ad_events/brave_news_ads/brave_news_ad_event_factory.h"

#include "bat/ads/brave_news_ad_info.h"
#include "bat/ads/internal/ad_events/brave_news_ads/brave_news_ad_event_clicked.h"
#include "bat/ads/internal/ad_events/brave_news_ads/brave_news_ad_event_viewed.h"

namespace ads {
namespace brave_news_ads {

std::unique_ptr<AdEvent<BraveNewsAdInfo>> AdEventFactory::Build(
    const BraveNewsAdEventType event_type) {
  switch (event_type) {
    case BraveNewsAdEventType::kViewed: {
      return std::make_unique<AdEventViewed>();
    }

    case BraveNewsAdEventType::kClicked: {
      return std::make_unique<AdEventClicked>();
    }
  }
}

}  // namespace brave_news_ads
}  // namespace ads
