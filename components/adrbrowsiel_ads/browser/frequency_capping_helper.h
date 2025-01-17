/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_ADS_BROWSER_FREQUENCY_CAPPING_HELPER_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_ADS_BROWSER_FREQUENCY_CAPPING_HELPER_H_

#include <cstdint>
#include <string>
#include <vector>

#include "base/memory/singleton.h"
#include "bat/ads/ad_event_history.h"

namespace adrbrowsiel_ads {

class FrequencyCappingHelper {
 public:
  static FrequencyCappingHelper* GetInstance();

  void RecordAdEvent(const std::string& ad_type,
                     const std::string& confirmation_type,
                     const uint64_t timestamp);

  std::vector<uint64_t> GetAdEvents(const std::string& ad_type,
                                    const std::string& confirmation_type) const;

 private:
  friend struct base::DefaultSingletonTraits<FrequencyCappingHelper>;

  FrequencyCappingHelper();
  ~FrequencyCappingHelper();

  ads::AdEventHistory ad_event_history_;

  FrequencyCappingHelper(const FrequencyCappingHelper&) = delete;
  FrequencyCappingHelper& operator=(const FrequencyCappingHelper&) = delete;
};

}  // namespace adrbrowsiel_ads

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_ADS_BROWSER_FREQUENCY_CAPPING_HELPER_H_
