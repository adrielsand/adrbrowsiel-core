/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <map>

#include "base/containers/flat_map.h"
#include "base/files/scoped_temp_dir.h"
#include "adrbrowsiel/browser/adrbrowsiel_ads/ads_service_factory.h"
#include "adrbrowsiel/browser/adrbrowsiel_rewards/rewards_service_factory.h"
#include "adrbrowsiel/components/adrbrowsiel_ads/browser/test_util.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/rewards_service_private_observer.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/common/pref_names.h"
#include "chrome/browser/profiles/profile.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- adrbrowsiel_unit_tests --filter=AdsServiceTest.*

using adrbrowsiel_ads::AdsService;
using adrbrowsiel_ads::AdsServiceFactory;
using adrbrowsiel_rewards::RewardsService;
using adrbrowsiel_rewards::RewardsServiceFactory;
using adrbrowsiel_rewards::RewardsServicePrivateObserver;

using ::testing::_;

class MockRewardsService : public RewardsService {
 public:
  MockRewardsService() {}
  ~MockRewardsService() {}

  MOCK_METHOD0(IsInitialized, bool());
  MOCK_METHOD1(GetRewardsParameters,
               void(adrbrowsiel_rewards::GetRewardsParametersCallback callback));
  MOCK_METHOD4(GetActivityInfoList,
               void(const uint32_t,
                    const uint32_t,
                    ledger::type::ActivityInfoFilterPtr,
                    const adrbrowsiel_rewards::GetPublisherInfoListCallback&));
  MOCK_METHOD1(GetExcludedList,
               void(const adrbrowsiel_rewards::GetPublisherInfoListCallback&));
  MOCK_METHOD0(FetchPromotions, void());
  MOCK_METHOD2(ClaimPromotion,
               void(const std::string&, adrbrowsiel_rewards::ClaimPromotionCallback));
  MOCK_METHOD2(ClaimPromotion,
               void(const std::string&,
                    adrbrowsiel_rewards::AttestPromotionCallback));
  MOCK_METHOD3(AttestPromotion,
               void(const std::string&,
                    const std::string&,
                    adrbrowsiel_rewards::AttestPromotionCallback));
  MOCK_METHOD1(RecoverWallet, void(const std::string&));
  MOCK_METHOD0(RestorePublishers, void());
  MOCK_METHOD2(OnLoad, void(SessionID, const GURL&));
  MOCK_METHOD1(OnUnload, void(SessionID));
  MOCK_METHOD1(OnShow, void(SessionID));
  MOCK_METHOD1(OnHide, void(SessionID));
  MOCK_METHOD1(OnForeground, void(SessionID));
  MOCK_METHOD1(OnBackground, void(SessionID));
  MOCK_METHOD4(OnXHRLoad,
               void(SessionID, const GURL&, const GURL&, const GURL&));
  MOCK_METHOD5(OnPostData,
               void(SessionID,
                    const GURL&,
                    const GURL&,
                    const GURL&,
                    const std::string&));
  MOCK_METHOD1(GetReconcileStamp,
               void(const adrbrowsiel_rewards::GetReconcileStampCallback&));
  MOCK_METHOD1(GetPublisherMinVisitTime,
               void(const adrbrowsiel_rewards::GetPublisherMinVisitTimeCallback&));
  MOCK_CONST_METHOD1(SetPublisherMinVisitTime, void(int));
  MOCK_METHOD1(GetPublisherMinVisits,
               void(const adrbrowsiel_rewards::GetPublisherMinVisitsCallback&));
  MOCK_CONST_METHOD1(SetPublisherMinVisits, void(int));
  MOCK_METHOD1(
      GetPublisherAllowNonVerified,
      void(const adrbrowsiel_rewards::GetPublisherAllowNonVerifiedCallback&));
  MOCK_CONST_METHOD1(SetPublisherAllowNonVerified, void(bool));
  MOCK_METHOD1(GetPublisherAllowVideos,
               void(const adrbrowsiel_rewards::GetPublisherAllowVideosCallback&));
  MOCK_CONST_METHOD1(SetPublisherAllowVideos, void(bool));
  MOCK_CONST_METHOD1(SetAutoContributionAmount, void(double));
  MOCK_METHOD1(GetAutoContributeEnabled,
               void(adrbrowsiel_rewards::GetAutoContributeEnabledCallback));
  MOCK_METHOD1(SetAutoContributeEnabled, void(bool));
  MOCK_CONST_METHOD0(ShouldShowOnboarding, bool());
  MOCK_METHOD0(EnableRewards, void());
  MOCK_METHOD2(SetTimer, void(uint64_t, uint32_t*));
  MOCK_METHOD4(GetPublisherActivityFromUrl,
               void(uint64_t,
                    const std::string&,
                    const std::string&,
                    const std::string&));
  MOCK_METHOD1(GetAutoContributionAmount,
               void(const adrbrowsiel_rewards::GetAutoContributionAmountCallback&));
  MOCK_METHOD2(GetPublisherBanner,
               void(const std::string&,
                    adrbrowsiel_rewards::GetPublisherBannerCallback));
  MOCK_METHOD3(OnTip, void(const std::string&, double, bool));
  MOCK_METHOD4(OnTip,
               void(const std::string&,
                    double,
                    bool,
                    ledger::type::PublisherInfoPtr publisher));
  MOCK_METHOD1(RemoveRecurringTip, void(const std::string&));
  MOCK_METHOD1(GetRecurringTips, void(adrbrowsiel_rewards::GetRecurringTipsCallback));
  MOCK_METHOD1(GetOneTimeTips, void(adrbrowsiel_rewards::GetOneTimeTipsCallback));
  MOCK_METHOD2(SetPublisherExclude, void(const std::string&, bool));
  MOCK_CONST_METHOD0(GetNotificationService,
                     adrbrowsiel_rewards::RewardsNotificationService*());
  MOCK_METHOD0(CheckImported, bool());
  MOCK_METHOD0(SetBackupCompleted, void());
  MOCK_METHOD1(GetAutoContributeProperties,
               void(const adrbrowsiel_rewards::GetAutoContributePropertiesCallback&));
  MOCK_METHOD1(
      GetPendingContributionsTotal,
      void(const adrbrowsiel_rewards::GetPendingContributionsTotalCallback&));
  MOCK_METHOD1(GetRewardsInternalsInfo,
               void(adrbrowsiel_rewards::GetRewardsInternalsInfoCallback));
  MOCK_METHOD3(SaveRecurringTip,
               void(const std::string&,
                    const double,
                    adrbrowsiel_rewards::SaveRecurringTipCallback));
  MOCK_METHOD2(RefreshPublisher,
               void(const std::string&,
                    adrbrowsiel_rewards::RefreshPublisherCallback));
  MOCK_METHOD1(OnAdsEnabled, void(bool));
  MOCK_METHOD0(GetAllNotifications,
               const adrbrowsiel_rewards::RewardsNotificationService::
                   RewardsNotificationsMap&());
  MOCK_METHOD3(SaveInlineMediaInfo,
               void(const std::string&,
                    const base::flat_map<std::string, std::string>&,
                    adrbrowsiel_rewards::SaveMediaInfoCallback));
  MOCK_METHOD4(
      UpdateMediaDuration,
      void(const uint64_t, const std::string&, const uint64_t, const bool));
  MOCK_METHOD2(GetPublisherInfo,
               void(const std::string&,
                    adrbrowsiel_rewards::GetPublisherInfoCallback callback));
  MOCK_METHOD2(GetPublisherPanelInfo,
               void(const std::string&,
                    adrbrowsiel_rewards::GetPublisherInfoCallback callback));
  MOCK_METHOD3(SavePublisherInfo,
               void(const uint64_t,
                    ledger::type::PublisherInfoPtr,
                    adrbrowsiel_rewards::SavePublisherInfoCallback callback));
  MOCK_METHOD2(SetInlineTippingPlatformEnabled,
               void(const std::string& key, bool enabled));
  MOCK_METHOD2(GetInlineTippingPlatformEnabled,
               void(const std::string& key,
                    adrbrowsiel_rewards::GetInlineTippingPlatformEnabledCallback));
  MOCK_METHOD2(GetShareURL,
               void(const base::flat_map<std::string, std::string>& args,
                    adrbrowsiel_rewards::GetShareURLCallback callback));
  MOCK_METHOD1(GetPendingContributions,
               void(adrbrowsiel_rewards::GetPendingContributionsCallback));
  MOCK_METHOD1(RemovePendingContribution, void(const uint64_t));
  MOCK_METHOD0(RemoveAllPendingContributions, void());

