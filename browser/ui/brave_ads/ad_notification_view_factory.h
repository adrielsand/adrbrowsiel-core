/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_adrbrowsiel_ADS_AD_NOTIFICATION_VIEW_FACTORY_H_
#define adrbrowsiel_BROWSER_UI_adrbrowsiel_ADS_AD_NOTIFICATION_VIEW_FACTORY_H_

#include <memory>

namespace adrbrowsiel_ads {

class AdNotification;
class AdNotificationView;

// Creates an AdNotificationView depending on the notification type. A
// notification is top level if it needs to be rendered outside the browser
// window
class AdNotificationViewFactory {
 public:
  static std::unique_ptr<AdNotificationView> Create(
      const AdNotification& ad_notification);
};

}  // namespace adrbrowsiel_ads

#endif  // adrbrowsiel_BROWSER_UI_adrbrowsiel_ADS_AD_NOTIFICATION_VIEW_FACTORY_H_
