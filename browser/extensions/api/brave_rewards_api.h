/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_EXTENSIONS_API_adrbrowsiel_REWARDS_API_H_
#define adrbrowsiel_BROWSER_EXTENSIONS_API_adrbrowsiel_REWARDS_API_H_

#include <map>
#include <memory>
#include <string>

#include "base/memory/weak_ptr.h"
#include "adrbrowsiel/vendor/bat-native-ledger/include/bat/ledger/mojom_structs.h"
#include "extensions/browser/extension_function.h"

namespace extensions {
namespace api {

class adrbrowsielRewardsOpenBrowserActionUIFunction :
    public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.openBrowserActionUI", UNKNOWN)

 protected:
  ~adrbrowsielRewardsOpenBrowserActionUIFunction() override;

  ResponseAction Run() override;
};

class adrbrowsielRewardsUpdateMediaDurationFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.updateMediaDuration", UNKNOWN)

 protected:
  ~adrbrowsielRewardsUpdateMediaDurationFunction() override;

  ResponseAction Run() override;
};

class adrbrowsielRewardsGetPublisherInfoFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.getPublisherInfo", UNKNOWN)

 protected:
  ~adrbrowsielRewardsGetPublisherInfoFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetPublisherInfo(
      const ledger::type::Result result,
      ledger::type::PublisherInfoPtr info);
};

class adrbrowsielRewardsGetPublisherPanelInfoFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.getPublisherPanelInfo", UNKNOWN)

 protected:
  ~adrbrowsielRewardsGetPublisherPanelInfoFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetPublisherPanelInfo(
      const ledger::type::Result result,
      ledger::type::PublisherInfoPtr info);
};

class adrbrowsielRewardsSavePublisherInfoFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.savePublisherInfo", UNKNOWN)

 protected:
  ~adrbrowsielRewardsSavePublisherInfoFunction() override;

  ResponseAction Run() override;

 private:
  void OnSavePublisherInfo(const ledger::type::Result result);
};

class adrbrowsielRewardsTipSiteFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.tipSite", UNKNOWN)

 protected:
  ~adrbrowsielRewardsTipSiteFunction() override;

  ResponseAction Run() override;
};

class adrbrowsielRewardsTipUserFunction : public ExtensionFunction {
 public:
  adrbrowsielRewardsTipUserFunction();
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.tipUser", UNKNOWN)

 protected:
  ~adrbrowsielRewardsTipUserFunction() override;

  ResponseAction Run() override;

 private:
  void OnProcessStarted(const std::string& publisher_key);
  void OnTipUserGetPublisherInfo(
      const ledger::type::Result result,
      ledger::type::PublisherInfoPtr info);
  void OnTipUserSavePublisherInfo(const ledger::type::Result result);
  void ShowTipDialog();

  base::WeakPtrFactory<adrbrowsielRewardsTipUserFunction> weak_factory_;
};

class adrbrowsielRewardsGetPublisherDataFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.getPublisherData", UNKNOWN)

 protected:
  ~adrbrowsielRewardsGetPublisherDataFunction() override;

  ResponseAction Run() override;
};

class adrbrowsielRewardsGetRewardsParametersFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.getRewardsParameters", UNKNOWN)

 protected:
  ~adrbrowsielRewardsGetRewardsParametersFunction() override;

  ResponseAction Run() override;

 private:
  void OnGet(ledger::type::RewardsParametersPtr parameters);
};

class adrbrowsielRewardsGetBalanceReportFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.getBalanceReport", UNKNOWN)

 protected:
  ~adrbrowsielRewardsGetBalanceReportFunction() override;

  ResponseAction Run() override;

 private:
  void OnBalanceReport(
      const ledger::type::Result result,
      ledger::type::BalanceReportInfoPtr report);
};

class adrbrowsielRewardsIncludeInAutoContributionFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.includeInAutoContribution", UNKNOWN)

 protected:
  ~adrbrowsielRewardsIncludeInAutoContributionFunction() override;

  ResponseAction Run() override;
};

class adrbrowsielRewardsFetchPromotionsFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.fetchPromotions", UNKNOWN)

 protected:
  ~adrbrowsielRewardsFetchPromotionsFunction() override;

  ResponseAction Run() override;
};

class adrbrowsielRewardsClaimPromotionFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.claimPromotion", UNKNOWN)

 protected:
  ~adrbrowsielRewardsClaimPromotionFunction() override;

  ResponseAction Run() override;

 private:
  void OnClaimPromotion(
      const std::string& promotion_id,
      const ledger::type::Result result,
      const std::string& captcha_image,
      const std::string& hint,
      const std::string& captcha_id);
};

class adrbrowsielRewardsAttestPromotionFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.attestPromotion", UNKNOWN)

 protected:
  ~adrbrowsielRewardsAttestPromotionFunction() override;

  ResponseAction Run() override;

 private:
  void OnAttestPromotion(
      const std::string& promotion_id,
      const ledger::type::Result result,
      ledger::type::PromotionPtr promotion);
};

class adrbrowsielRewardsGetPendingContributionsTotalFunction
    : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION(
      "adrbrowsielRewards.getPendingContributionsTotal", UNKNOWN)

 protected:
  ~adrbrowsielRewardsGetPendingContributionsTotalFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetPendingTotal(double amount);
};

class adrbrowsielRewardsSaveAdsSettingFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.saveAdsSetting", UNKNOWN)

 protected:
  ~adrbrowsielRewardsSaveAdsSettingFunction() override;

  ResponseAction Run() override;
};

class adrbrowsielRewardsSetAutoContributeEnabledFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.setAutoContributeEnabled", UNKNOWN)

 protected:
  ~adrbrowsielRewardsSetAutoContributeEnabledFunction() override;

  ResponseAction Run() override;
};

class adrbrowsielRewardsGetACEnabledFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.getACEnabled", UNKNOWN)

 protected:
  ~adrbrowsielRewardsGetACEnabledFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetACEnabled(bool enabled);
};

class adrbrowsielRewardsSaveRecurringTipFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.saveRecurringTip", UNKNOWN)

 protected:
  ~adrbrowsielRewardsSaveRecurringTipFunction() override;

  ResponseAction Run() override;

 private:
  void OnSaveRecurringTip(bool success);
};

class adrbrowsielRewardsRemoveRecurringTipFunction :
  public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.removeRecurringTip", UNKNOWN)

 protected:
  ~adrbrowsielRewardsRemoveRecurringTipFunction() override;

  ResponseAction Run() override;
};

class adrbrowsielRewardsGetRecurringTipsFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.getRecurringTips", UNKNOWN)

 protected:
  ~adrbrowsielRewardsGetRecurringTipsFunction() override;

  ResponseAction Run() override;

 private:
    void OnGetRecurringTips(ledger::type::PublisherInfoList list);
};

class adrbrowsielRewardsGetPublisherBannerFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION(
      "adrbrowsielRewards.getPublisherBanner", UNKNOWN)

 protected:
  ~adrbrowsielRewardsGetPublisherBannerFunction() override;

  ResponseAction Run() override;

 private:
  void OnPublisherBanner(ledger::type::PublisherBannerPtr banner);
};

class adrbrowsielRewardsRefreshPublisherFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.refreshPublisher", UNKNOWN)

 protected:
  ~adrbrowsielRewardsRefreshPublisherFunction() override;

  ResponseAction Run() override;

 private:
  void OnRefreshPublisher(
      const ledger::type::PublisherStatus status,
      const std::string& publisher_key);
};

class adrbrowsielRewardsGetAllNotificationsFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.getAllNotifications", UNKNOWN)

 protected:
  ~adrbrowsielRewardsGetAllNotificationsFunction() override;

  ResponseAction Run() override;
};