  MOCK_METHOD1(FetchBalance,
               void(adrbrowsiel_rewards::FetchBalanceCallback callback));

  MOCK_METHOD2(SaveRedditPublisherInfo,
               void(const std::map<std::string, std::string>&,
                    adrbrowsiel_rewards::SaveMediaInfoCallback));

  MOCK_METHOD1(GetExternalWallet,
               void(adrbrowsiel_rewards::GetExternalWalletCallback callback));

  MOCK_METHOD3(ProcessRewardsPageUrl,
               void(const std::string& path,
                    const std::string& query,
                    adrbrowsiel_rewards::ProcessRewardsPageUrlCallback callback));

  MOCK_METHOD0(DisconnectWallet, void());

  MOCK_CONST_METHOD0(OnlyAnonWallet, bool());

  MOCK_METHOD1(AddPrivateObserver,
               void(RewardsServicePrivateObserver* observer));
  MOCK_METHOD1(RemovePrivateObserver,
               void(RewardsServicePrivateObserver* observer));

  MOCK_METHOD1(GetAnonWalletStatus,
               void(adrbrowsiel_rewards::GetAnonWalletStatusCallback callback));

  MOCK_METHOD3(GetBalanceReport,
               void(const uint32_t month,
                    const uint32_t year,
                    adrbrowsiel_rewards::GetBalanceReportCallback callback));

