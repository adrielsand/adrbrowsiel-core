/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_REWARDS_BROWSER_REWARDS_NOTIFICATION_SERVICE_IMPL_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_REWARDS_BROWSER_REWARDS_NOTIFICATION_SERVICE_IMPL_H_

#include <memory>
#include <string>
#include <vector>

#include "base/values.h"
#include "bat/ledger/mojom_structs.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/rewards_notification_service.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/rewards_notification_service_observer.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/rewards_service_observer.h"
#include "extensions/buildflags/buildflags.h"

class Profile;

namespace adrbrowsiel_rewards {

class ExtensionRewardsNotificationServiceObserver;

class RewardsNotificationServiceImpl
    : public RewardsNotificationService,
      public RewardsServiceObserver,
      public base::SupportsWeakPtr<RewardsNotificationServiceImpl> {
 public:
  explicit RewardsNotificationServiceImpl(Profile* profile);
  ~RewardsNotificationServiceImpl() override;

  void Init(
      std::unique_ptr<RewardsNotificationServiceObserver> extension_observer);
  void AddNotification(RewardsNotificationType type,
                       RewardsNotificationArgs args,
                       RewardsNotificationID id = "",
                       bool only_once = false) override;
  void DeleteNotification(RewardsNotificationID id) override;
  void DeleteAllNotifications(const bool delete_displayed) override;
  void GetNotification(RewardsNotificationID id) override;
  void GetNotifications() override;
  const RewardsNotificationsMap& GetAllNotifications() const override;

  void ReadRewardsNotificationsJSON() override;
  void ReadRewardsNotifications(base::Value::ConstListView root);
  void StoreRewardsNotifications() override;

  bool Exists(RewardsNotificationID id) const override;

 private:
  bool IsAds(const ledger::type::PromotionType promotion_type);
  std::string GetPromotionIdPrefix(
      const ledger::type::PromotionType promotion_type);

  // RewardsServiceObserver impl
  void OnFetchPromotions(
      RewardsService* rewards_service,
      const ledger::type::Result result,
      const ledger::type::PromotionList& list) override;
  void OnPromotionFinished(
      RewardsService* rewards_service,
      const ledger::type::Result result,
      ledger::type::PromotionPtr promotion) override;
  void OnReconcileComplete(
      RewardsService* rewards_service,
      const ledger::type::Result result,
      const std::string& contribution_id,
      const double amount,
      const ledger::type::RewardsType type,
      const ledger::type::ContributionProcessor processor) override;

  void TriggerOnNotificationAdded(
      const RewardsNotification& rewards_notification);
  void TriggerOnNotificationDeleted(
      const RewardsNotification& rewards_notification);
  void TriggerOnAllNotificationsDeleted();
  void TriggerOnGetNotification(
      const RewardsNotification& rewards_notification);
  void TriggerOnGetAllNotifications(
      const RewardsNotificationsList& rewards_notifications_list);

  void OnNotificationAdded(const RewardsNotification& rewards_notification);
  void OnNotificationDeleted(const RewardsNotification& rewards_notification);
  void OnAllNotificationsDeleted();
  void OnGetNotification(const RewardsNotification& rewards_notification);
  void OnGetAllNotifications(
      const RewardsNotificationsList& rewards_notifications_list);

  RewardsNotificationID GenerateRewardsNotificationID() const;
  RewardsNotificationTimestamp GenerateRewardsNotificationTimestamp() const;

  Profile* profile_;
  RewardsNotificationsMap rewards_notifications_;
  std::vector<RewardsNotificationID> rewards_notifications_displayed_;
  std::unique_ptr<RewardsNotificationServiceObserver> extension_observer_;

  DISALLOW_COPY_AND_ASSIGN(RewardsNotificationServiceImpl);
};

}  // namespace adrbrowsiel_rewards

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_REWARDS_BROWSER_REWARDS_NOTIFICATION_SERVICE_IMPL_H_
