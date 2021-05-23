/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/webui/adrbrowsiel_rewards_page_ui.h"

#include <stdint.h>

#include <utility>
#include <memory>
#include <vector>

#include "base/i18n/time_formatting.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string_number_conversions.h"
#include "base/time/time.h"
#include "bat/ledger/mojom_structs.h"
#include "adrbrowsiel/browser/adrbrowsiel_ads/ads_service_factory.h"
#include "adrbrowsiel/browser/adrbrowsiel_rewards/rewards_service_factory.h"
#include "adrbrowsiel/browser/ui/webui/adrbrowsiel_webui_source.h"
#include "adrbrowsiel/common/webui_url_constants.h"
#include "adrbrowsiel/components/adrbrowsiel_ads/browser/ads_service.h"
#include "adrbrowsiel/components/adrbrowsiel_ads/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/rewards_notification_service.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/rewards_notification_service_observer.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/rewards_service.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/rewards_service_observer.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/resources/grit/adrbrowsiel_rewards_page_generated_map.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/resources/grit/adrbrowsiel_rewards_resources.h"
#include "adrbrowsiel/components/l10n/browser/locale_helper.h"
#include "adrbrowsiel/components/l10n/common/locale_util.h"
#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/render_view_host.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui_data_source.h"
#include "content/public/browser/web_ui_message_handler.h"
#include "content/public/common/bindings_policy.h"

#if defined(OS_ANDROID)
#include "content/public/browser/url_data_source.h"
#include "chrome/browser/ui/webui/favicon_source.h"
#include "components/favicon_base/favicon_url_parser.h"
#endif

using content::WebUIMessageHandler;

