/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_AD_EVENTS_NEW_TAB_PAGE_ADS_NEW_TAB_PAGE_AD_EVENT_FACTORY_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_AD_EVENTS_NEW_TAB_PAGE_ADS_NEW_TAB_PAGE_AD_EVENT_FACTORY_H_

#include <memory>

#include "bat/ads/internal/ad_events/ad_event.h"
#include "bat/ads/mojom.h"

namespace ads {

struct NewTabPageAdInfo;

namespace new_tab_page_ads {

class AdEventFactory {
 public:
  static std::unique_ptr<AdEvent<NewTabPageAdInfo>> Build(
      const NewTabPageAdEventType event_type);
};

}  // namespace new_tab_page_ads
}  // namespace ads

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_AD_EVENTS_NEW_TAB_PAGE_ADS_NEW_TAB_PAGE_AD_EVENT_FACTORY_H_
