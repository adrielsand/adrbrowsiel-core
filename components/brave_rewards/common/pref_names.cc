/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/adrbrowsiel_rewards/common/pref_names.h"

namespace adrbrowsiel_rewards {
namespace prefs {

const char kHideButton[] = "adrbrowsiel.hide_adrbrowsiel_rewards_button";
const char kEnabled[] = "adrbrowsiel.rewards.enabled";
const char kNotifications[] = "adrbrowsiel.rewards.notifications";
const char kNotificationTimerInterval[]=
    "adrbrowsiel.rewards.notification_timer_interval";
const char kBackupNotificationInterval[] =
    "adrbrowsiel.rewards.backup_notification_interval";
const char kBackupSucceeded[] = "adrbrowsiel.rewards.backup_succeeded";
const char kUserHasFunded[] = "adrbrowsiel.rewards.user_has_funded";
const char kUserHasClaimedGrant[] = "adrbrowsiel.rewards.user_has_claimed_grant";
const char kAddFundsNotification[] =
    "adrbrowsiel.rewards.add_funds_notification";
const char kNotificationStartupDelay[] =
    "adrbrowsiel.rewards.notification_startup_delay";
const char kExternalWallets[] = "adrbrowsiel.rewards.external_wallets";
const char kServerPublisherListStamp[] =
    "adrbrowsiel.rewards.publisher_prefix_list_stamp";
const char kUpholdAnonAddress[] =
    "adrbrowsiel.rewards.uphold_anon_address";
const char kBadgeText[] = "adrbrowsiel.rewards.badge_text";
const char kUseRewardsStagingServer[] = "adrbrowsiel.rewards.use_staging_server";
const char kPromotionLastFetchStamp[] =
    "adrbrowsiel.rewards.promotion_last_fetch_stamp";
const char kPromotionCorruptedMigrated[] =
    "adrbrowsiel.rewards.promotion_corrupted_migrated2";
const char kAnonTransferChecked[] =  "adrbrowsiel.rewards.anon_transfer_checked";
const char kVersion[] =  "adrbrowsiel.rewards.version";
const char kMinVisitTime[] =  "adrbrowsiel.rewards.ac.min_visit_time";
const char kMinVisits[] =  "adrbrowsiel.rewards.ac.min_visits";
const char kAllowNonVerified[] =  "adrbrowsiel.rewards.ac.allow_non_verified";
const char kAllowVideoContribution[] =
    "adrbrowsiel.rewards.ac.allow_video_contributions";
const char kScoreA[] = "adrbrowsiel.rewards.ac.score.a";
const char kScoreB[] = "adrbrowsiel.rewards.ac.score.b";
const char kAutoContributeEnabled[] = "adrbrowsiel.rewards.ac.enabled";
const char kAutoContributeAmount[] = "adrbrowsiel.rewards.ac.amount";
const char kNextReconcileStamp[] = "adrbrowsiel.rewards.ac.next_reconcile_stamp";
const char kCreationStamp[] = "adrbrowsiel.rewards.creation_stamp";
const char kRecoverySeed[] = "adrbrowsiel.rewards.wallet.seed";
const char kPaymentId[] = "adrbrowsiel.rewards.wallet.payment_id";
const char kInlineTipRedditEnabled[] = "adrbrowsiel.rewards.inline_tip.reddit";
const char kInlineTipTwitterEnabled[] = "adrbrowsiel.rewards.inline_tip.twitter";
const char kInlineTipGithubEnabled[] = "adrbrowsiel.rewards.inline_tip.github";
const char kParametersRate[] = "adrbrowsiel.rewards.parameters.rate";
const char kParametersAutoContributeChoice[] =
    "adrbrowsiel.rewards.parameters.ac.choice";
const char kParametersAutoContributeChoices[] =
    "adrbrowsiel.rewards.parameters.ac.choices";
const char kParametersTipChoices[] =
    "adrbrowsiel.rewards.parameters.tip.choices";
const char kParametersMonthlyTipChoices[] =
    "adrbrowsiel.rewards.parameters.tip.monthly_choices";
const char kFetchOldBalance[] =
    "adrbrowsiel.rewards.fetch_old_balance";
const char kEmptyBalanceChecked[] =
    "adrbrowsiel.rewards.empty_balance_checked";
const char kWalletadrbrowsiel[] =
    "adrbrowsiel.rewards.wallets.adrbrowsiel";
const char kWalletUphold[] =
    "adrbrowsiel.rewards.wallets.uphold";
const char kWalletBitflyer[] = "adrbrowsiel.rewards.wallets.bitflyer";

}  // namespace prefs
}  // namespace adrbrowsiel_rewards
