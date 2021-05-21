/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>

#include "adrbrowsiel/browser/adrbrowsiel_stats/adrbrowsiel_stats_updater.h"

#include "base/files/scoped_temp_dir.h"
#include "base/system/sys_info.h"
#include "base/time/time.h"
#include "bat/ads/pref_names.h"
#include "adrbrowsiel/browser/adrbrowsiel_stats/adrbrowsiel_stats_updater_params.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_ads/browser/test_util.h"
#include "adrbrowsiel/components/adrbrowsiel_referrals/browser/adrbrowsiel_referrals_service.h"
#include "adrbrowsiel/components/adrbrowsiel_stats/browser/adrbrowsiel_stats_updater_util.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/test/base/testing_browser_process.h"
#include "chrome/test/base/testing_profile_manager.h"
#include "components/prefs/testing_pref_service.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- adrbrowsiel_unit_tests --filter=adrbrowsielStatsUpdaterTest.*

const char kYesterday[] = "2018-06-21";
const char kToday[] = "2018-06-22";
const char kTomorrow[] = "2018-06-23";

const int kLastWeek = 24;
const int kThisWeek = 25;
const int kNextWeek = 26;

const int kLastMonth = 5;
const int kThisMonth = 6;
const int kNextMonth = 7;

class adrbrowsielStatsUpdaterTest : public testing::Test {
 public:
  adrbrowsielStatsUpdaterTest()
      : profile_manager_(TestingBrowserProcess::GetGlobal()) {}
  ~adrbrowsielStatsUpdaterTest() override {}

  void SetUp() override {
    EXPECT_TRUE(temp_dir_.CreateUniqueTempDir());
    EXPECT_TRUE(profile_manager_.SetUp());
    profile_ = adrbrowsiel_ads::CreateadrbrowsielAdsProfile(temp_dir_.GetPath());
    EXPECT_TRUE(profile_.get() != NULL);
    adrbrowsiel_stats::RegisterLocalStatePrefs(testing_local_state_.registry());
    adrbrowsiel::RegisterPrefsForadrbrowsielReferralsService(
        testing_local_state_.registry());
    adrbrowsiel_stats::adrbrowsielStatsUpdaterParams::SetFirstRunForTest(true);
  }

  void TearDown() override { profile_.reset(); }

  PrefService* GetLocalState() { return &testing_local_state_; }
  PrefService* GetProfilePrefs() { return profile_->GetPrefs(); }
  void SetEnableAds(bool ads_enabled) {
    GetProfilePrefs()->SetBoolean(ads::prefs::kEnabled, ads_enabled);
  }

  void SetCurrentTimeForTest(const base::Time& current_time) {
    adrbrowsiel_stats::adrbrowsielStatsUpdaterParams::SetCurrentTimeForTest(current_time);
  }

 private:
  TestingPrefServiceSimple testing_local_state_;
  TestingProfileManager profile_manager_;
  std::unique_ptr<Profile> profile_;
  base::ScopedTempDir temp_dir_;
  content::BrowserTaskEnvironment task_environment_;
};

TEST_F(adrbrowsielStatsUpdaterTest, IsDailyUpdateNeededLastCheckedYesterday) {
  GetLocalState()->SetString(kLastCheckYMD, kYesterday);

  adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), adrbrowsiel_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(adrbrowsiel_stats_updater_params.GetDailyParam(), "true");
  adrbrowsiel_stats_updater_params.SavePrefs();

  EXPECT_EQ(GetLocalState()->GetString(kLastCheckYMD), kToday);
}

TEST_F(adrbrowsielStatsUpdaterTest, IsDailyUpdateNeededLastCheckedToday) {
  GetLocalState()->SetString(kLastCheckYMD, kToday);

  adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), adrbrowsiel_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(adrbrowsiel_stats_updater_params.GetDailyParam(), "false");
  adrbrowsiel_stats_updater_params.SavePrefs();

  EXPECT_EQ(GetLocalState()->GetString(kLastCheckYMD), kToday);
}

TEST_F(adrbrowsielStatsUpdaterTest, IsDailyUpdateNeededLastCheckedTomorrow) {
  GetLocalState()->SetString(kLastCheckYMD, kTomorrow);

  adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), adrbrowsiel_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(adrbrowsiel_stats_updater_params.GetDailyParam(), "false");
  adrbrowsiel_stats_updater_params.SavePrefs();

  EXPECT_EQ(GetLocalState()->GetString(kLastCheckYMD), kToday);
}

