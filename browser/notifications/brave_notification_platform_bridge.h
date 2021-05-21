/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_NOTIFICATIONS_adrbrowsiel_NOTIFICATION_PLATFORM_BRIDGE_H_
#define adrbrowsiel_BROWSER_NOTIFICATIONS_adrbrowsiel_NOTIFICATION_PLATFORM_BRIDGE_H_

#include <memory>

#include "base/macros.h"

class NotificationPlatformBridge;

class adrbrowsielNotificationPlatformBridge {
 public:
  static std::unique_ptr<NotificationPlatformBridge> Create();

 private:
  adrbrowsielNotificationPlatformBridge() = default;
  ~adrbrowsielNotificationPlatformBridge() = default;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielNotificationPlatformBridge);
};

#endif  // adrbrowsiel_BROWSER_NOTIFICATIONS_adrbrowsiel_NOTIFICATION_PLATFORM_BRIDGE_H_
