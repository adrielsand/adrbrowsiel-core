/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/adrbrowsiel_rewards/android/adrbrowsiel_rewards_native_worker.h"

#include <iomanip>
#include <string>
#include <vector>
#include <utility>

#include "base/android/jni_android.h"
#include "base/android/jni_array.h"
#include "base/android/jni_string.h"
#include "base/containers/flat_map.h"
#include "base/json/json_writer.h"
#include "base/time/time.h"
#include "adrbrowsiel/browser/adrbrowsiel_ads/ads_service_factory.h"
#include "adrbrowsiel/browser/adrbrowsiel_rewards/rewards_service_factory.h"
#include "adrbrowsiel/build/android/jni_headers/adrbrowsielRewardsNativeWorker_jni.h"
#include "adrbrowsiel/components/adrbrowsiel_ads/browser/ads_service.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/rewards_service.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "content/public/browser/url_data_source.h"

#define DEFAULT_ADS_PER_HOUR 2
#define DEFAULT_AUTO_CONTRIBUTION_AMOUNT 10

namespace chrome {
namespace android {

adrbrowsielRewardsNativeWorker::adrbrowsielRewardsNativeWorker(JNIEnv* env,
    const base::android::JavaRef<jobject>& obj):
    weak_java_adrbrowsiel_rewards_native_worker_(env, obj),
    adrbrowsiel_rewards_service_(nullptr),
    weak_factory_(this) {
  Java_adrbrowsielRewardsNativeWorker_setNativePtr(env, obj,
    reinterpret_cast<intptr_t>(this));

  adrbrowsiel_rewards_service_ = adrbrowsiel_rewards::RewardsServiceFactory::GetForProfile(
      ProfileManager::GetActiveUserProfile()->GetOriginalProfile());
  if (adrbrowsiel_rewards_service_) {
    adrbrowsiel_rewards_service_->AddObserver(this);
    adrbrowsiel_rewards_service_->AddPrivateObserver(this);
    adrbrowsiel_rewards::RewardsNotificationService* notification_service =
      adrbrowsiel_rewards_service_->GetNotificationService();
    if (notification_service) {
      notification_service->AddObserver(this);
    }
  }
}

adrbrowsielRewardsNativeWorker::~adrbrowsielRewardsNativeWorker() {
}

void adrbrowsielRewardsNativeWorker::Destroy(JNIEnv* env) {
  if (adrbrowsiel_rewards_service_) {
    adrbrowsiel_rewards_service_->RemoveObserver(this);
    adrbrowsiel_rewards_service_->RemovePrivateObserver(this);
    adrbrowsiel_rewards::RewardsNotificationService* notification_service =
      adrbrowsiel_rewards_service_->GetNotificationService();
    if (notification_service) {
      notification_service->RemoveObserver(this);
    }
  }
  delete this;
}

void adrbrowsielRewardsNativeWorker::GetRewardsParameters(JNIEnv* env) {
  if (adrbrowsiel_rewards_service_) {
    adrbrowsiel_rewards_service_->GetRewardsParameters(
        base::BindOnce(&adrbrowsielRewardsNativeWorker::OnGetRewardsParameters,
                       weak_factory_.GetWeakPtr(), adrbrowsiel_rewards_service_));
  }
}

void adrbrowsielRewardsNativeWorker::GetPublisherInfo(
    JNIEnv* env,
    int tabId,
    const base::android::JavaParamRef<jstring>& host) {
  if (adrbrowsiel_rewards_service_) {
    adrbrowsiel_rewards_service_->GetPublisherActivityFromUrl(tabId,
      base::android::ConvertJavaStringToUTF8(env, host), "", "");
  }
}

void adrbrowsielRewardsNativeWorker::OnPanelPublisherInfo(
      adrbrowsiel_rewards::RewardsService* rewards_service,
      const ledger::type::Result result,
      const ledger::type::PublisherInfo* info,
      uint64_t tabId) {
  if (!info) {
    return;
  }
  ledger::type::PublisherInfoPtr pi = info->Clone();
  map_publishers_info_[tabId] = std::move(pi);
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_adrbrowsielRewardsNativeWorker_OnPublisherInfo(env,
        weak_java_adrbrowsiel_rewards_native_worker_.get(env), tabId);
}

base::android::ScopedJavaLocalRef<jstring>
adrbrowsielRewardsNativeWorker::GetPublisherURL(JNIEnv* env, uint64_t tabId) {
  base::android::ScopedJavaLocalRef<jstring> res =
    base::android::ConvertUTF8ToJavaString(env, "");

  PublishersInfoMap::const_iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    res = base::android::ConvertUTF8ToJavaString(env, iter->second->url);
  }