TEST_F(adrbrowsielStatsUpdaterTest, IsWeeklyUpdateNeededLastCheckedLastWeek) {
  GetLocalState()->SetInteger(kLastCheckWOY, kLastWeek);

  adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), adrbrowsiel_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(adrbrowsiel_stats_updater_params.GetWeeklyParam(), "true");
  adrbrowsiel_stats_updater_params.SavePrefs();

  EXPECT_EQ(GetLocalState()->GetInteger(kLastCheckWOY), kThisWeek);
}

TEST_F(adrbrowsielStatsUpdaterTest, IsWeeklyUpdateNeededLastCheckedThisWeek) {
  GetLocalState()->SetInteger(kLastCheckWOY, kThisWeek);

  adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), adrbrowsiel_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(adrbrowsiel_stats_updater_params.GetWeeklyParam(), "false");
  adrbrowsiel_stats_updater_params.SavePrefs();

  EXPECT_EQ(GetLocalState()->GetInteger(kLastCheckWOY), kThisWeek);
}

TEST_F(adrbrowsielStatsUpdaterTest, IsWeeklyUpdateNeededLastCheckedNextWeek) {
  GetLocalState()->SetInteger(kLastCheckWOY, kNextWeek);

  adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), adrbrowsiel_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(adrbrowsiel_stats_updater_params.GetWeeklyParam(), "true");
  adrbrowsiel_stats_updater_params.SavePrefs();

  EXPECT_EQ(GetLocalState()->GetInteger(kLastCheckWOY), kThisWeek);
}

TEST_F(adrbrowsielStatsUpdaterTest, IsMonthlyUpdateNeededLastCheckedLastMonth) {
  GetLocalState()->SetInteger(kLastCheckMonth, kLastMonth);

  adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), adrbrowsiel_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(adrbrowsiel_stats_updater_params.GetMonthlyParam(), "true");
  adrbrowsiel_stats_updater_params.SavePrefs();

  EXPECT_EQ(GetLocalState()->GetInteger(kLastCheckMonth), kThisMonth);
}

TEST_F(adrbrowsielStatsUpdaterTest, IsMonthlyUpdateNeededLastCheckedThisMonth) {
  GetLocalState()->SetInteger(kLastCheckMonth, kThisMonth);

  adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), adrbrowsiel_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(adrbrowsiel_stats_updater_params.GetMonthlyParam(), "false");
  adrbrowsiel_stats_updater_params.SavePrefs();

  EXPECT_EQ(GetLocalState()->GetInteger(kLastCheckMonth), kThisMonth);
}

TEST_F(adrbrowsielStatsUpdaterTest, IsMonthlyUpdateNeededLastCheckedNextMonth) {
  GetLocalState()->SetInteger(kLastCheckMonth, kNextMonth);

  adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), adrbrowsiel_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(adrbrowsiel_stats_updater_params.GetMonthlyParam(), "true");
  adrbrowsiel_stats_updater_params.SavePrefs();

  EXPECT_EQ(GetLocalState()->GetInteger(kLastCheckMonth), kThisMonth);
}

TEST_F(adrbrowsielStatsUpdaterTest, HasAdsDisabled) {
  adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), adrbrowsiel_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  SetEnableAds(false);
  EXPECT_EQ(adrbrowsiel_stats_updater_params.GetAdsEnabledParam(), "false");
}

TEST_F(adrbrowsielStatsUpdaterTest, HasAdsEnabled) {
  adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), adrbrowsiel_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  SetEnableAds(true);
  EXPECT_EQ(adrbrowsiel_stats_updater_params.GetAdsEnabledParam(), "true");
}

TEST_F(adrbrowsielStatsUpdaterTest, HasArchSkip) {
  adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), adrbrowsiel_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(adrbrowsiel_stats_updater_params.GetProcessArchParam(), "");
}

TEST_F(adrbrowsielStatsUpdaterTest, HasArchVirt) {
  adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), adrbrowsiel_stats::ProcessArch::kArchVirt,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(adrbrowsiel_stats_updater_params.GetProcessArchParam(), "virt");
}

TEST_F(adrbrowsielStatsUpdaterTest, HasArchMetal) {
  auto arch = base::SysInfo::OperatingSystemArchitecture();
  adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), adrbrowsiel_stats::ProcessArch::kArchMetal,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(adrbrowsiel_stats_updater_params.GetProcessArchParam(), arch);
}

