/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <ctime>
#include <memory>

#include "adrbrowsiel/components/adrbrowsiel_stats/browser/adrbrowsiel_stats_updater_util.h"

#include "base/environment.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/string_piece.h"
#include "base/strings/string_split.h"
#include "base/strings/stringprintf.h"

namespace adrbrowsiel_stats {

std::string GetDateAsYMD(const base::Time& time) {
  base::Time::Exploded exploded;
  time.LocalExplode(&exploded);
  return base::StringPrintf("%d-%02d-%02d", exploded.year, exploded.month,
                            exploded.day_of_month);
}

std::string GetPlatformIdentifier() {
#if defined(OS_WIN)
  if (base::SysInfo::OperatingSystemArchitecture() == "x86")
    return "winia32-bc";
  else
    return "winx64-bc";
#elif defined(OS_MAC)
  #if defined(ARCH_CPU_X86_64)
    return "osx-bc";
  #elif defined(ARCH_CPU_ARM64)
    return "osxarm64-bc";
  #endif
#elif defined(OS_ANDROID)
  return "android-bc";
#elif defined(OS_LINUX)
  return "linux-bc";
#else
  return std::string();
#endif
}

int GetIsoWeekNumber(const base::Time& time) {
  char buffer[24];
  time_t rawtime = time.ToTimeT();
  struct tm* timeinfo = std::localtime(&rawtime);
  strftime(buffer, 24, "%V", timeinfo);

  int week_number = 0;
  if (!base::StringToInt(buffer, &week_number))
    return 0;

  return week_number;
}

base::Time GetYMDAsDate(const base::StringPiece& ymd) {
  const auto pieces = base::SplitStringPiece(ymd, "-", base::TRIM_WHITESPACE,
                                             base::SPLIT_WANT_NONEMPTY);
  DCHECK_EQ(pieces.size(), 3ull);
  base::Time::Exploded time = {};

  bool ok = base::StringToInt(pieces[0], &time.year);
  DCHECK(ok);
  ok = base::StringToInt(pieces[1], &time.month);
  DCHECK(ok);
  ok = base::StringToInt(pieces[2], &time.day_of_month);
  DCHECK(ok);

  DCHECK(time.HasValidValues());

  base::Time result;
  ok = base::Time::FromLocalExploded(time, &result);
  DCHECK(ok);

  return result;
}

std::string GetAPIKey() {
  std::string api_key = adrbrowsiel_STATS_API_KEY;
  std::unique_ptr<base::Environment> env(base::Environment::Create());
  if (env->HasVar("adrbrowsiel_STATS_API_KEY"))
    env->GetVar("adrbrowsiel_STATS_API_KEY", &api_key);

  return api_key;
}

}  // namespace adrbrowsiel_stats