  return res;
}

base::android::ScopedJavaLocalRef<jstring>
adrbrowsielRewardsNativeWorker::GetPublisherFavIconURL(JNIEnv* env, uint64_t tabId) {
  base::android::ScopedJavaLocalRef<jstring> res =
    base::android::ConvertUTF8ToJavaString(env, "");

  PublishersInfoMap::const_iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    res = base::android::ConvertUTF8ToJavaString(env,
      iter->second->favicon_url);
  }

  return res;
}

base::android::ScopedJavaLocalRef<jstring>
adrbrowsielRewardsNativeWorker::GetPublisherName(JNIEnv* env, uint64_t tabId) {
  base::android::ScopedJavaLocalRef<jstring> res =
    base::android::ConvertUTF8ToJavaString(env, "");

  PublishersInfoMap::const_iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    res = base::android::ConvertUTF8ToJavaString(env, iter->second->name);
  }

  return res;
}

base::android::ScopedJavaLocalRef<jstring>
adrbrowsielRewardsNativeWorker::GetPublisherId(JNIEnv* env, uint64_t tabId) {
  base::android::ScopedJavaLocalRef<jstring> res =
    base::android::ConvertUTF8ToJavaString(env, "");

  PublishersInfoMap::const_iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    res = base::android::ConvertUTF8ToJavaString(env, iter->second->id);
  }

  return res;
}

int adrbrowsielRewardsNativeWorker::GetPublisherPercent(JNIEnv* env, uint64_t tabId) {
  int res = 0;

  PublishersInfoMap::const_iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    res = iter->second->percent;
  }

  return res;
}

bool adrbrowsielRewardsNativeWorker::GetPublisherExcluded(JNIEnv* env,
                                                    uint64_t tabId) {
  bool res = false;

  PublishersInfoMap::const_iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    res = iter->second->excluded == ledger::type::PublisherExclude::EXCLUDED;
  }

  return res;
}

int adrbrowsielRewardsNativeWorker::GetPublisherStatus(JNIEnv* env, uint64_t tabId) {
  int res = static_cast<int>(ledger::type::PublisherStatus::NOT_VERIFIED);
  PublishersInfoMap::const_iterator iter = map_publishers_info_.find(tabId);
  if (iter != map_publishers_info_.end()) {
    res = static_cast<int>(iter->second->status);
  }
  return res;
}

void adrbrowsielRewardsNativeWorker::IncludeInAutoContribution(JNIEnv* env,
                                                         uint64_t tabId,
                                                         bool exclude) {
  PublishersInfoMap::iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    if (exclude) {
      iter->second->excluded = ledger::type::PublisherExclude::EXCLUDED;
    } else {
      iter->second->excluded = ledger::type::PublisherExclude::INCLUDED;
    }
    if (adrbrowsiel_rewards_service_) {
      adrbrowsiel_rewards_service_->SetPublisherExclude(iter->second->id, exclude);
    }
  }
}

void adrbrowsielRewardsNativeWorker::RemovePublisherFromMap(JNIEnv* env,
                                                      uint64_t tabId) {
  PublishersInfoMap::const_iterator iter(map_publishers_info_.find(tabId));
  if (iter != map_publishers_info_.end()) {
    map_publishers_info_.erase(iter);
  }
}

