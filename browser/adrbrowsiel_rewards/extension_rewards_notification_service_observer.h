/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_adrbrowsiel_REWARDS_EXTENSION_REWARDS_NOTIFICATION_SERVICE_OBSERVER_H_
#define adrbrowsiel_BROWSER_adrbrowsiel_REWARDS_EXTENSION_REWARDS_NOTIFICATION_SERVICE_OBSERVER_H_

#include "base/macros.h"
#include "bat/ledger/mojom_structs.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/rewards_notification_service_observer.h"

class Profile;

namespace adrbrowsiel_rewards {

class RewardsNotificationService;

class ExtensionRewardsNotificationServiceObserver
    : public RewardsNotificationServiceObserver {
 public:
  explicit ExtensionRewardsNotificationServiceObserver(Profile* profile);
  ~ExtensionRewardsNotificationServiceObserver() override;

  // RewardsNotificationServiceObserver implementation
  void OnNotificationAdded(
      RewardsNotificationService* rewards_notification_service,
      const RewardsNotificationService::RewardsNotification&
          rewards_notification) override;
  void OnNotificationDeleted(
      RewardsNotificationService* rewards_notification_service,
      const RewardsNotificationService::RewardsNotification&
          rewards_notification) override;
  void OnAllNotificationsDeleted(
      RewardsNotificationService* rewards_notification_service) override;
  void OnGetNotification(
      RewardsNotificationService* rewards_notification_service,
      const RewardsNotificationService::RewardsNotification&
          rewards_notification) override;
  void OnGetAllNotifications(
      RewardsNotificationService* rewards_notification_service,
      const RewardsNotificationService::RewardsNotificationsList&
          rewards_notifications_list) override;

 private:
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionRewardsNotificationServiceObserver);
};

}  // namespace adrbrowsiel_rewards

#endif  // adrbrowsiel_BROWSER_adrbrowsiel_REWARDS_EXTENSION_REWARDS_NOTIFICATION_SERVICE_OBSERVER_H_
