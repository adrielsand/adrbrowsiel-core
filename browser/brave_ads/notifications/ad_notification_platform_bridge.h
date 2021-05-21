/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_adrbrowsiel_ADS_NOTIFICATIONS_AD_NOTIFICATION_PLATFORM_BRIDGE_H_
#define adrbrowsiel_BROWSER_adrbrowsiel_ADS_NOTIFICATIONS_AD_NOTIFICATION_PLATFORM_BRIDGE_H_

#include <string>

#include "adrbrowsiel/browser/profiles/profile_util.h"
#include "adrbrowsiel/browser/ui/adrbrowsiel_ads/ad_notification.h"

namespace adrbrowsiel_ads {

class AdNotificationPlatformBridge {
 public:
  explicit AdNotificationPlatformBridge(Profile* profile);
  ~AdNotificationPlatformBridge();

  void ShowAdNotification(AdNotification ad_notification);
  void CloseAdNotification(const std::string& notification_id);

 private:
  Profile* profile_ = nullptr;  // NOT OWNED

  AdNotificationPlatformBridge(const AdNotificationPlatformBridge&) = delete;
  AdNotificationPlatformBridge& operator=(const AdNotificationPlatformBridge&) =
      delete;
};

}  // namespace adrbrowsiel_ads

#endif  // adrbrowsiel_BROWSER_adrbrowsiel_ADS_NOTIFICATIONS_AD_NOTIFICATION_PLATFORM_BRIDGE_H_