void adrbrowsielRewardsNativeWorker::OnGetRewardsParameters(
    adrbrowsiel_rewards::RewardsService* rewards_service,
    ledger::type::RewardsParametersPtr parameters) {
  if (parameters) {
    parameters_ = *parameters;
  }

  if (rewards_service) {
    rewards_service->FetchBalance(
      base::Bind(
        &adrbrowsielRewardsNativeWorker::OnBalance,
        weak_factory_.GetWeakPtr()));
  }
}

void adrbrowsielRewardsNativeWorker::OnBalance(
    const ledger::type::Result result,
    ledger::type::BalancePtr balance) {
  if (result == ledger::type::Result::LEDGER_OK && balance) {
    balance_ = *balance;
  }

  JNIEnv* env = base::android::AttachCurrentThread();
  Java_adrbrowsielRewardsNativeWorker_OnRewardsParameters(
      env, weak_java_adrbrowsiel_rewards_native_worker_.get(env), 0);
}

base::android::ScopedJavaLocalRef<jstring>
adrbrowsielRewardsNativeWorker::GetWalletBalance(JNIEnv* env) {
  std::string json_balance;
  base::DictionaryValue json_root;
  json_root.SetDoubleKey("total", balance_.total);

  auto json_wallets = std::make_unique<base::DictionaryValue>();
  for (const auto & item : balance_.wallets) {
    json_wallets->SetDoubleKey(item.first, item.second);
  }
  json_root.SetDictionary("wallets", std::move(json_wallets));
  base::JSONWriter::Write(json_root, &json_balance);

  return base::android::ConvertUTF8ToJavaString(env, json_balance);
}

double adrbrowsielRewardsNativeWorker::GetWalletRate(JNIEnv* env) {
  return parameters_.rate;
}

void adrbrowsielRewardsNativeWorker::FetchGrants(JNIEnv* env) {
  if (adrbrowsiel_rewards_service_) {
    adrbrowsiel_rewards_service_->FetchPromotions();
  }
}

void adrbrowsielRewardsNativeWorker::StartProcess(JNIEnv* env) {
  if (adrbrowsiel_rewards_service_) {
    adrbrowsiel_rewards_service_->StartProcess(base::Bind(
          &adrbrowsielRewardsNativeWorker::OnStartProcess,
          weak_factory_.GetWeakPtr()));
  }
}

void adrbrowsielRewardsNativeWorker::OnStartProcess() {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_adrbrowsielRewardsNativeWorker_OnStartProcess(
      env, weak_java_adrbrowsiel_rewards_native_worker_.get(env));
}

void adrbrowsielRewardsNativeWorker::GetCurrentBalanceReport(JNIEnv* env) {
  if (adrbrowsiel_rewards_service_) {
    auto now = base::Time::Now();
    base::Time::Exploded exploded;
    now.LocalExplode(&exploded);

    adrbrowsiel_rewards_service_->GetBalanceReport(
        exploded.month, exploded.year,
        base::BindOnce(&adrbrowsielRewardsNativeWorker::OnGetCurrentBalanceReport,
                       weak_factory_.GetWeakPtr(), adrbrowsiel_rewards_service_));
  }
}

void adrbrowsielRewardsNativeWorker::OnGetCurrentBalanceReport(
        adrbrowsiel_rewards::RewardsService* rewards_service,
        const ledger::type::Result result,
        ledger::type::BalanceReportInfoPtr report) {
  base::android::ScopedJavaLocalRef<jdoubleArray> java_array;
  JNIEnv* env = base::android::AttachCurrentThread();
  if (report) {
    std::vector<double> values;
    values.push_back(report->grants);
    values.push_back(report->earning_from_ads);
    values.push_back(report->auto_contribute);
    values.push_back(report->recurring_donation);
    values.push_back(report->one_time_donation);
    java_array = base::android::ToJavaDoubleArray(env, values);
  }
  Java_adrbrowsielRewardsNativeWorker_OnGetCurrentBalanceReport(env,
        weak_java_adrbrowsiel_rewards_native_worker_.get(env), java_array);
}

