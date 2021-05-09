/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/ads/brave_news_ads/brave_news_ad.h"

#include "bat/ads/brave_news_ad_info.h"
#include "bat/ads/internal/ad_events/brave_news_ads/brave_news_ad_event_factory.h"
#include "bat/ads/internal/ads/brave_news_ads/brave_news_ad_frequency_capping.h"
#include "bat/ads/internal/bundle/creative_brave_news_ad_info.h"
#include "bat/ads/internal/database/tables/ad_events_database_table.h"
#include "bat/ads/internal/database/tables/creative_brave_news_ads_database_table.h"
#include "bat/ads/internal/logging.h"

namespace ads {

namespace {

BraveNewsAdInfo CreateBraveNewsAd(const std::string& uuid,
                                  const CreativeBraveNewsAdInfo& ad) {
  BraveNewsAdInfo brave_news_ad;

  brave_news_ad.type = AdType::kBraveNewsAd;
  brave_news_ad.uuid = uuid;
  brave_news_ad.creative_instance_id = ad.creative_instance_id;
  brave_news_ad.creative_set_id = ad.creative_set_id;
  brave_news_ad.campaign_id = ad.campaign_id;
  brave_news_ad.advertiser_id = ad.advertiser_id;
  brave_news_ad.segment = ad.segment;
  brave_news_ad.target_url = ad.target_url;
  brave_news_ad.title = ad.title;
  brave_news_ad.description = ad.description;

  return brave_news_ad;
}

}  // namespace

BraveNewsAd::BraveNewsAd() = default;

BraveNewsAd::~BraveNewsAd() = default;

void BraveNewsAd::AddObserver(BraveNewsAdObserver* observer) {
  DCHECK(observer);
  observers_.AddObserver(observer);
}

void BraveNewsAd::RemoveObserver(BraveNewsAdObserver* observer) {
  DCHECK(observer);
  observers_.RemoveObserver(observer);
}

void BraveNewsAd::FireEvent(const std::string& uuid,
                            const std::string& creative_instance_id,
                            const BraveNewsAdEventType event_type) {
  if (uuid.empty() || creative_instance_id.empty()) {
    BLOG(1, "Failed to fire Brave News ad event for uuid "
                << uuid << " and creative instance id "
                << creative_instance_id);

    NotifyBraveNewsAdEventFailed(uuid, creative_instance_id, event_type);

    return;
  }

  database::table::CreativeBraveNewsAds database_table;
  database_table.GetForCreativeInstanceId(
      creative_instance_id,
      [=](const Result result, const std::string& creative_instance_id,
          const CreativeBraveNewsAdInfo& creative_brave_news_ad) {
        if (result != SUCCESS) {
          BLOG(1, "Failed to fire Brave News ad event for uuid");

          NotifyBraveNewsAdEventFailed(uuid, creative_instance_id, event_type);

          return;
        }

        const BraveNewsAdInfo ad =
            CreateBraveNewsAd(uuid, creative_brave_news_ad);

        FireEvent(ad, uuid, creative_instance_id, event_type);
      });
}

///////////////////////////////////////////////////////////////////////////////

// bool BraveNewsAd::ShouldFireEvent(const BraveNewsAdInfo& ad,
//                                    const AdEventList& ad_events) {
//   brave_news_ads::FrequencyCapping frequency_capping(ad_events);

//   if (!frequency_capping.IsAdAllowed()) {
//     return false;
//   }

//   if (frequency_capping.ShouldExcludeAd(ad)) {
//     return false;
//   }

//   return true;
// }

void BraveNewsAd::FireEvent(const BraveNewsAdInfo& ad,
                            const std::string& uuid,
                            const std::string& creative_instance_id,
                            const BraveNewsAdEventType event_type) {
  database::table::AdEvents database_table;
  database_table.GetAll([=](const Result result, const AdEventList& ad_events) {
    if (result != Result::SUCCESS) {
      BLOG(1, "Brave News ad: Failed to get ad events");

      NotifyBraveNewsAdEventFailed(uuid, creative_instance_id, event_type);

      return;
    }

    // TODO(tmancey): Move this check to the frequency cap
    // if (event_type == BraveNewsAdEventType::kViewed &&
    //     !ShouldFireEvent(ad, ad_events)) {
    //   BLOG(1, "Brave News ad: Not allowed");

    //   NotifyBraveNewsAdEventFailed(uuid, creative_instance_id, event_type);

    //   return;
    // }

    const auto ad_event = brave_news_ads::AdEventFactory::Build(event_type);
    ad_event->FireEvent(ad);

    NotifyBraveNewsAdEvent(ad, event_type);
  });
}

void BraveNewsAd::NotifyBraveNewsAdEvent(
    const BraveNewsAdInfo& ad,
    const BraveNewsAdEventType event_type) {
  switch (event_type) {
    case BraveNewsAdEventType::kViewed: {
      NotifyBraveNewsAdViewed(ad);
      break;
    }

    case BraveNewsAdEventType::kClicked: {
      NotifyBraveNewsAdClicked(ad);
      break;
    }
  }
}

void BraveNewsAd::NotifyBraveNewsAdViewed(const BraveNewsAdInfo& ad) {
  for (BraveNewsAdObserver& observer : observers_) {
    observer.OnBraveNewsAdViewed(ad);
  }
}

void BraveNewsAd::NotifyBraveNewsAdClicked(const BraveNewsAdInfo& ad) {
  for (BraveNewsAdObserver& observer : observers_) {
    observer.OnBraveNewsAdClicked(ad);
  }
}

void BraveNewsAd::NotifyBraveNewsAdEventFailed(
    const std::string& uuid,
    const std::string& creative_instance_id,
    const BraveNewsAdEventType event_type) {
  for (BraveNewsAdObserver& observer : observers_) {
    observer.OnBraveNewsAdEventFailed(uuid, creative_instance_id, event_type);
  }
}

}  // namespace ads
