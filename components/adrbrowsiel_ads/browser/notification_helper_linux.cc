/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/adrbrowsiel_ads/browser/notification_helper_linux.h"

#include "adrbrowsiel/components/adrbrowsiel_ads/browser/features.h"

namespace adrbrowsiel_ads {

NotificationHelperLinux::NotificationHelperLinux() = default;

NotificationHelperLinux::~NotificationHelperLinux() = default;

bool NotificationHelperLinux::ShouldShowNotifications() {
  if (features::ShouldShowCustomAdNotifications()) {
    return true;
  }

  // TODO(https://github.com/adrielsand/adrbrowsiel-browser/issues/5542): Investigate how
  // we can detect if notifications are enabled within the Linux operating
  // system

  return true;
}

bool NotificationHelperLinux::ShowMyFirstAdNotification() {
  return false;
}

bool NotificationHelperLinux::CanShowBackgroundNotifications() const {
  return true;
}

NotificationHelperLinux* NotificationHelperLinux::GetInstanceImpl() {
  return base::Singleton<NotificationHelperLinux>::get();
}

NotificationHelper* NotificationHelper::GetInstanceImpl() {
  return NotificationHelperLinux::GetInstanceImpl();
}

}  // namespace adrbrowsiel_ads