TEST_F(adrbrowsielStatsUpdaterTest, HasDateOfInstallationFirstRun) {
  base::Time::Exploded exploded;
  base::Time current_time;

  // Set date to 2018-11-04 (ISO week #44)
  exploded.hour = 0;
  exploded.minute = 0;
  exploded.second = 0;
  exploded.millisecond = 0;
  exploded.day_of_week = 0;
  exploded.year = 2018;
  exploded.month = 11;
  exploded.day_of_month = 4;

  EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &current_time));
  SetCurrentTimeForTest(current_time);

  adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), adrbrowsiel_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  EXPECT_EQ(adrbrowsiel_stats_updater_params.GetDateOfInstallationParam(),
            "2018-11-04");
}

TEST_F(adrbrowsielStatsUpdaterTest, HasDailyRetention) {
  base::Time::Exploded exploded;
  base::Time current_time, dtoi_time;

  // Set date to 2018-11-04
  exploded.hour = 0;
  exploded.minute = 0;
  exploded.second = 0;
  exploded.millisecond = 0;
  exploded.day_of_week = 0;
  exploded.year = 2018;
  exploded.month = 11;
  exploded.day_of_month = 4;

  EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &dtoi_time));
  // Make first run date 6 days earlier (still within 14 day window)
  exploded.day_of_month = 10;
  EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &current_time));

  SetCurrentTimeForTest(dtoi_time);
  adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), adrbrowsiel_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  SetCurrentTimeForTest(current_time);
  EXPECT_EQ(adrbrowsiel_stats_updater_params.GetDateOfInstallationParam(),
            "2018-11-04");
}

TEST_F(adrbrowsielStatsUpdaterTest, HasDailyRetentionExpiration) {
  base::Time::Exploded exploded;
  base::Time current_time, dtoi_time;

  // Set date to 2018-11-04
  exploded.hour = 0;
  exploded.minute = 0;
  exploded.second = 0;
  exploded.millisecond = 0;
  exploded.day_of_week = 0;
  exploded.year = 2018;
  exploded.month = 11;
  exploded.day_of_month = 4;

  EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &dtoi_time));
  // Make first run date 14 days earlier (outside 14 day window)
  exploded.day_of_month = 18;
  EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &current_time));

  SetCurrentTimeForTest(dtoi_time);
  adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
      GetLocalState(), GetProfilePrefs(), adrbrowsiel_stats::ProcessArch::kArchSkip,
      kToday, kThisWeek, kThisMonth);
  SetCurrentTimeForTest(current_time);
  EXPECT_EQ(adrbrowsiel_stats_updater_params.GetDateOfInstallationParam(), "null");
}

// This test ensures that our weekly stats cut over on Monday
TEST_F(adrbrowsielStatsUpdaterTest, IsWeeklyUpdateNeededOnMondayLastCheckedOnSunday) {
  base::Time::Exploded exploded;
  base::Time current_time;

  {
    // Set our local state to indicate that the last weekly check was
    // performed during ISO week #43
    GetLocalState()->SetInteger(kLastCheckWOY, 43);

    // Set date to 2018-11-04 (ISO week #44)
    exploded.hour = 0;
    exploded.minute = 0;
    exploded.second = 0;
    exploded.millisecond = 0;
    exploded.day_of_week = 0;
    exploded.year = 2018;
    exploded.month = 11;
    exploded.day_of_month = 4;

    EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &current_time));

    SetCurrentTimeForTest(current_time);
    adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
        GetLocalState(), GetProfilePrefs(),
        adrbrowsiel_stats::ProcessArch::kArchSkip);

    // Make sure that the weekly param was set to true, since this is
    // a new ISO week (#44)
    EXPECT_EQ(adrbrowsiel_stats_updater_params.GetWeeklyParam(), "true");
    adrbrowsiel_stats_updater_params.SavePrefs();

    // Make sure that local state was updated to reflect this as well
    EXPECT_EQ(GetLocalState()->GetInteger(kLastCheckWOY), 44);
  }

  {
    // Now it's the next day (Monday)
    exploded.day_of_week = 1;
    exploded.day_of_month = 5;

    EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &current_time));

    SetCurrentTimeForTest(current_time);
    adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
        GetLocalState(), GetProfilePrefs(),
        adrbrowsiel_stats::ProcessArch::kArchSkip);

    // Make sure that the weekly param was set to true, since this is
    // a new ISO week (#45)
    EXPECT_EQ(adrbrowsiel_stats_updater_params.GetWeeklyParam(), "true");
    adrbrowsiel_stats_updater_params.SavePrefs();

    // Make sure that local state was updated to reflect this as well
    EXPECT_EQ(GetLocalState()->GetInteger(kLastCheckWOY), 45);
  }

  {
    // Now it's the next day (Tuesday)
    exploded.day_of_week = 2;
    exploded.day_of_month = 6;

    EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &current_time));

    SetCurrentTimeForTest(current_time);
    adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
        GetLocalState(), GetProfilePrefs(),
        adrbrowsiel_stats::ProcessArch::kArchSkip);

    // Make sure that the weekly param was set to false, since this is
    // still the same ISO week (#45)
    EXPECT_EQ(adrbrowsiel_stats_updater_params.GetWeeklyParam(), "false");
    adrbrowsiel_stats_updater_params.SavePrefs();

    // Make sure that local state also didn't change
    EXPECT_EQ(GetLocalState()->GetInteger(kLastCheckWOY), 45);
  }
}