class adrbrowsielRewardsGetInlineTippingPlatformEnabledFunction :
    public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION(
      "adrbrowsielRewards.getInlineTippingPlatformEnabled",
      UNKNOWN)

 protected:
  ~adrbrowsielRewardsGetInlineTippingPlatformEnabledFunction() override;

  ResponseAction Run() override;

 private:
  void OnInlineTipSetting(bool value);
};

class adrbrowsielRewardsFetchBalanceFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.fetchBalance", UNKNOWN)

 protected:
  ~adrbrowsielRewardsFetchBalanceFunction() override;

  ResponseAction Run() override;

 private:
  void OnBalance(
      const ledger::type::Result result,
      ledger::type::BalancePtr balance);
};

class adrbrowsielRewardsGetExternalWalletFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.getExternalWallet", UNKNOWN)

 protected:
  ~adrbrowsielRewardsGetExternalWalletFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetExternalWallet(const ledger::type::Result result,
                           ledger::type::ExternalWalletPtr wallet);
};

class adrbrowsielRewardsDisconnectWalletFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.disconnectWallet", UNKNOWN)

 protected:
  ~adrbrowsielRewardsDisconnectWalletFunction() override;

  ResponseAction Run() override;
};

class adrbrowsielRewardsOnlyAnonWalletFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.onlyAnonWallet", UNKNOWN)

 protected:
  ~adrbrowsielRewardsOnlyAnonWalletFunction() override;

  ResponseAction Run() override;
};

class adrbrowsielRewardsGetAdsEnabledFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.getAdsEnabled", UNKNOWN)

 protected:
  ~adrbrowsielRewardsGetAdsEnabledFunction() override;

  ResponseAction Run() override;
};

class adrbrowsielRewardsGetAdsAccountStatementFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.getAdsAccountStatement", UNKNOWN)

 protected:
  ~adrbrowsielRewardsGetAdsAccountStatementFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetAdsAccountStatement(const bool success,
                                const double estimated_pending_rewards,
                                const int64_t next_payment_date,
                                const int ads_received_this_month,
                                const double earnings_this_month,
                                const double earnings_last_month);
};

class adrbrowsielRewardsGetAdsSupportedFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.getAdsSupported", UNKNOWN)

 protected:
  ~adrbrowsielRewardsGetAdsSupportedFunction() override;

  ResponseAction Run() override;
};

class adrbrowsielRewardsGetAnonWalletStatusFunction
    : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.getAnonWalletStatus", UNKNOWN)

 protected:
  ~adrbrowsielRewardsGetAnonWalletStatusFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetAnonWalletStatus(const ledger::type::Result result);
};

class adrbrowsielRewardsIsInitializedFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.isInitialized", UNKNOWN)

 protected:
  ~adrbrowsielRewardsIsInitializedFunction() override;

  ResponseAction Run() override;
};

class adrbrowsielRewardsShouldShowOnboardingFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.shouldShowOnboarding", UNKNOWN)

 protected:
  ~adrbrowsielRewardsShouldShowOnboardingFunction() override;

  ResponseAction Run() override;
};

class adrbrowsielRewardsEnableRewardsFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.enableRewards", UNKNOWN)

 protected:
  ~adrbrowsielRewardsEnableRewardsFunction() override;

  ResponseAction Run() override;
};

class adrbrowsielRewardsGetPrefsFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.getPrefs", UNKNOWN)

 protected:
  ~adrbrowsielRewardsGetPrefsFunction() override;

  ResponseAction Run() override;

 private:
  void GetAutoContributePropertiesCallback(
      ledger::type::AutoContributePropertiesPtr properties);
};

class adrbrowsielRewardsUpdatePrefsFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielRewards.updatePrefs", UNKNOWN)

 protected:
  ~adrbrowsielRewardsUpdatePrefsFunction() override;

  ResponseAction Run() override;
};

}  // namespace api
}  // namespace extensions

#endif  // adrbrowsiel_BROWSER_EXTENSIONS_API_adrbrowsiel_REWARDS_API_H_
