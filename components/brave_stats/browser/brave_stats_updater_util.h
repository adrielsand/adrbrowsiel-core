/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_STATS_BROWSER_adrbrowsiel_STATS_UPDATER_UTIL_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_STATS_BROWSER_adrbrowsiel_STATS_UPDATER_UTIL_H_

#include <string>

#include "base/time/time.h"
#include "base/system/sys_info.h"
#include "components/prefs/pref_service.h"

namespace adrbrowsiel_stats {

enum class ProcessArch {
  kArchSkip,
  kArchMetal,
  kArchVirt,
};

std::string GetDateAsYMD(const base::Time& time);

std::string GetPlatformIdentifier();

int GetIsoWeekNumber(const base::Time& time);

base::Time GetYMDAsDate(const base::StringPiece& ymd);

std::string GetAPIKey();

}  // namespace adrbrowsiel_stats

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_STATS_BROWSER_adrbrowsiel_STATS_UPDATER_UTIL_H_
