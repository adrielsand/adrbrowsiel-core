/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_ADS_BROWSER_NOTIFICATION_HELPER_LINUX_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_ADS_BROWSER_NOTIFICATION_HELPER_LINUX_H_

#include "base/memory/weak_ptr.h"

#include "adrbrowsiel/components/adrbrowsiel_ads/browser/notification_helper.h"

namespace adrbrowsiel_ads {

class NotificationHelperLinux
    : public NotificationHelper,
      public base::SupportsWeakPtr<NotificationHelperLinux> {
 public:
  NotificationHelperLinux(const NotificationHelperLinux&) = delete;
  NotificationHelperLinux& operator=(const NotificationHelperLinux&) = delete;

  static NotificationHelperLinux* GetInstanceImpl();

 private:
  friend struct base::DefaultSingletonTraits<NotificationHelperLinux>;

  NotificationHelperLinux();
  ~NotificationHelperLinux() override;

  // NotificationHelper impl
  bool ShouldShowNotifications() override;

  bool ShowMyFirstAdNotification() override;

  bool CanShowBackgroundNotifications() const override;
};

}  // namespace adrbrowsiel_ads

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_ADS_BROWSER_NOTIFICATION_HELPER_LINUX_H_
