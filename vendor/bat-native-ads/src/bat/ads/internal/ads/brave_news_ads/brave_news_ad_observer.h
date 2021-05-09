/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_BRAVE_NEWS_ADS_BRAVE_NEWS_AD_OBSERVER_H_
#define BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_BRAVE_NEWS_ADS_BRAVE_NEWS_AD_OBSERVER_H_

#include <string>

#include "base/observer_list.h"
#include "bat/ads/mojom.h"

namespace ads {

struct BraveNewsAdInfo;

class BraveNewsAdObserver : public base::CheckedObserver {
 public:
  // Invoked when a Brave News ad is viewed
  virtual void OnBraveNewsAdViewed(const BraveNewsAdInfo& ad) {}

  // Invoked when a Brave News ad is clicked
  virtual void OnBraveNewsAdClicked(const BraveNewsAdInfo& ad) {}

  // Invoked when a Brave News ad event fails
  virtual void OnBraveNewsAdEventFailed(const std::string& uuid,
                                        const std::string& creative_instance_id,
                                        const BraveNewsAdEventType event_type) {
  }

 protected:
  ~BraveNewsAdObserver() override = default;
};

}  // namespace ads

#endif  // BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_BRAVE_NEWS_ADS_BRAVE_NEWS_AD_OBSERVER_H_