namespace {

// The handler for Javascript messages for adrbrowsiel about: pages
class RewardsDOMHandler : public WebUIMessageHandler,
    public adrbrowsiel_ads::AdsServiceObserver,
    public adrbrowsiel_rewards::RewardsNotificationServiceObserver,
    public adrbrowsiel_rewards::RewardsServiceObserver {
 public:
  RewardsDOMHandler();
  ~RewardsDOMHandler() override;

  void Init();

  // WebUIMessageHandler implementation.
  void OnJavascriptAllowed() override;
  void OnJavascriptDisallowed() override;
  void RegisterMessages() override;

 private:
  void IsInitialized(const base::ListValue* args);
  void GetRewardsParameters(const base::ListValue* args);
  void GetAutoContributeProperties(const base::ListValue* args);
  void FetchPromotions(const base::ListValue* args);
  void ClaimPromotion(const base::ListValue* args);
  void AttestPromotion(const base::ListValue* args);
  void RecoverWallet(const base::ListValue* args);
  void GetReconcileStamp(const base::ListValue* args);
  void SaveSetting(const base::ListValue* args);
  void OnPublisherList(ledger::type::PublisherInfoList list);
  void OnExcludedSiteList(ledger::type::PublisherInfoList list);
  void ExcludePublisher(const base::ListValue* args);
  void RestorePublishers(const base::ListValue* args);
  void RestorePublisher(const base::ListValue* args);
  void GetAutoContributionAmount(const base::ListValue* args);
  void RemoveRecurringTip(const base::ListValue* args);
  void GetRecurringTips(const base::ListValue* args);
  void GetOneTimeTips(const base::ListValue* args);
  void GetContributionList(const base::ListValue* args);
  void GetAdsData(const base::ListValue* args);
  void GetAdsHistory(const base::ListValue* args);
  void OnGetAdsHistory(const base::ListValue& history);
  void ToggleAdThumbUp(const base::ListValue* args);
  void OnToggleAdThumbUp(
      const std::string& creative_instance_id,
      const int action);
  void ToggleAdThumbDown(const base::ListValue* args);
  void OnToggleAdThumbDown(
      const std::string& creative_instance_id,
      const int action);
  void ToggleAdOptInAction(const base::ListValue* args);
  void OnToggleAdOptInAction(
      const std::string& category,
      const int action);
  void ToggleAdOptOutAction(const base::ListValue* args);
  void OnToggleAdOptOutAction(
      const std::string& category,
      const int action);
  void ToggleSaveAd(const base::ListValue* args);
  void OnToggleSaveAd(
      const std::string& creative_instance_id,
      const bool saved);
  void ToggleFlagAd(const base::ListValue* args);
  void OnToggleFlagAd(
      const std::string& creative_instance_id,
      const bool flagged);
  void SaveAdsSetting(const base::ListValue* args);
  void SetBackupCompleted(const base::ListValue* args);
  void OnGetContributionAmount(double amount);
  void OnGetAutoContributeProperties(
      ledger::type::AutoContributePropertiesPtr properties);
  void OnGetReconcileStamp(uint64_t reconcile_stamp);
  void OnAutoContributePropsReady(
      ledger::type::AutoContributePropertiesPtr properties);
  void GetPendingContributionsTotal(const base::ListValue* args);
  void OnGetPendingContributionsTotal(double amount);
  void GetStatement(const base::ListValue* args);
  void GetExcludedSites(const base::ListValue* args);

  void OnGetStatement(const bool success,
                      const double estimated_pending_rewards,
                      const int64_t next_payment_date,
                      const int ads_received_this_month,
                      const double earnings_this_month,
                      const double earnings_last_month);

  void OnGetRecurringTips(ledger::type::PublisherInfoList list);

  void OnGetOneTimeTips(ledger::type::PublisherInfoList list);

  void SetInlineTippingPlatformEnabled(const base::ListValue* args);

  void GetPendingContributions(const base::ListValue* args);
  void OnGetPendingContributions(
      ledger::type::PendingContributionInfoList list);
  void RemovePendingContribution(const base::ListValue* args);
  void RemoveAllPendingContributions(const base::ListValue* args);
  void FetchBalance(const base::ListValue* args);
  void OnFetchBalance(
    const ledger::type::Result result,
    ledger::type::BalancePtr balance);

  void GetExternalWallet(const base::ListValue* args);
  void OnGetExternalWallet(const ledger::type::Result result,
                           ledger::type::ExternalWalletPtr wallet);

  void ProcessRewardsPageUrl(const base::ListValue* args);

  void OnProcessRewardsPageUrl(
    const ledger::type::Result result,
    const std::string& wallet_type,
    const std::string& action,
    const base::flat_map<std::string, std::string>& args);

  void DisconnectWallet(const base::ListValue* args);

  void OnlyAnonWallet(const base::ListValue* args);

  void GetBalanceReport(const base::ListValue* args);

  void OnGetBalanceReport(
      const uint32_t month,
      const uint32_t year,
      const ledger::type::Result result,
      ledger::type::BalanceReportInfoPtr report);

  void GetMonthlyReport(const base::ListValue* args);

  void GetAllMonthlyReportIds(const base::ListValue* args);
  void GetCountryCode(const base::ListValue* args);

  void OnGetMonthlyReport(
      const uint32_t month,
      const uint32_t year,
      ledger::type::MonthlyReportInfoPtr report);

  void OnGetAllMonthlyReportIds(const std::vector<std::string>& ids);

  void OnGetRewardsParameters(ledger::type::RewardsParametersPtr parameters);

  void CompleteReset(const base::ListValue* args);

  void GetPaymentId(const base::ListValue* args);

  void OnWalletCreatedForPaymentId(ledger::type::Result result);

  void OnGetPaymentId(ledger::type::adrbrowsielWalletPtr wallet);

  void GetWalletPassphrase(const base::ListValue* args);

  void OnGetWalletPassphrase(const std::string& pass);

  void GetOnboardingStatus(const base::ListValue* args);
  void SaveOnboardingResult(const base::ListValue* args);

  // RewardsServiceObserver implementation
  void OnRewardsInitialized(
      adrbrowsiel_rewards::RewardsService* rewards_service) override;
  void OnFetchPromotions(
      adrbrowsiel_rewards::RewardsService* rewards_service,
      const ledger::type::Result result,
      const ledger::type::PromotionList& list) override;
  void OnPromotionFinished(
      adrbrowsiel_rewards::RewardsService* rewards_service,
      const ledger::type::Result result,
      ledger::type::PromotionPtr promotion) override;
  void OnRecoverWallet(
      adrbrowsiel_rewards::RewardsService* rewards_service,
      const ledger::type::Result result) override;
  void OnExcludedSitesChanged(
      adrbrowsiel_rewards::RewardsService* rewards_service,
      std::string publisher_id,
      bool excluded) override;
  void OnReconcileComplete(
      adrbrowsiel_rewards::RewardsService* rewards_service,
      const ledger::type::Result result,
      const std::string& contribution_id,
      const double amount,
      const ledger::type::RewardsType type,
      const ledger::type::ContributionProcessor processor) override;

  void OnPendingContributionSaved(
      adrbrowsiel_rewards::RewardsService* rewards_service,
      const ledger::type::Result result) override;

  void OnPublisherListNormalized(
      adrbrowsiel_rewards::RewardsService* rewards_service,
      ledger::type::PublisherInfoList list) override;

  void OnStatementChanged(
      adrbrowsiel_rewards::RewardsService* rewards_service) override;

  void OnRecurringTipSaved(
      adrbrowsiel_rewards::RewardsService* rewards_service,
      bool success) override;

  void OnRecurringTipRemoved(
      adrbrowsiel_rewards::RewardsService* rewards_service,
      bool success) override;

  void OnPendingContributionRemoved(
      adrbrowsiel_rewards::RewardsService* rewards_service,
      const ledger::type::Result result) override;

  void OnDisconnectWallet(
      adrbrowsiel_rewards::RewardsService* rewards_service,
      const ledger::type::Result result,
      const std::string& wallet_type) override;

  void OnAdsEnabled(adrbrowsiel_rewards::RewardsService* rewards_service,
                    bool ads_enabled) override;

  void OnClaimPromotion(
      const std::string& promotion_id,
      const ledger::type::Result result,
      const std::string& captcha_image,
      const std::string& hint,
      const std::string& captcha_id);

  void OnAttestPromotion(
      const std::string& promotion_id,
      const ledger::type::Result result,
      ledger::type::PromotionPtr promotion);

  void OnUnblindedTokensReady(
    adrbrowsiel_rewards::RewardsService* rewards_service) override;

  void ReconcileStampReset() override;

  void OnCompleteReset(const bool success) override;

  // RewardsNotificationsServiceObserver implementation
  void OnNotificationAdded(
      adrbrowsiel_rewards::RewardsNotificationService* rewards_notification_service,
      const adrbrowsiel_rewards::RewardsNotificationService::RewardsNotification&
          notification) override;
  void OnNotificationDeleted(
      adrbrowsiel_rewards::RewardsNotificationService* rewards_notification_service,
      const adrbrowsiel_rewards::RewardsNotificationService::RewardsNotification&
          notification) override;
  void OnAllNotificationsDeleted(adrbrowsiel_rewards::RewardsNotificationService*
        rewards_notification_service) override;
  void OnGetNotification(
      adrbrowsiel_rewards::RewardsNotificationService* rewards_notification_service,
      const adrbrowsiel_rewards::RewardsNotificationService::RewardsNotification&
          notification) override;
  void OnGetAllNotifications(
      adrbrowsiel_rewards::RewardsNotificationService* rewards_notification_service,
      const adrbrowsiel_rewards::RewardsNotificationService::RewardsNotificationsList&
          notifications_list) override;

  // AdsServiceObserver implementation
  void OnAdRewardsChanged() override;

  adrbrowsiel_rewards::RewardsService* rewards_service_;  // NOT OWNED
  adrbrowsiel_ads::AdsService* ads_service_;  // NOT OWNED
  base::WeakPtrFactory<RewardsDOMHandler> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(RewardsDOMHandler);
};

namespace {

const int kDaysOfAdsHistory = 7;

const char kShouldAllowAdsSubdivisionTargeting[] =
    "shouldAllowAdsSubdivisionTargeting";
const char kAdsSubdivisionTargeting[] = "adsSubdivisionTargeting";
const char kAutoDetectedAdsSubdivisionTargeting[] =
    "automaticallyDetectedAdsSubdivisionTargeting";

}  // namespace

RewardsDOMHandler::RewardsDOMHandler() : weak_factory_(this) {}

RewardsDOMHandler::~RewardsDOMHandler() {}

void RewardsDOMHandler::RegisterMessages() {
#if defined(OS_ANDROID)
  // Create our favicon data source.
  Profile* profile = Profile::FromWebUI(web_ui());
  content::URLDataSource::Add(
      profile, std::make_unique<FaviconSource>(
                   profile, chrome::FaviconUrlFormat::kFaviconLegacy));
#endif

  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.isInitialized",
      base::BindRepeating(&RewardsDOMHandler::IsInitialized,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.getRewardsParameters",
      base::BindRepeating(&RewardsDOMHandler::GetRewardsParameters,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.getAutoContributeProperties",
      base::BindRepeating(&RewardsDOMHandler::GetAutoContributeProperties,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.fetchPromotions",
      base::BindRepeating(&RewardsDOMHandler::FetchPromotions,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.claimPromotion",
      base::BindRepeating(&RewardsDOMHandler::ClaimPromotion,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.attestPromotion",
      base::BindRepeating(&RewardsDOMHandler::AttestPromotion,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.recoverWallet",
      base::BindRepeating(&RewardsDOMHandler::RecoverWallet,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.getReconcileStamp",
      base::BindRepeating(&RewardsDOMHandler::GetReconcileStamp,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.saveSetting",
      base::BindRepeating(&RewardsDOMHandler::SaveSetting,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.excludePublisher",
      base::BindRepeating(&RewardsDOMHandler::ExcludePublisher,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.restorePublishers",
      base::BindRepeating(&RewardsDOMHandler::RestorePublishers,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.restorePublisher",
      base::BindRepeating(&RewardsDOMHandler::RestorePublisher,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.getContributionAmount",
      base::BindRepeating(&RewardsDOMHandler::GetAutoContributionAmount,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.removeRecurringTip",
      base::BindRepeating(&RewardsDOMHandler::RemoveRecurringTip,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "adrbrowsiel_rewards.getRecurringTips",
      base::BindRepeating(&RewardsDOMHandler::GetRecurringTips,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.getOneTimeTips",
      base::BindRepeating(&RewardsDOMHandler::GetOneTimeTips,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.getContributionList",
      base::BindRepeating(&RewardsDOMHandler::GetContributionList,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.getAdsData",
      base::BindRepeating(&RewardsDOMHandler::GetAdsData,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.getAdsHistory",
      base::BindRepeating(&RewardsDOMHandler::GetAdsHistory,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.toggleAdThumbUp",
      base::BindRepeating(&RewardsDOMHandler::ToggleAdThumbUp,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.toggleAdThumbDown",
      base::BindRepeating(&RewardsDOMHandler::ToggleAdThumbDown,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.toggleAdOptInAction",
      base::BindRepeating(&RewardsDOMHandler::ToggleAdOptInAction,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.toggleAdOptOutAction",
      base::BindRepeating(&RewardsDOMHandler::ToggleAdOptOutAction,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.toggleSaveAd",
      base::BindRepeating(&RewardsDOMHandler::ToggleSaveAd,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.toggleFlagAd",
      base::BindRepeating(&RewardsDOMHandler::ToggleFlagAd,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.saveAdsSetting",
      base::BindRepeating(&RewardsDOMHandler::SaveAdsSetting,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.setBackupCompleted",
      base::BindRepeating(&RewardsDOMHandler::SetBackupCompleted,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "adrbrowsiel_rewards.getPendingContributionsTotal",
      base::BindRepeating(&RewardsDOMHandler::GetPendingContributionsTotal,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "adrbrowsiel_rewards.getStatement",
      base::BindRepeating(&RewardsDOMHandler::GetStatement,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "adrbrowsiel_rewards.setInlineTippingPlatformEnabled",
      base::BindRepeating(&RewardsDOMHandler::SetInlineTippingPlatformEnabled,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.getPendingContributions",
      base::BindRepeating(&RewardsDOMHandler::GetPendingContributions,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.removePendingContribution",
      base::BindRepeating(&RewardsDOMHandler::RemovePendingContribution,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "adrbrowsiel_rewards.removeAllPendingContribution",
      base::BindRepeating(&RewardsDOMHandler::RemoveAllPendingContributions,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.getExcludedSites",
      base::BindRepeating(&RewardsDOMHandler::GetExcludedSites,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.fetchBalance",
      base::BindRepeating(&RewardsDOMHandler::FetchBalance,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "adrbrowsiel_rewards.getExternalWallet",
      base::BindRepeating(&RewardsDOMHandler::GetExternalWallet,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.processRewardsPageUrl",
      base::BindRepeating(&RewardsDOMHandler::ProcessRewardsPageUrl,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.disconnectWallet",
      base::BindRepeating(&RewardsDOMHandler::DisconnectWallet,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.onlyAnonWallet",
      base::BindRepeating(&RewardsDOMHandler::OnlyAnonWallet,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.getBalanceReport",
      base::BindRepeating(&RewardsDOMHandler::GetBalanceReport,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.getMonthlyReport",
      base::BindRepeating(&RewardsDOMHandler::GetMonthlyReport,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.getMonthlyReportIds",
      base::BindRepeating(&RewardsDOMHandler::GetAllMonthlyReportIds,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.getCountryCode",
      base::BindRepeating(&RewardsDOMHandler::GetCountryCode,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.completeReset",
      base::BindRepeating(&RewardsDOMHandler::CompleteReset,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.getPaymentId",
      base::BindRepeating(&RewardsDOMHandler::GetPaymentId,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.getWalletPassphrase",
      base::BindRepeating(&RewardsDOMHandler::GetWalletPassphrase,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.getOnboardingStatus",
      base::BindRepeating(&RewardsDOMHandler::GetOnboardingStatus,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback("adrbrowsiel_rewards.saveOnboardingResult",
      base::BindRepeating(&RewardsDOMHandler::SaveOnboardingResult,
      base::Unretained(this)));
}

void RewardsDOMHandler::Init() {
  Profile* profile = Profile::FromWebUI(web_ui());

  rewards_service_ =
      adrbrowsiel_rewards::RewardsServiceFactory::GetForProfile(profile);
  rewards_service_->StartProcess(base::DoNothing());

  ads_service_ = adrbrowsiel_ads::AdsServiceFactory::GetForProfile(profile);
}

void RewardsDOMHandler::IsInitialized(const base::ListValue* args) {
  AllowJavascript();

  if (rewards_service_ && rewards_service_->IsInitialized()) {
    CallJavascriptFunction("adrbrowsiel_rewards.initialized", base::Value(0));
  }
}

void RewardsDOMHandler::OnJavascriptAllowed() {
  if (rewards_service_) {
    rewards_service_->AddObserver(this);
  }

  if (ads_service_) {
    ads_service_->AddObserver(this);
  }
}

void RewardsDOMHandler::OnJavascriptDisallowed() {
  if (rewards_service_) {
    rewards_service_->RemoveObserver(this);
  }

  if (ads_service_) {
    ads_service_->RemoveObserver(this);
  }

  weak_factory_.InvalidateWeakPtrs();
}

void RewardsDOMHandler::GetRewardsParameters(const base::ListValue* args) {
  if (!rewards_service_)
    return;

  AllowJavascript();

  rewards_service_->GetRewardsParameters(
      base::BindOnce(&RewardsDOMHandler::OnGetRewardsParameters,
                     weak_factory_.GetWeakPtr()));
}

void RewardsDOMHandler::OnGetRewardsParameters(
    ledger::type::RewardsParametersPtr parameters) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  base::DictionaryValue data;
  if (parameters) {
    auto choices = std::make_unique<base::ListValue>();
    for (double const& choice : parameters->auto_contribute_choices) {
      choices->AppendDouble(choice);
    }

    data.SetDouble("rate", parameters->rate);
    data.SetDouble("autoContributeChoice", parameters->auto_contribute_choice);
    data.SetList("autoContributeChoices", std::move(choices));
  }
  CallJavascriptFunction("adrbrowsiel_rewards.rewardsParameters", data);
}

void RewardsDOMHandler::OnRewardsInitialized(
    adrbrowsiel_rewards::RewardsService* rewards_service) {
  if (!IsJavascriptAllowed())
    return;

  CallJavascriptFunction("adrbrowsiel_rewards.initialized", base::Value(0));
}

void RewardsDOMHandler::GetAutoContributeProperties(
    const base::ListValue* args) {
  if (!rewards_service_)
    return;

  AllowJavascript();

  rewards_service_->GetAutoContributeProperties(
      base::Bind(&RewardsDOMHandler::OnGetAutoContributeProperties,
          weak_factory_.GetWeakPtr()));
}

void RewardsDOMHandler::OnGetAutoContributeProperties(
    ledger::type::AutoContributePropertiesPtr properties) {
  if (!IsJavascriptAllowed() || !properties)
    return;

  base::DictionaryValue values;
  values.SetBoolean("enabledContribute", properties->enabled_contribute);
  values.SetInteger("contributionMinTime", properties->contribution_min_time);
  values.SetInteger("contributionMinVisits",
      properties->contribution_min_visits);
  values.SetBoolean("contributionNonVerified",
      properties->contribution_non_verified);
  values.SetBoolean("contributionVideos", properties->contribution_videos);

  CallJavascriptFunction("adrbrowsiel_rewards.autoContributeProperties", values);
}

void RewardsDOMHandler::OnFetchPromotions(
    adrbrowsiel_rewards::RewardsService* rewards_service,
    const ledger::type::Result result,
    const ledger::type::PromotionList& list) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  base::ListValue promotions;
  for (const auto& item : list) {
    auto dict = std::make_unique<base::DictionaryValue>();
    dict->SetString("promotionId", item->id);
    dict->SetInteger("type", static_cast<int>(item->type));
    dict->SetInteger("status", static_cast<int>(item->status));
    dict->SetInteger("expiresAt", item->expires_at);
    dict->SetDouble("amount", item->approximate_value);
    promotions.Append(std::move(dict));
  }

  base::DictionaryValue dict;
  dict.SetInteger("result", static_cast<int>(result));
  dict.SetKey("promotions", std::move(promotions));

  CallJavascriptFunction("adrbrowsiel_rewards.promotions", dict);
}

void RewardsDOMHandler::FetchPromotions(const base::ListValue* args) {
  if (rewards_service_) {
    AllowJavascript();
    rewards_service_->FetchPromotions();
  }
}

void RewardsDOMHandler::OnClaimPromotion(
      const std::string& promotion_id,
      const ledger::type::Result result,
      const std::string& captcha_image,
      const std::string& hint,
      const std::string& captcha_id) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  base::DictionaryValue response;
  response.SetInteger("result", static_cast<int>(result));
  response.SetString("promotionId", promotion_id);
  response.SetString("captchaImage", captcha_image);
  response.SetString("captchaId", captcha_id);
  response.SetString("hint", hint);

  CallJavascriptFunction("adrbrowsiel_rewards.claimPromotion", response);
}

void RewardsDOMHandler::ClaimPromotion(const base::ListValue* args) {
  CHECK_EQ(1U, args->GetSize());
  if (!rewards_service_) {
    return;
  }

  AllowJavascript();

  const std::string promotion_id = args->GetList()[0].GetString();

#if !defined(OS_ANDROID)
  rewards_service_->ClaimPromotion(
      promotion_id,
      base::Bind(&RewardsDOMHandler::OnClaimPromotion,
          weak_factory_.GetWeakPtr(),
          promotion_id));
#else
  // No need for a callback. The UI receives "adrbrowsiel_rewards.promotionFinish".
  adrbrowsiel_rewards::AttestPromotionCallback callback = base::DoNothing();
  rewards_service_->ClaimPromotion(promotion_id, std::move(callback));
#endif
}


void RewardsDOMHandler::AttestPromotion(const base::ListValue *args) {
  CHECK_EQ(2U, args->GetSize());
  AllowJavascript();

  if (!rewards_service_) {
    base::DictionaryValue finish;
    finish.SetInteger("status", 1);
    CallJavascriptFunction("adrbrowsiel_rewards.promotionFinish", finish);
    return;
  }

  const std::string promotion_id = args->GetList()[0].GetString();
  const std::string solution = args->GetList()[1].GetString();
  rewards_service_->AttestPromotion(
      promotion_id,
      solution,
      base::BindOnce(
        &RewardsDOMHandler::OnAttestPromotion,
        weak_factory_.GetWeakPtr(),
        promotion_id));
}

void RewardsDOMHandler::OnAttestPromotion(
    const std::string& promotion_id,
      const ledger::type::Result result,
      ledger::type::PromotionPtr promotion) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  base::DictionaryValue promotion_dict;
  promotion_dict.SetString("promotionId", promotion_id);

  if (promotion) {
    promotion_dict.SetInteger("expiresAt", promotion->expires_at);
    promotion_dict.SetDouble("amount", promotion->approximate_value);
    promotion_dict.SetInteger("type", static_cast<int>(promotion->type));
  }

  base::DictionaryValue finish;
  finish.SetInteger("result", static_cast<int>(result));
  finish.SetKey("promotion", std::move(promotion_dict));

  CallJavascriptFunction("adrbrowsiel_rewards.promotionFinish", finish);
}

void RewardsDOMHandler::OnPromotionFinished(
    adrbrowsiel_rewards::RewardsService* rewards_service,
    const ledger::type::Result result,
    ledger::type::PromotionPtr promotion) {
  if (result != ledger::type::Result::LEDGER_OK) {
    return;
  }

  OnAttestPromotion(
      promotion->id,
      result,
      promotion->Clone());
}

void RewardsDOMHandler::RecoverWallet(const base::ListValue *args) {
  CHECK_EQ(1U, args->GetSize());
  if (rewards_service_) {
    AllowJavascript();
    const std::string pass_phrase = args->GetList()[0].GetString();
    rewards_service_->RecoverWallet(pass_phrase);
  }
}

void RewardsDOMHandler::OnRecoverWallet(
    adrbrowsiel_rewards::RewardsService* rewards_service,
    const ledger::type::Result result) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  CallJavascriptFunction("adrbrowsiel_rewards.recoverWalletData",
                         base::Value(static_cast<int>(result)));
}

void RewardsDOMHandler::OnGetReconcileStamp(uint64_t reconcile_stamp) {
  if (IsJavascriptAllowed()) {
    std::string stamp = std::to_string(reconcile_stamp);
    CallJavascriptFunction("adrbrowsiel_rewards.reconcileStamp", base::Value(stamp));
  }
}

void RewardsDOMHandler::GetReconcileStamp(const base::ListValue* args) {
  if (rewards_service_) {
    AllowJavascript();
    rewards_service_->GetReconcileStamp(base::Bind(
          &RewardsDOMHandler::OnGetReconcileStamp,
          weak_factory_.GetWeakPtr()));
  }
}

void RewardsDOMHandler::OnAutoContributePropsReady(
    ledger::type::AutoContributePropertiesPtr properties) {
  auto filter = ledger::type::ActivityInfoFilter::New();
  auto pair = ledger::type::ActivityInfoFilterOrderPair::New(
      "ai.percent",
      false);
  filter->order_by.push_back(std::move(pair));
  filter->min_duration = properties->contribution_min_time;
  filter->reconcile_stamp = properties->reconcile_stamp;
  filter->excluded = ledger::type::ExcludeFilter::FILTER_ALL_EXCEPT_EXCLUDED;
  filter->percent = 1;
  filter->non_verified = properties->contribution_non_verified;
  filter->min_visits = properties->contribution_min_visits;

  rewards_service_->GetActivityInfoList(
      0,
      0,
      std::move(filter),
      base::Bind(&RewardsDOMHandler::OnPublisherList,
                 weak_factory_.GetWeakPtr()));
}

void RewardsDOMHandler::GetExcludedSites(const base::ListValue* args) {
  AllowJavascript();
  rewards_service_->GetExcludedList(
      base::Bind(&RewardsDOMHandler::OnExcludedSiteList,
          weak_factory_.GetWeakPtr()));
}

void RewardsDOMHandler::OnExcludedSitesChanged(
    adrbrowsiel_rewards::RewardsService* rewards_service,
    std::string publisher_id,
    bool excluded) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  CallJavascriptFunction("adrbrowsiel_rewards.excludedSiteChanged");
}

void RewardsDOMHandler::OnNotificationAdded(
    adrbrowsiel_rewards::RewardsNotificationService* rewards_notification_service,
    const adrbrowsiel_rewards::RewardsNotificationService::RewardsNotification&
        notification) {}

void RewardsDOMHandler::OnNotificationDeleted(
    adrbrowsiel_rewards::RewardsNotificationService* rewards_notification_service,
    const adrbrowsiel_rewards::RewardsNotificationService::RewardsNotification&
        notification) {
#if defined(OS_ANDROID)
  if (notification.type_ == adrbrowsiel_rewards::RewardsNotificationService::
                                REWARDS_NOTIFICATION_GRANT &&
      IsJavascriptAllowed()) {
    base::DictionaryValue finish;
    finish.SetInteger("status", false);
    finish.SetInteger("expiryTime", 0);
    finish.SetString("probi", "0");

    CallJavascriptFunction("adrbrowsiel_rewards.grantFinish", finish);
  }
#endif
}

void RewardsDOMHandler::OnAllNotificationsDeleted(
    adrbrowsiel_rewards::RewardsNotificationService* rewards_notification_service) {}

void RewardsDOMHandler::OnGetNotification(
    adrbrowsiel_rewards::RewardsNotificationService* rewards_notification_service,
    const adrbrowsiel_rewards::RewardsNotificationService::RewardsNotification&
        notification) {}

void RewardsDOMHandler::OnGetAllNotifications(
    adrbrowsiel_rewards::RewardsNotificationService* rewards_notification_service,
    const adrbrowsiel_rewards::RewardsNotificationService::RewardsNotificationsList&
        notifications_list) {}

void RewardsDOMHandler::SaveSetting(const base::ListValue* args) {
  CHECK_EQ(2U, args->GetSize());
  AllowJavascript();

  if (rewards_service_) {
    const std::string key = args->GetList()[0].GetString();
    const std::string value = args->GetList()[1].GetString();

    if (key == "contributionMonthly") {
      rewards_service_->SetAutoContributionAmount(std::stod(value));
    }

    if (key == "contributionMinTime") {
      int int_value;
      if (!base::StringToInt(value, &int_value)) {
        LOG(ERROR) << "Min time was not converted to int";
        return;
      }

      rewards_service_->SetPublisherMinVisitTime(int_value);
    }

    if (key == "contributionMinVisits") {
      int int_value;
      if (!base::StringToInt(value, &int_value)) {
        LOG(ERROR) << "Min visits was not converted to int";
        return;
      }

      rewards_service_->SetPublisherMinVisits(int_value);
    }

    if (key == "contributionNonVerified") {
      rewards_service_->SetPublisherAllowNonVerified(value == "true");
    }

    if (key == "contributionVideos") {
      rewards_service_->SetPublisherAllowVideos(value == "true");
    }

    if (key == "enabledContribute") {
      rewards_service_->SetAutoContributeEnabled(value == "true");
    }
  }
}

void RewardsDOMHandler::ExcludePublisher(const base::ListValue *args) {
  CHECK_EQ(1U, args->GetSize());
  if (!rewards_service_) {
    return;
  }

  AllowJavascript();
  const std::string publisherKey = args->GetList()[0].GetString();
  rewards_service_->SetPublisherExclude(publisherKey, true);
}

void RewardsDOMHandler::RestorePublishers(const base::ListValue *args) {
  if (!rewards_service_) {
    return;
  }

  AllowJavascript();
  rewards_service_->RestorePublishers();
}

void RewardsDOMHandler::RestorePublisher(const base::ListValue *args) {
  CHECK_EQ(1U, args->GetSize());
  if (!rewards_service_) {
    return;
  }

  AllowJavascript();
  std::string publisherKey = args->GetList()[0].GetString();
  rewards_service_->SetPublisherExclude(publisherKey, false);
}

void RewardsDOMHandler::OnPublisherList(ledger::type::PublisherInfoList list) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  auto publishers = std::make_unique<base::ListValue>();
  for (auto const& item : list) {
    auto publisher = std::make_unique<base::DictionaryValue>();
    publisher->SetString("id", item->id);
    publisher->SetDouble("percentage", item->percent);
    publisher->SetString("publisherKey", item->id);
    publisher->SetInteger("status", static_cast<int>(item->status));
    publisher->SetInteger("excluded", static_cast<int>(item->excluded));
    publisher->SetString("name", item->name);
    publisher->SetString("provider", item->provider);
    publisher->SetString("url", item->url);
    publisher->SetString("favIcon", item->favicon_url);
    publishers->Append(std::move(publisher));
  }

  CallJavascriptFunction("adrbrowsiel_rewards.contributeList", *publishers);
}

void RewardsDOMHandler::OnExcludedSiteList(
    ledger::type::PublisherInfoList list) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  auto publishers = std::make_unique<base::ListValue>();
  for (auto const& item : list) {
    auto publisher = std::make_unique<base::DictionaryValue>();
    publisher->SetString("id", item->id);
    publisher->SetInteger("status", static_cast<int>(item->status));
    publisher->SetString("name", item->name);
    publisher->SetString("provider", item->provider);
    publisher->SetString("url", item->url);
    publisher->SetString("favIcon", item->favicon_url);
    publishers->Append(std::move(publisher));
  }

  CallJavascriptFunction("adrbrowsiel_rewards.excludedList", *publishers);
}

void RewardsDOMHandler::OnGetContributionAmount(double amount) {
  if (IsJavascriptAllowed()) {
    CallJavascriptFunction("adrbrowsiel_rewards.contributionAmount",
                           base::Value(amount));
  }
}

void RewardsDOMHandler::GetAutoContributionAmount(const base::ListValue* args) {
  if (rewards_service_) {
    AllowJavascript();
    rewards_service_->GetAutoContributionAmount(
        base::Bind(&RewardsDOMHandler::OnGetContributionAmount,
          weak_factory_.GetWeakPtr()));
  }
}

void RewardsDOMHandler::OnReconcileComplete(
    adrbrowsiel_rewards::RewardsService* rewards_service,
    const ledger::type::Result result,
    const std::string& contribution_id,
    const double amount,
    const ledger::type::RewardsType type,
    const ledger::type::ContributionProcessor processor) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  base::DictionaryValue complete;
  complete.SetKey("result", base::Value(static_cast<int>(result)));
  complete.SetKey("type", base::Value(static_cast<int>(type)));

  CallJavascriptFunction("adrbrowsiel_rewards.reconcileComplete", complete);
}

void RewardsDOMHandler::RemoveRecurringTip(const base::ListValue *args) {
  CHECK_EQ(1U, args->GetSize());
  if (rewards_service_) {
    AllowJavascript();
    const std::string publisherKey = args->GetList()[0].GetString();
    rewards_service_->RemoveRecurringTip(publisherKey);
  }
}

void RewardsDOMHandler::GetRecurringTips(
    const base::ListValue *args) {
  if (rewards_service_) {
    AllowJavascript();
    rewards_service_->GetRecurringTips(base::BindOnce(
          &RewardsDOMHandler::OnGetRecurringTips,
          weak_factory_.GetWeakPtr()));
  }
}

void RewardsDOMHandler::OnGetRecurringTips(
    ledger::type::PublisherInfoList list) {
  if (!IsJavascriptAllowed()) {
    return;
  }
  auto publishers = std::make_unique<base::ListValue>();

  for (auto const& item : list) {
    auto publisher = std::make_unique<base::DictionaryValue>();
    publisher->SetString("id", item->id);
    publisher->SetDouble("percentage", item->weight);
    publisher->SetString("publisherKey", item->id);
    publisher->SetInteger("status", static_cast<int>(item->status));
    publisher->SetInteger("excluded", static_cast<int>(item->excluded));
    publisher->SetString("name", item->name);
    publisher->SetString("provider", item->provider);
    publisher->SetString("url", item->url);
    publisher->SetString("favIcon", item->favicon_url);
    publisher->SetInteger("tipDate", 0);
    publishers->Append(std::move(publisher));
  }

  CallJavascriptFunction("adrbrowsiel_rewards.recurringTips", *publishers);
}

void RewardsDOMHandler::OnGetOneTimeTips(ledger::type::PublisherInfoList list) {
  if (!IsJavascriptAllowed()) {
    return;
  }
  auto publishers = std::make_unique<base::ListValue>();

  for (auto const& item : list) {
    auto publisher = std::make_unique<base::DictionaryValue>();
    publisher->SetString("id", item->id);
    publisher->SetDouble("percentage", item->weight);
    publisher->SetString("publisherKey", item->id);
    publisher->SetInteger("status", static_cast<int>(item->status));
    publisher->SetInteger("excluded", static_cast<int>(item->excluded));
    publisher->SetString("name", item->name);
    publisher->SetString("provider", item->provider);
    publisher->SetString("url", item->url);
    publisher->SetString("favIcon", item->favicon_url);
    publisher->SetInteger("tipDate", item->reconcile_stamp);
    publishers->Append(std::move(publisher));
  }

  CallJavascriptFunction("adrbrowsiel_rewards.currentTips", *publishers);
}

void RewardsDOMHandler::GetOneTimeTips(const base::ListValue *args) {
  if (rewards_service_) {
    AllowJavascript();
    rewards_service_->GetOneTimeTips(base::BindOnce(
          &RewardsDOMHandler::OnGetOneTimeTips,
          weak_factory_.GetWeakPtr()));
  }
}

void RewardsDOMHandler::GetContributionList(const base::ListValue *args) {
  if (!rewards_service_) {
    return;
  }

  AllowJavascript();

  rewards_service_->GetAutoContributeProperties(
      base::Bind(&RewardsDOMHandler::OnAutoContributePropsReady,
        weak_factory_.GetWeakPtr()));
}

void RewardsDOMHandler::GetAdsData(const base::ListValue *args) {
  if (!ads_service_) {
    return;
  }

  AllowJavascript();

  base::DictionaryValue ads_data;

  auto is_supported_locale = ads_service_->IsSupportedLocale();
  ads_data.SetBoolean("adsIsSupported", is_supported_locale);

  auto is_enabled = ads_service_->IsEnabled();
  ads_data.SetBoolean("adsEnabled", is_enabled);

  auto ads_per_hour = ads_service_->GetAdsPerHour();
  ads_data.SetInteger("adsPerHour", ads_per_hour);

  const std::string subdivision_targeting_code =
      ads_service_->GetAdsSubdivisionTargetingCode();
  ads_data.SetString(kAdsSubdivisionTargeting, subdivision_targeting_code);

  const std::string auto_detected_subdivision_targeting_code =
      ads_service_->GetAutoDetectedAdsSubdivisionTargetingCode();
  ads_data.SetString(kAutoDetectedAdsSubdivisionTargeting,
      auto_detected_subdivision_targeting_code);

  const bool should_allow_subdivision_ad_targeting =
      ads_service_->ShouldAllowAdsSubdivisionTargeting();
  ads_data.SetBoolean(kShouldAllowAdsSubdivisionTargeting,
      should_allow_subdivision_ad_targeting);

#if BUILDFLAG(adrbrowsiel_ADS_ENABLED)
    auto ads_ui_enabled = true;
#else
    auto ads_ui_enabled = false;
#endif
  ads_data.SetBoolean("adsUIEnabled", ads_ui_enabled);

  CallJavascriptFunction("adrbrowsiel_rewards.adsData", ads_data);
}

void RewardsDOMHandler::GetAdsHistory(const base::ListValue* args) {
  if (!ads_service_) {
    return;
  }

  AllowJavascript();

  const base::Time to_time = base::Time::Now();
  const uint64_t to_timestamp = to_time.ToDoubleT();

  const base::Time from_time = to_time -
      base::TimeDelta::FromDays(kDaysOfAdsHistory - 1);
  const base::Time from_time_local_midnight = from_time.LocalMidnight();
  const uint64_t from_timestamp = from_time_local_midnight.ToDoubleT();

  ads_service_->GetAdsHistory(from_timestamp, to_timestamp,
      base::BindOnce(&RewardsDOMHandler::OnGetAdsHistory,
          weak_factory_.GetWeakPtr()));
}

void RewardsDOMHandler::OnGetAdsHistory(const base::ListValue& ads_history) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  CallJavascriptFunction("adrbrowsiel_rewards.adsHistory", ads_history);
}

void RewardsDOMHandler::ToggleAdThumbUp(const base::ListValue* args) {
  CHECK_EQ(3U, args->GetSize());
  if (!ads_service_) {
    return;
  }

  AllowJavascript();

  const std::string id = args->GetList()[0].GetString();
  const std::string creative_set_id = args->GetList()[1].GetString();
  const int action = args->GetList()[2].GetInt();
  ads_service_->ToggleAdThumbUp(
      id, creative_set_id, action,
      base::BindOnce(&RewardsDOMHandler::OnToggleAdThumbUp,
                     weak_factory_.GetWeakPtr()));
}

void RewardsDOMHandler::OnToggleAdThumbUp(
    const std::string& creative_instance_id,
    const int action) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  base::Value result(base::Value::Type::DICTIONARY);
  result.SetKey("creativeInstanceId", base::Value(creative_instance_id));
  result.SetKey("action", base::Value(action));
  CallJavascriptFunction("adrbrowsiel_rewards.onToggleAdThumbUp", result);
}

void RewardsDOMHandler::ToggleAdThumbDown(const base::ListValue* args) {
  CHECK_EQ(3U, args->GetSize());
  if (!ads_service_) {
    return;
  }

  AllowJavascript();

  const std::string id = args->GetList()[0].GetString();
  const std::string creative_set_id = args->GetList()[1].GetString();
  const int action = args->GetList()[2].GetInt();
  ads_service_->ToggleAdThumbDown(
      id, creative_set_id, action,
      base::BindOnce(&RewardsDOMHandler::OnToggleAdThumbDown,
                     weak_factory_.GetWeakPtr()));
}

void RewardsDOMHandler::OnToggleAdThumbDown(
    const std::string& creative_instance_id,
    const int action) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  base::Value result(base::Value::Type::DICTIONARY);
  result.SetKey("creativeInstanceId", base::Value(creative_instance_id));
  result.SetKey("action", base::Value(action));
  CallJavascriptFunction("adrbrowsiel_rewards.onToggleAdThumbDown", result);
}

void RewardsDOMHandler::ToggleAdOptInAction(const base::ListValue* args) {
  CHECK_EQ(2U, args->GetSize());
  if (!ads_service_) {
    return;
  }

  AllowJavascript();

  const std::string category = args->GetList()[0].GetString();
  const int action = args->GetList()[1].GetInt();
  ads_service_->ToggleAdOptInAction(
      category, action,
      base::BindOnce(&RewardsDOMHandler::OnToggleAdOptInAction,
                     weak_factory_.GetWeakPtr()));
}

void RewardsDOMHandler::OnToggleAdOptInAction(const std::string& category,
                                              int action) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  base::Value result(base::Value::Type::DICTIONARY);
  result.SetKey("category", base::Value(category));
  result.SetKey("action", base::Value(action));
  CallJavascriptFunction("adrbrowsiel_rewards.onToggleAdOptInAction", result);
}

void RewardsDOMHandler::ToggleAdOptOutAction(const base::ListValue* args) {
  CHECK_EQ(2U, args->GetSize());
  if (!ads_service_) {
    return;
  }

  AllowJavascript();

  const std::string category = args->GetList()[0].GetString();
  const int action = args->GetList()[1].GetInt();
  ads_service_->ToggleAdOptOutAction(
      category, action,
      base::BindOnce(&RewardsDOMHandler::OnToggleAdOptOutAction,
                     weak_factory_.GetWeakPtr()));
}

void RewardsDOMHandler::OnToggleAdOptOutAction(const std::string& category,
                                               int action) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  base::Value result(base::Value::Type::DICTIONARY);
  result.SetKey("category", base::Value(category));
  result.SetKey("action", base::Value(action));
  CallJavascriptFunction("adrbrowsiel_rewards.onToggleAdOptOutAction", result);
}

void RewardsDOMHandler::ToggleSaveAd(const base::ListValue* args) {
  CHECK_EQ(3U, args->GetSize());
  if (!ads_service_) {
    return;
  }

  AllowJavascript();

  const std::string creative_instance_id = args->GetList()[0].GetString();
  const std::string creative_set_id = args->GetList()[1].GetString();
  const bool saved = args->GetList()[2].GetBool();
  ads_service_->ToggleSaveAd(creative_instance_id, creative_set_id, saved,
      base::BindOnce(&RewardsDOMHandler::OnToggleSaveAd,
          weak_factory_.GetWeakPtr()));
}

void RewardsDOMHandler::OnToggleSaveAd(
    const std::string& creative_instance_id,
    bool saved) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  base::Value result(base::Value::Type::DICTIONARY);
  result.SetKey("creativeInstanceId", base::Value(creative_instance_id));
  result.SetKey("saved", base::Value(saved));
  CallJavascriptFunction("adrbrowsiel_rewards.onToggleSaveAd", result);
}

void RewardsDOMHandler::ToggleFlagAd(const base::ListValue* args) {
  CHECK_EQ(3U, args->GetSize());
  if (!ads_service_) {
    return;
  }

  AllowJavascript();

  const std::string creative_instance_id = args->GetList()[0].GetString();
  const std::string creative_set_id = args->GetList()[1].GetString();
  const bool flagged = args->GetList()[2].GetBool();
  ads_service_->ToggleFlagAd(creative_instance_id, creative_set_id, flagged,
      base::BindOnce(&RewardsDOMHandler::OnToggleFlagAd,
          weak_factory_.GetWeakPtr()));
}

void RewardsDOMHandler::OnToggleFlagAd(
      const std::string& creative_instance_id, bool flagged) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  base::Value result(base::Value::Type::DICTIONARY);
  result.SetKey("creativeInstanceId", base::Value(creative_instance_id));
  result.SetKey("flagged", base::Value(flagged));
  CallJavascriptFunction("adrbrowsiel_rewards.onToggleFlagAd", result);
}

void RewardsDOMHandler::SaveAdsSetting(const base::ListValue* args) {
  CHECK_EQ(2U, args->GetSize());
  if (!ads_service_) {
    return;
  }

  AllowJavascript();

  const std::string key = args->GetList()[0].GetString();
  const std::string value = args->GetList()[1].GetString();

  if (key == "adsEnabled") {
    const auto is_enabled =
        value == "true" && ads_service_->IsSupportedLocale();
    rewards_service_->SetAdsEnabled(is_enabled);
  } else if (key == "adsPerHour") {
    ads_service_->SetAdsPerHour(std::stoull(value));
  } else if (key == kAdsSubdivisionTargeting) {
    ads_service_->SetAdsSubdivisionTargetingCode(value);
  } else if (key == kAutoDetectedAdsSubdivisionTargeting) {
    ads_service_->SetAutoDetectedAdsSubdivisionTargetingCode(value);
  }

  base::ListValue* emptyArgs = nullptr;
  GetAdsData(emptyArgs);
}

void RewardsDOMHandler::SetBackupCompleted(const base::ListValue *args) {
  if (rewards_service_) {
    AllowJavascript();
    rewards_service_->SetBackupCompleted();
  }
}

void RewardsDOMHandler::GetPendingContributionsTotal(
    const base::ListValue* args) {
  if (rewards_service_) {
    AllowJavascript();
    rewards_service_->GetPendingContributionsTotal(base::Bind(
          &RewardsDOMHandler::OnGetPendingContributionsTotal,
          weak_factory_.GetWeakPtr()));
  }
}

void RewardsDOMHandler::OnGetPendingContributionsTotal(double amount) {
  if (IsJavascriptAllowed()) {
    CallJavascriptFunction("adrbrowsiel_rewards.pendingContributionTotal",
                           base::Value(amount));
  }
}

void RewardsDOMHandler::OnPendingContributionSaved(
    adrbrowsiel_rewards::RewardsService* rewards_service,
    const ledger::type::Result result) {
  if (!IsJavascriptAllowed()) {
    return;
  }
  CallJavascriptFunction("adrbrowsiel_rewards.onPendingContributionSaved",
                         base::Value(static_cast<int>(result)));
}

void RewardsDOMHandler::OnPublisherListNormalized(
    adrbrowsiel_rewards::RewardsService* rewards_service,
    ledger::type::PublisherInfoList list) {
  OnPublisherList(std::move(list));
}

void RewardsDOMHandler::GetStatement(
    const base::ListValue* args) {
  AllowJavascript();
  ads_service_->GetAccountStatement(base::Bind(
      &RewardsDOMHandler::OnGetStatement, weak_factory_.GetWeakPtr()));
}

void RewardsDOMHandler::OnGetStatement(const bool success,
                                       const double estimated_pending_rewards,
                                       const int64_t next_payment_date,
                                       const int ads_received_this_month,
                                       const double earnings_this_month,
                                       const double earnings_last_month) {
  if (!success) {
    return;
  }

  if (!IsJavascriptAllowed()) {
    return;
  }

  base::DictionaryValue history;

  history.SetDouble("adsEstimatedPendingRewards",
      estimated_pending_rewards);

  if (next_payment_date == 0) {
    history.SetString("adsNextPaymentDate", "");
  } else {
    base::Time time = base::Time::FromDoubleT(next_payment_date);
    history.SetString("adsNextPaymentDate",
        base::TimeFormatWithPattern(time, "MMMd"));
  }

  history.SetInteger("adsReceivedThisMonth", ads_received_this_month);

  CallJavascriptFunction("adrbrowsiel_rewards.statement", history);
}

void RewardsDOMHandler::OnStatementChanged(
    adrbrowsiel_rewards::RewardsService* rewards_service) {
  if (IsJavascriptAllowed()) {
    CallJavascriptFunction("adrbrowsiel_rewards.statementChanged");
  }
}

void RewardsDOMHandler::OnAdRewardsChanged() {
  ads_service_->GetAccountStatement(base::Bind(
      &RewardsDOMHandler::OnGetStatement, weak_factory_.GetWeakPtr()));
}

void RewardsDOMHandler::OnRecurringTipSaved(
    adrbrowsiel_rewards::RewardsService* rewards_service,
    bool success) {
  if (IsJavascriptAllowed()) {
    CallJavascriptFunction("adrbrowsiel_rewards.recurringTipSaved",
                           base::Value(success));
  }
}

void RewardsDOMHandler::OnRecurringTipRemoved(
    adrbrowsiel_rewards::RewardsService* rewards_service,
    bool success) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  CallJavascriptFunction("adrbrowsiel_rewards.recurringTipRemoved",
                         base::Value(success));
}

void RewardsDOMHandler::SetInlineTippingPlatformEnabled(
    const base::ListValue* args) {
  AllowJavascript();

  std::string key;
  args->GetString(0, &key);

  std::string value;
  args->GetString(1, &value);

  if (rewards_service_) {
    rewards_service_->SetInlineTippingPlatformEnabled(key, value == "true");
  }
}

void RewardsDOMHandler::GetPendingContributions(
    const base::ListValue* args) {
  if (rewards_service_) {
    AllowJavascript();
    rewards_service_->GetPendingContributions(base::Bind(
          &RewardsDOMHandler::OnGetPendingContributions,
          weak_factory_.GetWeakPtr()));
  }
}

void RewardsDOMHandler::OnGetPendingContributions(
    ledger::type::PendingContributionInfoList list) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  auto contributions = std::make_unique<base::ListValue>();
  for (auto const& item : list) {
    auto contribution =
        std::make_unique<base::Value>(base::Value::Type::DICTIONARY);
    contribution->SetKey("id", base::Value(static_cast<int>(item->id)));
    contribution->SetKey("publisherKey", base::Value(item->publisher_key));
    contribution->SetKey("status",
        base::Value(static_cast<int>(item->status)));
    contribution->SetKey("name", base::Value(item->name));
    contribution->SetKey("provider", base::Value(item->provider));
    contribution->SetKey("url", base::Value(item->url));
    contribution->SetKey("favIcon", base::Value(item->favicon_url));
    contribution->SetKey("amount", base::Value(item->amount));
    contribution->SetKey("addedDate",
        base::Value(std::to_string(item->added_date)));
    contribution->SetKey("type", base::Value(static_cast<int>(item->type)));
    contribution->SetKey("viewingId", base::Value(item->viewing_id));
    contribution->SetKey("expirationDate",
        base::Value(std::to_string(item->expiration_date)));
    contributions->Append(std::move(contribution));
  }

  CallJavascriptFunction("adrbrowsiel_rewards.pendingContributions", *contributions);
}

void RewardsDOMHandler::RemovePendingContribution(
    const base::ListValue* args) {
  CHECK_EQ(1U, args->GetSize());
  if (!rewards_service_) {
    return;
  }

  AllowJavascript();

  const uint64_t id = args->GetList()[0].GetInt();
  rewards_service_->RemovePendingContribution(id);
}

void RewardsDOMHandler::RemoveAllPendingContributions(
    const base::ListValue* args) {
  if (rewards_service_) {
    AllowJavascript();
    rewards_service_->RemoveAllPendingContributions();
  }
}

void RewardsDOMHandler::OnPendingContributionRemoved(
    adrbrowsiel_rewards::RewardsService* rewards_service,
    const ledger::type::Result result) {
  if (IsJavascriptAllowed()) {
    CallJavascriptFunction("adrbrowsiel_rewards.onRemovePendingContribution",
                           base::Value(static_cast<int>(result)));
  }
}

void RewardsDOMHandler::OnFetchBalance(
    const ledger::type::Result result,
    ledger::type::BalancePtr balance) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  base::Value balance_value(base::Value::Type::DICTIONARY);

  if (balance) {
    balance_value.SetDoubleKey("total", balance->total);

    if (result == ledger::type::Result::LEDGER_OK) {
      base::Value wallets(base::Value::Type::DICTIONARY);
      for (auto const& wallet : balance->wallets) {
        wallets.SetDoubleKey(wallet.first, wallet.second);
      }
      balance_value.SetKey("wallets", std::move(wallets));
    }
  } else {
    balance_value.SetDoubleKey("total", 0.0);
  }

  base::DictionaryValue data;
  data.SetIntKey("status", static_cast<int>(result));
  data.SetKey("balance", std::move(balance_value));
  CallJavascriptFunction("adrbrowsiel_rewards.balance", data);
}

void RewardsDOMHandler::FetchBalance(const base::ListValue* args) {
  if (rewards_service_) {
    AllowJavascript();
    rewards_service_->FetchBalance(base::BindOnce(
          &RewardsDOMHandler::OnFetchBalance,
          weak_factory_.GetWeakPtr()));
  }
}

void RewardsDOMHandler::GetExternalWallet(const base::ListValue* args) {
  if (!rewards_service_) {
    return;
  }

  AllowJavascript();
  rewards_service_->GetExternalWallet(base::BindOnce(
      &RewardsDOMHandler::OnGetExternalWallet, weak_factory_.GetWeakPtr()));
}

void RewardsDOMHandler::OnGetExternalWallet(
    const ledger::type::Result result,
    ledger::type::ExternalWalletPtr wallet) {
  if (IsJavascriptAllowed()) {
    base::Value data(base::Value::Type::DICTIONARY);

    data.SetIntKey("result", static_cast<int>(result));
    base::Value wallet_dict(base::Value::Type::DICTIONARY);

    if (wallet) {
      wallet_dict.SetStringKey("type", wallet->type);
      wallet_dict.SetStringKey("address", wallet->address);
      wallet_dict.SetIntKey("status", static_cast<int>(wallet->status));
      wallet_dict.SetStringKey("verifyUrl", wallet->verify_url);
      wallet_dict.SetStringKey("addUrl", wallet->add_url);
      wallet_dict.SetStringKey("withdrawUrl", wallet->withdraw_url);
      wallet_dict.SetStringKey("userName", wallet->user_name);
      wallet_dict.SetStringKey("accountUrl", wallet->account_url);
      wallet_dict.SetStringKey("loginUrl", wallet->login_url);
    }

    data.SetKey("wallet", std::move(wallet_dict));

    CallJavascriptFunction("adrbrowsiel_rewards.externalWallet", data);
  }
}

void RewardsDOMHandler::OnProcessRewardsPageUrl(
    const ledger::type::Result result,
    const std::string& wallet_type,
    const std::string& action,
    const base::flat_map<std::string, std::string>& args) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  base::Value data(base::Value::Type::DICTIONARY);
  data.SetIntKey("result", static_cast<int>(result));
  data.SetStringKey("walletType", wallet_type);
  data.SetStringKey("action", action);

  base::Value new_args(base::Value::Type::DICTIONARY);
  for (auto const& arg : args) {
    new_args.SetStringKey(arg.first, arg.second);
  }
  data.SetKey("args", std::move(new_args));

  CallJavascriptFunction("adrbrowsiel_rewards.processRewardsPageUrl", data);
}

void RewardsDOMHandler::ProcessRewardsPageUrl(const base::ListValue* args) {
  CHECK_EQ(2U, args->GetSize());
  if (!rewards_service_) {
    return;
  }

  AllowJavascript();
  const std::string path = args->GetList()[0].GetString();
  const std::string query = args->GetList()[1].GetString();
  rewards_service_->ProcessRewardsPageUrl(
      path,
      query,
      base::BindOnce(&RewardsDOMHandler::OnProcessRewardsPageUrl,
                     weak_factory_.GetWeakPtr()));
}

void RewardsDOMHandler::DisconnectWallet(const base::ListValue* args) {
  if (!rewards_service_) {
    return;
  }
  AllowJavascript();
  rewards_service_->DisconnectWallet();
}

void RewardsDOMHandler::OnDisconnectWallet(
      adrbrowsiel_rewards::RewardsService* rewards_service,
      const ledger::type::Result result,
      const std::string& wallet_type) {
  base::Value data(base::Value::Type::DICTIONARY);
  data.SetIntKey("result", static_cast<int>(result));
  data.SetStringKey("walletType", wallet_type);

  CallJavascriptFunction("adrbrowsiel_rewards.disconnectWallet", data);
}

void RewardsDOMHandler::OnAdsEnabled(
    adrbrowsiel_rewards::RewardsService* rewards_service,
    bool ads_enabled) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  base::ListValue* emptyArgs = nullptr;
  GetAdsData(emptyArgs);
  GetAutoContributeProperties(emptyArgs);
  GetOnboardingStatus(emptyArgs);
}

void RewardsDOMHandler::OnlyAnonWallet(const base::ListValue* args) {
  if (!rewards_service_) {
    return;
  }

  AllowJavascript();

  const bool allow = rewards_service_->OnlyAnonWallet();

  CallJavascriptFunction("adrbrowsiel_rewards.onlyAnonWallet", base::Value(allow));
}

void RewardsDOMHandler::OnUnblindedTokensReady(
    adrbrowsiel_rewards::RewardsService* rewards_service) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  CallJavascriptFunction("adrbrowsiel_rewards.unblindedTokensReady");
}

void RewardsDOMHandler::ReconcileStampReset() {
  if (!IsJavascriptAllowed()) {
    return;
  }

  CallJavascriptFunction("adrbrowsiel_rewards.reconcileStampReset");
}

void RewardsDOMHandler::OnGetBalanceReport(
    const uint32_t month,
    const uint32_t year,
    const ledger::type::Result result,
    ledger::type::BalanceReportInfoPtr report) {
  if (!IsJavascriptAllowed() || !report) {
    return;
  }

  base::Value report_base(base::Value::Type::DICTIONARY);
  report_base.SetDoubleKey("grant", report->grants);
  report_base.SetDoubleKey("ads", report->earning_from_ads);
  report_base.SetDoubleKey("contribute", report->auto_contribute);
  report_base.SetDoubleKey("monthly", report->recurring_donation);
  report_base.SetDoubleKey("tips", report->one_time_donation);

  base::Value data(base::Value::Type::DICTIONARY);
  data.SetIntKey("month", month);
  data.SetIntKey("year", year);
  data.SetKey("report", std::move(report_base));

  CallJavascriptFunction("adrbrowsiel_rewards.balanceReport", data);
}

void RewardsDOMHandler::GetBalanceReport(const base::ListValue* args) {
  CHECK_EQ(2U, args->GetSize());
  if (!rewards_service_) {
    return;
  }

  AllowJavascript();

  const uint32_t month = args->GetList()[0].GetInt();
  const uint32_t year = args->GetList()[1].GetInt();
  rewards_service_->GetBalanceReport(
      month,
      year,
      base::BindOnce(&RewardsDOMHandler::OnGetBalanceReport,
                     weak_factory_.GetWeakPtr(),
                     month,
                     year));
}

void RewardsDOMHandler::OnGetMonthlyReport(
    const uint32_t month,
    const uint32_t year,
    ledger::type::MonthlyReportInfoPtr report) {
  if (!IsJavascriptAllowed() || !report) {
    return;
  }

  base::Value data(base::Value::Type::DICTIONARY);
  data.SetIntKey("month", month);
  data.SetIntKey("year", year);

  base::Value balance_report(base::Value::Type::DICTIONARY);
  balance_report.SetDoubleKey("grant", report->balance->grants);
  balance_report.SetDoubleKey("ads", report->balance->earning_from_ads);
  balance_report.SetDoubleKey("contribute", report->balance->auto_contribute);
  balance_report.SetDoubleKey("monthly", report->balance->recurring_donation);
  balance_report.SetDoubleKey("tips", report->balance->one_time_donation);

  base::Value transactions(base::Value::Type::LIST);
  for (const auto& item : report->transactions) {
    base::Value transaction_report(base::Value::Type::DICTIONARY);
    transaction_report.SetDoubleKey("amount", item->amount);
    transaction_report.SetIntKey("type", static_cast<int>(item->type));
    transaction_report.SetIntKey(
        "processor",
        static_cast<int>(item->processor));
    transaction_report.SetIntKey("created_at", item->created_at);

    transactions.Append(std::move(transaction_report));
  }

  base::Value contributions(base::Value::Type::LIST);
  for (const auto& contribution : report->contributions) {
    base::Value publishers(base::Value::Type::LIST);
    for (const auto& item : contribution->publishers) {
      base::Value publisher(base::Value::Type::DICTIONARY);
      publisher.SetStringKey("id", item->id);
      publisher.SetDoubleKey("percentage", item->percent);
      publisher.SetDoubleKey("weight", item->weight);
      publisher.SetStringKey("publisherKey", item->id);
      publisher.SetIntKey("status", static_cast<int>(item->status));
      publisher.SetStringKey("name", item->name);
      publisher.SetStringKey("provider", item->provider);
      publisher.SetStringKey("url", item->url);
      publisher.SetStringKey("favIcon", item->favicon_url);
      publishers.Append(std::move(publisher));
    }

    base::Value contribution_report(base::Value::Type::DICTIONARY);
    contribution_report.SetDoubleKey("amount", contribution->amount);
    contribution_report.SetIntKey("type", static_cast<int>(contribution->type));
    contribution_report.SetIntKey(
        "processor",
        static_cast<int>(contribution->processor));
    contribution_report.SetIntKey("created_at", contribution->created_at);
    contribution_report.SetKey("publishers", std::move(publishers));
    contributions.Append(std::move(contribution_report));
  }

  base::Value report_base(base::Value::Type::DICTIONARY);
  report_base.SetKey("balance", std::move(balance_report));
  report_base.SetKey("transactions", std::move(transactions));
  report_base.SetKey("contributions", std::move(contributions));

  data.SetKey("report", std::move(report_base));

  CallJavascriptFunction("adrbrowsiel_rewards.monthlyReport", data);
}

void RewardsDOMHandler::GetMonthlyReport(const base::ListValue* args) {
  CHECK_EQ(2U, args->GetSize());
  if (!rewards_service_) {
    return;
  }

  AllowJavascript();

  const uint32_t month = args->GetList()[0].GetInt();
  const uint32_t year = args->GetList()[1].GetInt();

  rewards_service_->GetMonthlyReport(
      month,
      year,
      base::BindOnce(&RewardsDOMHandler::OnGetMonthlyReport,
          weak_factory_.GetWeakPtr(),
          month,
          year));
}

void RewardsDOMHandler::OnGetAllMonthlyReportIds(
    const std::vector<std::string>& ids) {
  base::Value list(base::Value::Type::LIST);
  for (const auto& item : ids) {
    list.Append(base::Value(item));
  }

  CallJavascriptFunction("adrbrowsiel_rewards.monthlyReportIds", list);
}

void RewardsDOMHandler::GetAllMonthlyReportIds(const base::ListValue* args) {
  if (!rewards_service_) {
    return;
  }

  AllowJavascript();

  rewards_service_->GetAllMonthlyReportIds(
      base::BindOnce(&RewardsDOMHandler::OnGetAllMonthlyReportIds,
          weak_factory_.GetWeakPtr()));
}

void RewardsDOMHandler::GetCountryCode(const base::ListValue* args) {
  AllowJavascript();

  const std::string locale =
      adrbrowsiel_l10n::LocaleHelper::GetInstance()->GetLocale();
  const std::string country_code = adrbrowsiel_l10n::GetCountryCode(locale);

  CallJavascriptFunction("adrbrowsiel_rewards.countryCode",
                         base::Value(country_code));
}

void RewardsDOMHandler::CompleteReset(const base::ListValue* args) {
  if (!rewards_service_) {
    return;
  }

  AllowJavascript();
  rewards_service_->CompleteReset(base::DoNothing());
}

void RewardsDOMHandler::OnCompleteReset(const bool success) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  CallJavascriptFunction("adrbrowsiel_rewards.completeReset", base::Value(success));
}

void RewardsDOMHandler::GetPaymentId(const base::ListValue* args) {
  if (!rewards_service_) {
    return;
  }

  AllowJavascript();

  // Ensure that a wallet has been created for the user before attempting
  // to retrieve a payment ID.
  rewards_service_->CreateWallet(
      base::BindOnce(&RewardsDOMHandler::OnWalletCreatedForPaymentId,
          weak_factory_.GetWeakPtr()));
}

void RewardsDOMHandler::OnWalletCreatedForPaymentId(
    ledger::type::Result result) {
  rewards_service_->GetadrbrowsielWallet(
      base::BindOnce(&RewardsDOMHandler::OnGetPaymentId,
          weak_factory_.GetWeakPtr()));
}

void RewardsDOMHandler::OnGetPaymentId(ledger::type::adrbrowsielWalletPtr wallet) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  std::string payment_id;
  if (wallet) {
    payment_id = wallet->payment_id;
  }

  CallJavascriptFunction("adrbrowsiel_rewards.paymentId", base::Value(payment_id));
}

void RewardsDOMHandler::GetWalletPassphrase(const base::ListValue* args) {
  if (!rewards_service_) {
    return;
  }

  AllowJavascript();
  rewards_service_->GetWalletPassphrase(
      base::Bind(&RewardsDOMHandler::OnGetWalletPassphrase,
          weak_factory_.GetWeakPtr()));
}

void RewardsDOMHandler::OnGetWalletPassphrase(const std::string& passphrase) {
  if (!IsJavascriptAllowed()) {
    return;
  }

  CallJavascriptFunction("adrbrowsiel_rewards.walletPassphrase",
                         base::Value(passphrase));
}

void RewardsDOMHandler::GetOnboardingStatus(const base::ListValue* args) {
  if (!rewards_service_) {
    return;
  }
  AllowJavascript();
  base::Value data(base::Value::Type::DICTIONARY);
  data.SetBoolKey("showOnboarding", rewards_service_->ShouldShowOnboarding());
  CallJavascriptFunction("adrbrowsiel_rewards.onboardingStatus", data);
}

void RewardsDOMHandler::SaveOnboardingResult(const base::ListValue* args) {
  CHECK_EQ(1U, args->GetSize());
  if (!rewards_service_)
    return;

  AllowJavascript();
  if (args->GetList()[0].GetString() == "opted-in")
    rewards_service_->EnableRewards();
}

}  // namespace

adrbrowsielRewardsPageUI::adrbrowsielRewardsPageUI(content::WebUI* web_ui,
                                       const std::string& name)
    : WebUIController(web_ui) {
  CreateAndAddWebUIDataSource(web_ui, name, kadrbrowsielRewardsPageGenerated,
                              kadrbrowsielRewardsPageGeneratedSize,
#if defined(OS_ANDROID)
                              IDR_adrbrowsiel_REWARDS_ANDROID_PAGE_HTML,
#else
                              IDR_adrbrowsiel_REWARDS_PAGE_HTML,
#endif
                              /*disable_trusted_types_csp=*/true);
  auto handler_owner = std::make_unique<RewardsDOMHandler>();
  RewardsDOMHandler * handler = handler_owner.get();
  web_ui->AddMessageHandler(std::move(handler_owner));
  handler->Init();
}

adrbrowsielRewardsPageUI::~adrbrowsielRewardsPageUI() {}