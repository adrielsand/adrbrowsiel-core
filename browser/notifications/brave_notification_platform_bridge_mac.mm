/* Copyright (c) 2019 The adrbrowsiel Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/notifications/adrbrowsiel_notification_platform_bridge.h"

#include <memory>

#include "adrbrowsiel/browser/notifications/adrbrowsiel_alert_dispatcher_mac.h"
#include "chrome/browser/notifications/notification_platform_bridge_mac.h"
#include "ui/message_center/public/cpp/notification.h"
#include "ui/message_center/public/cpp/notification_types.h"

@class NSUserNotificationCenter;

// static
std::unique_ptr<NotificationPlatformBridge>
adrbrowsielNotificationPlatformBridge::Create() {
  base::scoped_nsobject<adrbrowsielAlertDispatcherMac> alert_dispatcher(
      [[adrbrowsielAlertDispatcherMac alloc] init]);
  return std::make_unique<NotificationPlatformBridgeMac>(
      [NSUserNotificationCenter defaultUserNotificationCenter],
          alert_dispatcher.get());
}
