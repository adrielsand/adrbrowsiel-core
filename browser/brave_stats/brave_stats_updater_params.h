/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_adrbrowsiel_STATS_adrbrowsiel_STATS_UPDATER_PARAMS_H_
#define adrbrowsiel_BROWSER_adrbrowsiel_STATS_adrbrowsiel_STATS_UPDATER_PARAMS_H_

#include <string>

#include "base/macros.h"
#include "base/time/time.h"
#include "adrbrowsiel/components/adrbrowsiel_stats/browser/adrbrowsiel_stats_updater_util.h"

class adrbrowsielStatsUpdaterTest;
class PrefService;

namespace adrbrowsiel_stats {

class adrbrowsielStatsUpdaterParams {
 public:
  explicit adrbrowsielStatsUpdaterParams(PrefService* stats_pref_service,
                                   PrefService* profile_pref_service,
                                   const ProcessArch arch);
  adrbrowsielStatsUpdaterParams(PrefService* stats_pref_service,
                          PrefService* profile_pref_service,
                          const ProcessArch arch,
                          const std::string& ymd,
                          int woy,
                          int month);
  ~adrbrowsielStatsUpdaterParams();

  std::string GetDailyParam() const;
  std::string GetWeeklyParam() const;
  std::string GetMonthlyParam() const;
  std::string GetFirstCheckMadeParam() const;
  std::string GetWeekOfInstallationParam() const;
  std::string GetDateOfInstallationParam() const;
  std::string GetReferralCodeParam() const;
  std::string GetAdsEnabledParam() const;
  std::string GetProcessArchParam() const;

  void SavePrefs();

 private:
  friend class ::adrbrowsielStatsUpdaterTest;
  PrefService* stats_pref_service_;
  PrefService* profile_pref_service_;
  ProcessArch arch_;
  std::string ymd_;
  int woy_;
  int month_;
  std::string last_check_ymd_;
  int last_check_woy_;
  int last_check_month_;
  bool first_check_made_;
  std::string week_of_installation_;
  base::Time date_of_installation_;
  std::string referral_promo_code_;
  static base::Time g_current_time;
  static bool g_force_first_run;

  void LoadPrefs();

  std::string BooleanToString(bool bool_value) const;

  std::string GetDateAsYMD(const base::Time& time) const;
  std::string GetCurrentDateAsYMD() const;
  std::string GetLastMondayAsYMD() const;
  int GetCurrentMonth() const;
  int GetCurrentISOWeekNumber() const;
  base::Time GetCurrentTimeNow() const;
  bool ShouldForceFirstRun() const;

  static void SetCurrentTimeForTest(const base::Time& current_time);
  static void SetFirstRunForTest(bool first_run);
  // Returns the timestamp of the browsers first run
  static base::Time GetFirstRunTime(PrefService *pref_service);

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielStatsUpdaterParams);
};

}  // namespace adrbrowsiel_stats

#endif  // adrbrowsiel_BROWSER_adrbrowsiel_STATS_adrbrowsiel_STATS_UPDATER_PARAMS_H_
