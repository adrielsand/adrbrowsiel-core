/* Copyright 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/adrbrowsiel_ads/browser/ads_p2a.h"

#include <cstdint>
#include <map>
#include <string>

#include "base/metrics/histogram_functions.h"
#include "adrbrowsiel/components/adrbrowsiel_ads/common/pref_names.h"
#include "adrbrowsiel/components/weekly_storage/weekly_storage.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"

namespace adrbrowsiel_ads {
namespace {

constexpr const char* kP2AQuestionNameList[] = {
    // Ad Opportunities
    "adrbrowsiel.P2A.TotalAdOpportunities",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.architecture",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.artsentertainment",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.automotive",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.business",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.careers",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.cellphones",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.crypto",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.education",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.familyparenting",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.fashion",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.folklore",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.fooddrink",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.gaming",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.healthfitness",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.history",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.hobbiesinterests",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.home",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.law",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.military",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.other",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.personalfinance",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.pets",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.realestate",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.science",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.sports",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.technologycomputing",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.travel",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.weather",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.untargeted",
    // Ad Impressions
    "adrbrowsiel.P2A.TotalAdImpressions",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.architecture",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.artsentertainment",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.automotive",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.business",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.careers",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.cellphones",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.crypto",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.education",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.familyparenting",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.fashion",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.folklore",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.fooddrink",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.gaming",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.healthfitness",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.history",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.hobbiesinterests",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.home",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.law",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.military",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.other",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.personalfinance",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.pets",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.realestate",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.science",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.sports",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.technologycomputing",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.travel",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.weather",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.untargeted"};

const uint16_t kIntervalBuckets[] = {0, 5, 10, 20, 50, 100, 250, 500};

}  // namespace

void RegisterP2APrefs(PrefRegistrySimple* registry) {
  for (const char* question_name : kP2AQuestionNameList) {
    std::string pref_path(prefs::kP2AStoragePrefNamePrefix);
    pref_path.append(question_name);
    registry->RegisterListPref(pref_path);
  }
}

void RecordInWeeklyStorageAndEmitP2AHistogramAnswer(PrefService* prefs,
                                                    const std::string& name) {
  std::string pref_path(prefs::kP2AStoragePrefNamePrefix);
  pref_path.append(name);
  if (!prefs->FindPreference(pref_path)) {
    return;
  }
  WeeklyStorage storage(prefs, pref_path.c_str());
  storage.AddDelta(1);
  EmitP2AHistogramAnswer(name, storage.GetWeeklySum());
}

void EmitP2AHistogramAnswer(const std::string& name, uint16_t count_value) {
  const uint16_t* iter = std::lower_bound(
      kIntervalBuckets, std::end(kIntervalBuckets), count_value);
  const uint16_t bucket = iter - kIntervalBuckets;

  for (const char* question_name : kP2AQuestionNameList) {
    if (name != question_name) {
      continue;
    }

    base::UmaHistogramExactLinear(question_name, bucket,
                                  base::size(kIntervalBuckets) + 1);
  }
}

void SuspendP2AHistograms() {
  for (const char* question_name : kP2AQuestionNameList) {
    base::UmaHistogramExactLinear(question_name, INT_MAX,
                                  base::size(kIntervalBuckets) + 1);
  }
}

}  // namespace adrbrowsiel_ads
