// Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef adrbrowsiel_BROWSER_adrbrowsiel_STATS_SWITCHES_H_
#define adrbrowsiel_BROWSER_adrbrowsiel_STATS_SWITCHES_H_

namespace adrbrowsiel_stats {

namespace switches {

// Allows setting the usage server to a custom host. Useful both for manual
// testing against staging and for browser tests.
constexpr char kadrbrowsielStatsUpdaterServer[] =
    "adrbrowsiel-stats-updater-server";

}  // namespace switches
}  // namespace adrbrowsiel_stats

#endif  // adrbrowsiel_BROWSER_adrbrowsiel_STATS_SWITCHES_H_
