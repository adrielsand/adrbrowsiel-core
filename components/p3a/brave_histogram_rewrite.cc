/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/p3a/adrbrowsiel_histogram_rewrite.h"

#include "base/bind.h"
#include "base/metrics/histogram_macros.h"
#include "base/metrics/statistics_recorder.h"

namespace adrbrowsiel {

namespace {

// Please keep this list sorted and synced with |DoHistogramadrbrowsielzation|.
constexpr const char* kadrbrowsielzationHistograms[] = {
    "Bookmarks.Count.OnProfileLoad",
    "DefaultBrowser.State",
    "Extensions.LoadExtension",
    "Tabs.TabCount",
    "Tabs.TabCountPerLoad",
    "Tabs.WindowCount",
};

// TODO(iefremov): Replace a bunch of 'if's with something more elegant.
// Records the given sample using the proper adrbrowsiel way.
void DoHistogramadrbrowsielzation(const char* histogram_name,
                            uint64_t name_hash,
                            base::HistogramBase::Sample sample) {
  DCHECK(histogram_name);
  if (strcmp("Bookmarks.Count.OnProfileLoad", histogram_name) == 0) {
    constexpr int kIntervals[] = {5, 20, 100, 500, 1000, 5000, 10000};
    const int* it =
        std::lower_bound(kIntervals, std::end(kIntervals), sample);
    const int answer = it - kIntervals;
    UMA_HISTOGRAM_EXACT_LINEAR("adrbrowsiel.Core.BookmarksCountOnProfileLoad.2",
                               answer, base::size(kIntervals));
    return;
  }

  if (strcmp("DefaultBrowser.State", histogram_name) == 0) {
    int answer = 0;
    switch (sample) {
      case 0:  // Not default.
      case 1:  // Default.
        answer = sample;
        break;
      case 2:  // Unknown, merging to "Not default".
        answer = 0;
        break;
      case 3:  // Other mode is default, merging to "Default".
        answer = 1;
        break;
    default:
      NOTREACHED();
    }
    UMA_HISTOGRAM_BOOLEAN("adrbrowsiel.Core.IsDefault", answer);
  }

  if (strcmp("Extensions.LoadExtension", histogram_name) == 0) {
    int answer = 0;
    if (sample == 1)
      answer = 1;
    else if (2 <= sample && sample <= 4)
      answer = 2;
    else if (sample >= 5)
      answer = 3;

    UMA_HISTOGRAM_EXACT_LINEAR("adrbrowsiel.Core.NumberOfExtensions", answer, 3);
    return;
  }

  if (strcmp("Tabs.TabCount", histogram_name) == 0 ||
      strcmp("Tabs.TabCountPerLoad", histogram_name) == 0) {
    int answer = 0;
    if (0 <= sample && sample <= 1) {
      answer = 0;
    } else if (2 <= sample && sample <= 5) {
      answer = 1;
    } else if (6 <= sample && sample <= 10) {
      answer = 2;
    } else if (11 <= sample && sample <= 50) {
      answer = 3;
    } else {
      answer = 4;
    }

    UMA_HISTOGRAM_EXACT_LINEAR("adrbrowsiel.Core.TabCount", answer, 4);
    return;
  }

  if (strcmp("Tabs.WindowCount", histogram_name) == 0) {
    int answer = 0;
    if (sample <= 0) {
      answer = 0;
    } else if (sample == 1) {
      answer = 1;
    } else if (2 <= sample && sample <= 5) {
      answer = 2;
    } else {
      answer = 3;
    }

    UMA_HISTOGRAM_EXACT_LINEAR("adrbrowsiel.Core.WindowCount.2", answer, 3);
    return;
  }
}

}  // namespace

void SetupHistogramsadrbrowsielization() {
  for (const char* histogram_name : kadrbrowsielzationHistograms) {
    base::StatisticsRecorder::SetCallback(
        histogram_name,
        base::BindRepeating(&DoHistogramadrbrowsielzation));
  }
}

}  // namespace adrbrowsiel