void adrbrowsielRewardsNativeWorker::Donate(JNIEnv* env,
        const base::android::JavaParamRef<jstring>& publisher_key,
        int amount, bool recurring) {
  if (adrbrowsiel_rewards_service_) {
    adrbrowsiel_rewards_service_->OnTip(
      base::android::ConvertJavaStringToUTF8(env, publisher_key), amount,
        recurring);
    if (!recurring) {
      Java_adrbrowsielRewardsNativeWorker_OnOneTimeTip(env,
        weak_java_adrbrowsiel_rewards_native_worker_.get(env));
    }
  }
}

void adrbrowsielRewardsNativeWorker::GetAllNotifications(JNIEnv* env) {
  if (!adrbrowsiel_rewards_service_) {
    return;
  }
  adrbrowsiel_rewards::RewardsNotificationService* notification_service =
    adrbrowsiel_rewards_service_->GetNotificationService();
  if (notification_service) {
    notification_service->GetNotifications();
  }
}

void adrbrowsielRewardsNativeWorker::DeleteNotification(JNIEnv* env,
        const base::android::JavaParamRef<jstring>& notification_id) {
  if (!adrbrowsiel_rewards_service_) {
    return;
  }
  adrbrowsiel_rewards::RewardsNotificationService* notification_service =
    adrbrowsiel_rewards_service_->GetNotificationService();
  if (notification_service) {
    notification_service->DeleteNotification(
      base::android::ConvertJavaStringToUTF8(env, notification_id));
  }
}

void adrbrowsielRewardsNativeWorker::GetGrant(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& promotionId) {
  if (adrbrowsiel_rewards_service_) {
    std::string promotion_id =
      base::android::ConvertJavaStringToUTF8(env, promotionId);
    adrbrowsiel_rewards_service_->ClaimPromotion(
        promotion_id,
        base::BindOnce(&adrbrowsielRewardsNativeWorker::OnClaimPromotion,
                       weak_factory_.GetWeakPtr()));
  }
}

void adrbrowsielRewardsNativeWorker::OnClaimPromotion(
    const ledger::type::Result result,
    ledger::type::PromotionPtr promotion) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_adrbrowsielRewardsNativeWorker_OnClaimPromotion(env,
      weak_java_adrbrowsiel_rewards_native_worker_.get(env),
      static_cast<int>(result));
}

base::android::ScopedJavaLocalRef<jobjectArray>
    adrbrowsielRewardsNativeWorker::GetCurrentGrant(JNIEnv* env,
      int position) {
  if ((size_t)position > promotions_.size() - 1) {
    return base::android::ScopedJavaLocalRef<jobjectArray>();
  }
  std::stringstream stream;
  stream << std::fixed << std::setprecision(2) <<
      (promotions_[position])->approximate_value;
  std::vector<std::string> values;
  values.push_back(stream.str());
  values.push_back(
    std::to_string((promotions_[position])->expires_at));
  values.push_back(
      std::to_string(static_cast<int>((promotions_[position])->type)));

  return base::android::ToJavaArrayOfStrings(env, values);
}

void adrbrowsielRewardsNativeWorker::GetPendingContributionsTotal(JNIEnv* env) {
  if (adrbrowsiel_rewards_service_) {
    adrbrowsiel_rewards_service_->GetPendingContributionsTotal(base::Bind(
          &adrbrowsielRewardsNativeWorker::OnGetPendingContributionsTotal,
          weak_factory_.GetWeakPtr()));
  }
}

void adrbrowsielRewardsNativeWorker::GetRecurringDonations(JNIEnv* env) {
  if (adrbrowsiel_rewards_service_) {
    adrbrowsiel_rewards_service_->GetRecurringTips(base::Bind(
          &adrbrowsielRewardsNativeWorker::OnGetRecurringTips,
          weak_factory_.GetWeakPtr()));
  }
}

