/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_FREQUENCY_CAPPING_EXCLUSION_RULES_MARKED_TO_NO_LONGER_RECEIVE_FREQUENCY_CAP_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_FREQUENCY_CAPPING_EXCLUSION_RULES_MARKED_TO_NO_LONGER_RECEIVE_FREQUENCY_CAP_H_

#include <string>

#include "bat/ads/internal/frequency_capping/exclusion_rules/exclusion_rule.h"

namespace ads {

struct CreativeAdInfo;

class MarkedToNoLongerReceiveFrequencyCap
    : public ExclusionRule<CreativeAdInfo> {
 public:
  MarkedToNoLongerReceiveFrequencyCap();

  ~MarkedToNoLongerReceiveFrequencyCap() override;

  MarkedToNoLongerReceiveFrequencyCap(
      const MarkedToNoLongerReceiveFrequencyCap&) = delete;
  MarkedToNoLongerReceiveFrequencyCap& operator=(
      const MarkedToNoLongerReceiveFrequencyCap&) = delete;

  bool ShouldExclude(const CreativeAdInfo& ad) override;

  std::string get_last_message() const override;

 private:
  std::string last_message_;

  bool DoesRespectCap(const CreativeAdInfo& ad);
};

}  // namespace ads

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_FREQUENCY_CAPPING_EXCLUSION_RULES_MARKED_TO_NO_LONGER_RECEIVE_FREQUENCY_CAP_H_
