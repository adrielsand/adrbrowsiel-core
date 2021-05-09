/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/ad_events/brave_news_ads/brave_news_ad_event_clicked.h"

#include "bat/ads/brave_news_ad_info.h"
#include "bat/ads/confirmation_type.h"
#include "bat/ads/internal/ad_events/ad_events.h"
#include "bat/ads/internal/ads_history/ads_history.h"
#include "bat/ads/internal/logging.h"

namespace ads {
namespace brave_news_ads {

AdEventClicked::AdEventClicked() = default;

AdEventClicked::~AdEventClicked() = default;

void AdEventClicked::FireEvent(const BraveNewsAdInfo& ad) {
  BLOG(3, "Clicked Brave News ad with uuid " << ad.uuid
                                             << " and creative instance id "
                                             << ad.creative_instance_id);

  LogAdEvent(ad, ConfirmationType::kClicked, [](const Result result) {
    if (result != Result::SUCCESS) {
      BLOG(1, "Failed to log Brave News ad clicked event");
      return;
    }

    BLOG(6, "Successfully logged Brave News ad clicked event");
  });

  history::AddBraveNewsAd(ad, ConfirmationType::kClicked);
}

}  // namespace brave_news_ads
}  // namespace ads
