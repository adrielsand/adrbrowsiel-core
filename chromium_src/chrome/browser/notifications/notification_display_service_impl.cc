// Copyright 2019 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "adrbrowsiel/browser/notifications/ads_notification_handler.h"

#define adrbrowsiel_ADD_adrbrowsiel_ADS_NOTIFICATION_HANDLER \
  AddNotificationHandler(                        \
      NotificationHandler::Type::adrbrowsiel_ADS,      \
      std::make_unique<adrbrowsiel_ads::AdsNotificationHandler>(profile));
#include "../../../../../chrome/browser/notifications/notification_display_service_impl.cc"
#undef adrbrowsiel_ADD_adrbrowsiel_ADS_NOTIFICATION_HANDLER
