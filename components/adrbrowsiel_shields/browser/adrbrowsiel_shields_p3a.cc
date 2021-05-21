/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/adrbrowsiel_shields/browser/adrbrowsiel_shields_p3a.h"

#include "base/logging.h"
#include "base/metrics/histogram_macros.h"
#include "adrbrowsiel/components/p3a/adrbrowsiel_p3a_utils.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"

namespace adrbrowsiel_shields {

namespace {
// TODO(iefremov): Move to separate header when needed.
constexpr char kPrefName[] = "adrbrowsiel_shields.p3a_usage";
}  // namespace

void MaybeRecordShieldsUsageP3A(ShieldsIconUsage usage,
                                PrefService* local_state) {
  ::adrbrowsiel::RecordValueIfGreater<ShieldsIconUsage>(
      usage, "adrbrowsiel.Shields.UsageStatus", kPrefName, local_state);
}

void RegisterShieldsP3APrefs(PrefRegistrySimple* local_state) {
  local_state->RegisterIntegerPref(kPrefName, -1);
}

}  // namespace adrbrowsiel_shields
