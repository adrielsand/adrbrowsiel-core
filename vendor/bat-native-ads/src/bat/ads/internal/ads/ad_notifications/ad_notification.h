/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_AD_NOTIFICATIONS_AD_NOTIFICATION_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_AD_NOTIFICATIONS_AD_NOTIFICATION_H_

#include <string>

#include "bat/ads/internal/ads/ad_notifications/ad_notification_observer.h"
#include "bat/ads/mojom.h"

namespace ads {

struct AdNotificationInfo;

class AdNotification : public AdNotificationObserver {
 public:
  AdNotification();

  ~AdNotification() override;

  void AddObserver(AdNotificationObserver* observer);
  void RemoveObserver(AdNotificationObserver* observer);

  void FireEvent(const std::string& uuid,
                 const AdNotificationEventType event_type);

 private:
  base::ObserverList<AdNotificationObserver> observers_;

  void NotifyAdNotificationEvent(const AdNotificationInfo& ad,
                                 const AdNotificationEventType event_type);

  void NotifyAdNotificationViewed(const AdNotificationInfo& ad);
  void NotifyAdNotificationClicked(const AdNotificationInfo& ad);
  void NotifyAdNotificationDismissed(const AdNotificationInfo& ad);
  void NotifyAdNotificationTimedOut(const AdNotificationInfo& ad);

  void NotifyAdNotificationEventFailed(
      const std::string& uuid,
      const AdNotificationEventType event_type);
};

}  // namespace ads

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ADS_AD_NOTIFICATIONS_AD_NOTIFICATION_H_