void adrbrowsielRewardsNativeWorker::OnGetRecurringTips(
    ledger::type::PublisherInfoList list) {
  map_recurrent_publishers_.clear();
  for (const auto& item : list) {
    map_recurrent_publishers_[item->id] = item->Clone();
  }

  JNIEnv* env = base::android::AttachCurrentThread();
  Java_adrbrowsielRewardsNativeWorker_OnRecurringDonationUpdated(env,
        weak_java_adrbrowsiel_rewards_native_worker_.get(env));
}

bool adrbrowsielRewardsNativeWorker::IsCurrentPublisherInRecurrentDonations(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& publisher) {
  return map_recurrent_publishers_.find(
    base::android::ConvertJavaStringToUTF8(env, publisher)) !=
      map_recurrent_publishers_.end();
}

void adrbrowsielRewardsNativeWorker::GetAutoContributeProperties(JNIEnv* env) {
  if (adrbrowsiel_rewards_service_) {
    adrbrowsiel_rewards_service_->GetAutoContributeProperties(
        base::Bind(&adrbrowsielRewardsNativeWorker::OnGetAutoContributeProperties,
                   weak_factory_.GetWeakPtr()));
  }
}

void adrbrowsielRewardsNativeWorker::OnGetAutoContributeProperties(
    ledger::type::AutoContributePropertiesPtr properties) {
  if (properties) {
    auto_contrib_properties_ = std::move(properties);
  }

  JNIEnv* env = base::android::AttachCurrentThread();
  Java_adrbrowsielRewardsNativeWorker_OnGetAutoContributeProperties(
      env, weak_java_adrbrowsiel_rewards_native_worker_.get(env));
}

bool adrbrowsielRewardsNativeWorker::IsAutoContributeEnabled(JNIEnv* env) {
  if (!auto_contrib_properties_) {
    return false;
  }

  return auto_contrib_properties_->enabled_contribute;
}

void adrbrowsielRewardsNativeWorker::GetReconcileStamp(JNIEnv* env) {
  if (adrbrowsiel_rewards_service_) {
    adrbrowsiel_rewards_service_->GetReconcileStamp(base::Bind(
            &adrbrowsielRewardsNativeWorker::OnGetGetReconcileStamp,
            weak_factory_.GetWeakPtr()));
  }
}

void adrbrowsielRewardsNativeWorker::ResetTheWholeState(JNIEnv* env) {
  if (adrbrowsiel_rewards_service_) {
    adrbrowsiel_rewards_service_->CompleteReset(base::Bind(
           &adrbrowsielRewardsNativeWorker::OnResetTheWholeState,
           weak_factory_.GetWeakPtr()));
  } else {
    JNIEnv* env = base::android::AttachCurrentThread();

    Java_adrbrowsielRewardsNativeWorker_OnResetTheWholeState(env,
            weak_java_adrbrowsiel_rewards_native_worker_.get(env), false);
  }
}

void adrbrowsielRewardsNativeWorker::OnResetTheWholeState(const bool success) {
  JNIEnv* env = base::android::AttachCurrentThread();

  Java_adrbrowsielRewardsNativeWorker_OnResetTheWholeState(env,
          weak_java_adrbrowsiel_rewards_native_worker_.get(env), success);
}

double adrbrowsielRewardsNativeWorker::GetPublisherRecurrentDonationAmount(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& publisher) {
  double amount(0.0);
  auto it = map_recurrent_publishers_.find(
    base::android::ConvertJavaStringToUTF8(env, publisher));
  if (it != map_recurrent_publishers_.end()) {
    // for Recurrent Donations, the amount is stored in ContentSite::percentage
    amount = it->second->percent;
  }
  return  amount;
}

void adrbrowsielRewardsNativeWorker::RemoveRecurring(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& publisher) {
  if (adrbrowsiel_rewards_service_) {
      adrbrowsiel_rewards_service_->RemoveRecurringTip(
        base::android::ConvertJavaStringToUTF8(env, publisher));
  }
}

