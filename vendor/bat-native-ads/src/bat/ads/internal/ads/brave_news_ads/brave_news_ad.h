/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_BRAVE_NEWS_ADS_BRAVE_NEWS_AD_H_
#define BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_BRAVE_NEWS_ADS_BRAVE_NEWS_AD_H_

#include <string>

#include "bat/ads/internal/ad_events/ad_event_info.h"
#include "bat/ads/internal/ads/brave_news_ads/brave_news_ad_observer.h"
#include "bat/ads/mojom.h"

namespace ads {

struct BraveNewsAdInfo;

class BraveNewsAd : public BraveNewsAdObserver {
 public:
  BraveNewsAd();

  ~BraveNewsAd() override;

  void AddObserver(BraveNewsAdObserver* observer);
  void RemoveObserver(BraveNewsAdObserver* observer);

  void FireEvent(const std::string& uuid,
                 const std::string& creative_instance_id,
                 const BraveNewsAdEventType event_type);

 private:
  base::ObserverList<BraveNewsAdObserver> observers_;

  void FireEvent(const BraveNewsAdInfo& ad,
                 const std::string& uuid,
                 const std::string& creative_instance_id,
                 const BraveNewsAdEventType event_type);

  void NotifyBraveNewsAdEvent(const BraveNewsAdInfo& ad,
                              const BraveNewsAdEventType event_type);

  void NotifyBraveNewsAdViewed(const BraveNewsAdInfo& ad);
  void NotifyBraveNewsAdClicked(const BraveNewsAdInfo& ad);

  void NotifyBraveNewsAdEventFailed(const std::string& uuid,
                                    const std::string& creative_instance_id,
                                    const BraveNewsAdEventType event_type);
};

}  // namespace ads

#endif  // BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_BRAVE_NEWS_ADS_BRAVE_NEWS_AD_H_
