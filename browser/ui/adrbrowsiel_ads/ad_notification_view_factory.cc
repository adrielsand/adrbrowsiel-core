/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/adrbrowsiel_ads/ad_notification_view_factory.h"

#include "adrbrowsiel/browser/ui/adrbrowsiel_ads/text_ad_notification_view.h"

namespace adrbrowsiel_ads {

// static
std::unique_ptr<AdNotificationView> AdNotificationViewFactory::Create(
    const AdNotification& ad_notification) {
  return std::make_unique<TextAdNotificationView>(ad_notification);
}

}  // namespace adrbrowsiel_ads