void adrbrowsielRewardsNativeWorker::OnGetGetReconcileStamp(uint64_t timestamp) {
  JNIEnv* env = base::android::AttachCurrentThread();

  Java_adrbrowsielRewardsNativeWorker_OnGetReconcileStamp(env,
          weak_java_adrbrowsiel_rewards_native_worker_.get(env), timestamp);
}

void adrbrowsielRewardsNativeWorker::OnGetPendingContributionsTotal(double amount) {
  JNIEnv* env = base::android::AttachCurrentThread();

  Java_adrbrowsielRewardsNativeWorker_OnGetPendingContributionsTotal(env,
        weak_java_adrbrowsiel_rewards_native_worker_.get(env), amount);
}

void adrbrowsielRewardsNativeWorker::OnNotificationAdded(
    adrbrowsiel_rewards::RewardsNotificationService* rewards_notification_service,
    const adrbrowsiel_rewards::RewardsNotificationService::RewardsNotification&
      notification) {
  JNIEnv* env = base::android::AttachCurrentThread();

  Java_adrbrowsielRewardsNativeWorker_OnNotificationAdded(env,
        weak_java_adrbrowsiel_rewards_native_worker_.get(env),
        base::android::ConvertUTF8ToJavaString(env, notification.id_),
        notification.type_,
        notification.timestamp_,
        base::android::ToJavaArrayOfStrings(env, notification.args_));
}

void adrbrowsielRewardsNativeWorker::OnGetAllNotifications(
    adrbrowsiel_rewards::RewardsNotificationService* rewards_notification_service,
    const adrbrowsiel_rewards::RewardsNotificationService::RewardsNotificationsList&
      notifications_list) {
  JNIEnv* env = base::android::AttachCurrentThread();

  // Notify about notifications count
  Java_adrbrowsielRewardsNativeWorker_OnNotificationsCount(env,
        weak_java_adrbrowsiel_rewards_native_worker_.get(env),
        notifications_list.size());

  adrbrowsiel_rewards::RewardsNotificationService::RewardsNotificationsList::
    const_iterator iter =
      std::max_element(notifications_list.begin(), notifications_list.end(),
        [](const adrbrowsiel_rewards::RewardsNotificationService::
            RewardsNotification& notification_a,
          const adrbrowsiel_rewards::RewardsNotificationService::
            RewardsNotification& notification_b) {
        return notification_a.timestamp_ > notification_b.timestamp_;
      });

  if (iter != notifications_list.end()) {
    Java_adrbrowsielRewardsNativeWorker_OnGetLatestNotification(env,
        weak_java_adrbrowsiel_rewards_native_worker_.get(env),
        base::android::ConvertUTF8ToJavaString(env, iter->id_),
        iter->type_,
        iter->timestamp_,
        base::android::ToJavaArrayOfStrings(env, iter->args_));
  }
}

void adrbrowsielRewardsNativeWorker::OnNotificationDeleted(
      adrbrowsiel_rewards::RewardsNotificationService* rewards_notification_service,
      const adrbrowsiel_rewards::RewardsNotificationService::RewardsNotification&
        notification) {
  JNIEnv* env = base::android::AttachCurrentThread();

  Java_adrbrowsielRewardsNativeWorker_OnNotificationDeleted(env,
        weak_java_adrbrowsiel_rewards_native_worker_.get(env),
        base::android::ConvertUTF8ToJavaString(env, notification.id_));
}

void adrbrowsielRewardsNativeWorker::OnPromotionFinished(
    adrbrowsiel_rewards::RewardsService* rewards_service,
    const ledger::type::Result result,
    ledger::type::PromotionPtr promotion) {
  JNIEnv* env = base::android::AttachCurrentThread();

  Java_adrbrowsielRewardsNativeWorker_OnGrantFinish(env,
        weak_java_adrbrowsiel_rewards_native_worker_.get(env),
        static_cast<int>(result));
}