TEST_F(adrbrowsielStatsUpdaterTest, HasCorrectWeekOfInstallation) {
  base::Time::Exploded exploded;
  base::Time current_time;

  {
    // Set date to 2019-03-24 (Sunday)
    exploded.hour = 0;
    exploded.minute = 0;
    exploded.second = 0;
    exploded.millisecond = 0;
    exploded.day_of_week = 0;
    exploded.year = 2019;
    exploded.month = 3;
    exploded.day_of_month = 24;

    EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &current_time));
    SetCurrentTimeForTest(current_time);

    // Make sure that week of installation is previous Monday
    adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
        GetLocalState(), GetProfilePrefs(),
        adrbrowsiel_stats::ProcessArch::kArchSkip);
    EXPECT_EQ(adrbrowsiel_stats_updater_params.GetWeekOfInstallationParam(),
              "2019-03-18");
  }

  {
    // Set date to 2019-03-25 (Monday)
    exploded.hour = 0;
    exploded.minute = 0;
    exploded.second = 0;
    exploded.millisecond = 0;
    exploded.day_of_week = 0;
    exploded.year = 2019;
    exploded.month = 3;
    exploded.day_of_month = 25;

    EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &current_time));
    SetCurrentTimeForTest(current_time);

    // Make sure that week of installation is today, since today is a
    // Monday
    adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
        GetLocalState(), GetProfilePrefs(),
        adrbrowsiel_stats::ProcessArch::kArchSkip);
    EXPECT_EQ(adrbrowsiel_stats_updater_params.GetWeekOfInstallationParam(),
              "2019-03-25");
  }

  {
    // Set date to 2019-03-30 (Saturday)
    exploded.hour = 0;
    exploded.minute = 0;
    exploded.second = 0;
    exploded.millisecond = 0;
    exploded.day_of_week = 0;
    exploded.year = 2019;
    exploded.month = 3;
    exploded.day_of_month = 30;

    EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &current_time));
    SetCurrentTimeForTest(current_time);

    // Make sure that week of installation is previous Monday
    adrbrowsiel_stats::adrbrowsielStatsUpdaterParams adrbrowsiel_stats_updater_params(
        GetLocalState(), GetProfilePrefs(),
        adrbrowsiel_stats::ProcessArch::kArchSkip);
    EXPECT_EQ(adrbrowsiel_stats_updater_params.GetWeekOfInstallationParam(),
              "2019-03-25");
  }
}

TEST_F(adrbrowsielStatsUpdaterTest, GetIsoWeekNumber) {
  base::Time::Exploded exploded;
  exploded.hour = 0;
  exploded.minute = 0;
  exploded.second = 0;
  exploded.millisecond = 0;
  exploded.day_of_week = 1;
  exploded.day_of_month = 29;
  exploded.month = 7;
  exploded.year = 2019;

  base::Time time;
  EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &time));
  EXPECT_EQ(adrbrowsiel_stats::GetIsoWeekNumber(time), 31);

  exploded.day_of_month = 30;
  exploded.month = 9;

  EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &time));
  EXPECT_EQ(adrbrowsiel_stats::GetIsoWeekNumber(time), 40);

  exploded.day_of_month = 1;
  exploded.month = 9;
  exploded.day_of_week = 0;

  EXPECT_TRUE(base::Time::FromLocalExploded(exploded, &time));
  EXPECT_EQ(adrbrowsiel_stats::GetIsoWeekNumber(time), 35);
}