  MOCK_METHOD3(GetMonthlyReport,
               void(const uint32_t month,
                    const uint32_t year,
                    adrbrowsiel_rewards::GetMonthlyReportCallback callback));

  MOCK_METHOD1(GetAllMonthlyReportIds,
               void(adrbrowsiel_rewards::GetAllMonthlyReportIdsCallback callback));

  MOCK_METHOD1(GetAllContributions,
               void(adrbrowsiel_rewards::GetAllContributionsCallback callback));

  MOCK_METHOD1(GetAllPromotions,
               void(adrbrowsiel_rewards::GetAllPromotionsCallback callback));

  MOCK_METHOD4(WriteDiagnosticLog,
               void(const std::string& file,
                    const int line,
                    const int verbose_level,
                    const std::string& message));

  MOCK_METHOD2(LoadDiagnosticLog,
               void(const int num_lines,
                    adrbrowsiel_rewards::LoadDiagnosticLogCallback callback));

  MOCK_METHOD1(ClearDiagnosticLog,
               void(adrbrowsiel_rewards::ClearDiagnosticLogCallback callback));

  MOCK_METHOD1(CompleteReset, void(adrbrowsiel_rewards::SuccessCallback callback));

  MOCK_METHOD1(GetEventLogs,
               void(adrbrowsiel_rewards::GetEventLogsCallback callback));

  MOCK_METHOD1(GetEncryptedStringState, std::string(const std::string&));

  MOCK_METHOD2(SetEncryptedStringState,
               bool(const std::string&, const std::string&));

  MOCK_METHOD1(GetadrbrowsielWallet, void(adrbrowsiel_rewards::GetadrbrowsielWalletCallback));

  MOCK_METHOD1(CreateWallet, void(adrbrowsiel_rewards::CreateWalletCallback));

  MOCK_METHOD1(StartProcess, void(base::OnceClosure));

  MOCK_METHOD1(GetWalletPassphrase,
               void(adrbrowsiel_rewards::GetWalletPassphraseCallback));

  MOCK_METHOD1(SetAdsEnabled, void(const bool is_enabled));

  MOCK_CONST_METHOD0(IsRewardsEnabled, bool());
};

class AdsServiceTest : public testing::Test {
 public:
  AdsServiceTest() {}
  ~AdsServiceTest() override {}

  AdsService* ads_service_;

 protected:
  void SetUp() override {
    ASSERT_TRUE(temp_dir_.CreateUniqueTempDir());
    profile_ = adrbrowsiel_ads::CreateadrbrowsielAdsProfile(temp_dir_.GetPath());
    ASSERT_TRUE(profile_.get() != NULL);
    RewardsServiceFactory::SetServiceForTesting(new MockRewardsService());
    rewards_service_ = static_cast<MockRewardsService*>(
        RewardsServiceFactory::GetForProfile(profile()));
    ads_service_ = AdsServiceFactory::GetForProfile(profile());
    ASSERT_TRUE(AdsServiceFactory::GetInstance() != NULL);
    ASSERT_TRUE(ads_service() != NULL);
  }

  void TearDown() override {
    profile_.reset();
    delete rewards_service_;
  }

  Profile* profile() { return profile_.get(); }
  AdsService* ads_service() { return ads_service_; }
  MockRewardsService* rewards_service() { return rewards_service_; }

 private:
  content::BrowserTaskEnvironment task_environment_;
  std::unique_ptr<Profile> profile_;
  base::ScopedTempDir temp_dir_;
  MockRewardsService* rewards_service_;
};