int adrbrowsielRewardsNativeWorker::GetAdsPerHour(JNIEnv* env) {
  auto* ads_service_ = adrbrowsiel_ads::AdsServiceFactory::GetForProfile(
      ProfileManager::GetActiveUserProfile()->GetOriginalProfile());
  if (!ads_service_) {
    return DEFAULT_ADS_PER_HOUR;
  }
  return ads_service_->GetAdsPerHour();
}

void adrbrowsielRewardsNativeWorker::SetAdsPerHour(JNIEnv* env, jint value) {
  auto* ads_service_ = adrbrowsiel_ads::AdsServiceFactory::GetForProfile(
      ProfileManager::GetActiveUserProfile()->GetOriginalProfile());
  if (!ads_service_) {
    return;
  }
  ads_service_->SetAdsPerHour(value);
}

void adrbrowsielRewardsNativeWorker::SetAutoContributionAmount(JNIEnv* env,
                                                         jdouble value) {
  if (adrbrowsiel_rewards_service_) {
    adrbrowsiel_rewards_service_->SetAutoContributionAmount(value);
  }
}

bool adrbrowsielRewardsNativeWorker::IsAnonWallet(JNIEnv* env) {
  if (adrbrowsiel_rewards_service_) {
    return adrbrowsiel_rewards_service_->OnlyAnonWallet();
  }
  return false;
}

bool adrbrowsielRewardsNativeWorker::IsRewardsEnabled(JNIEnv* env) {
  if (adrbrowsiel_rewards_service_) {
    return adrbrowsiel_rewards_service_->IsRewardsEnabled();
  }
  return false;
}

void adrbrowsielRewardsNativeWorker::GetExternalWallet(JNIEnv* env) {
  if (adrbrowsiel_rewards_service_) {
    adrbrowsiel_rewards_service_->GetExternalWallet(
        base::BindOnce(&adrbrowsielRewardsNativeWorker::OnGetExternalWallet,
                       weak_factory_.GetWeakPtr()));
  }
}

void adrbrowsielRewardsNativeWorker::OnGetExternalWallet(
    const ledger::type::Result result,
    ledger::type::ExternalWalletPtr wallet) {
  std::string json_wallet;
  if (!wallet) {
    json_wallet = "";
  } else {
    base::Value dict(base::Value::Type::DICTIONARY);
    dict.SetStringKey("token", wallet->token);
    dict.SetStringKey("address", wallet->address);

    // enum class WalletStatus : int32_t
    dict.SetIntKey("status", static_cast<int32_t>(wallet->status));
    dict.SetStringKey("verify_url", wallet->verify_url);
    dict.SetStringKey("add_url", wallet->add_url);
    dict.SetStringKey("type", wallet->type);
    dict.SetStringKey("withdraw_url", wallet->withdraw_url);
    dict.SetStringKey("user_name", wallet->user_name);
    dict.SetStringKey("account_url", wallet->account_url);
    dict.SetStringKey("login_url", wallet->login_url);
    base::JSONWriter::Write(dict, &json_wallet);
  }
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_adrbrowsielRewardsNativeWorker_OnGetExternalWallet(env,
      weak_java_adrbrowsiel_rewards_native_worker_.get(env),
      static_cast<int>(result),
      base::android::ConvertUTF8ToJavaString(env, json_wallet));
}

void adrbrowsielRewardsNativeWorker::DisconnectWallet(JNIEnv* env) {
  if (adrbrowsiel_rewards_service_) {
    adrbrowsiel_rewards_service_->DisconnectWallet();
  }
}

void adrbrowsielRewardsNativeWorker::OnDisconnectWallet(
    adrbrowsiel_rewards::RewardsService* rewards_service,
    const ledger::type::Result result,
    const std::string& wallet_type) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_adrbrowsielRewardsNativeWorker_OnDisconnectWallet(env,
        weak_java_adrbrowsiel_rewards_native_worker_.get(env),
        static_cast<int>(result),
        base::android::ConvertUTF8ToJavaString(env, wallet_type));
}

void adrbrowsielRewardsNativeWorker::ProcessRewardsPageUrl(JNIEnv* env,
        const base::android::JavaParamRef<jstring>& path,
        const base::android::JavaParamRef<jstring>& query) {
  if (adrbrowsiel_rewards_service_) {
    std::string cpath = base::android::ConvertJavaStringToUTF8(env, path);
    std::string cquery = base::android::ConvertJavaStringToUTF8(env, query);
    auto callback =
        base::Bind(&adrbrowsielRewardsNativeWorker::OnProcessRewardsPageUrl,
                   weak_factory_.GetWeakPtr());
    adrbrowsiel_rewards_service_->ProcessRewardsPageUrl(cpath, cquery, callback);
  }
}

void adrbrowsielRewardsNativeWorker::OnProcessRewardsPageUrl(
    const ledger::type::Result result,
    const std::string& wallet_type,
    const std::string& action,
    const base::flat_map<std::string, std::string>& args) {
  std::string json_args = StdStrStrMapToJsonString(args);
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_adrbrowsielRewardsNativeWorker_OnProcessRewardsPageUrl(env,
        weak_java_adrbrowsiel_rewards_native_worker_.get(env),
        static_cast<int>(result),
        base::android::ConvertUTF8ToJavaString(env, wallet_type),
        base::android::ConvertUTF8ToJavaString(env, action),
        base::android::ConvertUTF8ToJavaString(env, json_args));
}

std::string adrbrowsielRewardsNativeWorker::StdStrStrMapToJsonString(
    const base::flat_map<std::string, std::string>& args) {
    std::string json_args;
    base::Value dict(base::Value::Type::DICTIONARY);
    for (const auto & item : args) {
      dict.SetStringKey(item.first, item.second);
    }
    base::JSONWriter::Write(dict, &json_args);
    return json_args;
}

void adrbrowsielRewardsNativeWorker::RecoverWallet(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& pass_phrase) {
  if (adrbrowsiel_rewards_service_) {
    adrbrowsiel_rewards_service_->RecoverWallet(
        base::android::ConvertJavaStringToUTF8(env, pass_phrase));
  }
}

void adrbrowsielRewardsNativeWorker::OnRecoverWallet(
    adrbrowsiel_rewards::RewardsService* rewards_service,
    const ledger::type::Result result) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_adrbrowsielRewardsNativeWorker_OnRecoverWallet(
      env, weak_java_adrbrowsiel_rewards_native_worker_.get(env),
      static_cast<int>(result));
}

void adrbrowsielRewardsNativeWorker::RefreshPublisher(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& publisher_key) {
  if (!adrbrowsiel_rewards_service_) {
    NOTREACHED();
    return;
  }
  adrbrowsiel_rewards_service_->RefreshPublisher(
      base::android::ConvertJavaStringToUTF8(env, publisher_key),
      base::BindOnce(&adrbrowsielRewardsNativeWorker::OnRefreshPublisher,
                     weak_factory_.GetWeakPtr()));
}

void adrbrowsielRewardsNativeWorker::OnRefreshPublisher(
    const ledger::type::PublisherStatus status,
    const std::string& publisher_key) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_adrbrowsielRewardsNativeWorker_OnRefreshPublisher(
      env, weak_java_adrbrowsiel_rewards_native_worker_.get(env),
      static_cast<int>(status),
      base::android::ConvertUTF8ToJavaString(env, publisher_key));
}

void adrbrowsielRewardsNativeWorker::SetAutoContributeEnabled(
    JNIEnv* env,
    bool isAutoContributeEnabled) {
  if (adrbrowsiel_rewards_service_) {
    adrbrowsiel_rewards_service_->SetAutoContributeEnabled(isAutoContributeEnabled);
  }
}

static void JNI_adrbrowsielRewardsNativeWorker_Init(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& jcaller) {
  new adrbrowsielRewardsNativeWorker(env, jcaller);
}

}  // namespace android
}  // namespace chrome
