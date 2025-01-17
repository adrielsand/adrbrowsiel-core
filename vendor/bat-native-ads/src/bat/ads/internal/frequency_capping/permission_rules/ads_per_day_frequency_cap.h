/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_FREQUENCY_CAPPING_PERMISSION_RULES_ADS_PER_DAY_FREQUENCY_CAP_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_FREQUENCY_CAPPING_PERMISSION_RULES_ADS_PER_DAY_FREQUENCY_CAP_H_

#include <cstdint>
#include <deque>
#include <string>

#include "bat/ads/internal/frequency_capping/permission_rules/permission_rule.h"

namespace ads {

class AdsPerDayFrequencyCap : public PermissionRule {
 public:
  AdsPerDayFrequencyCap();
  ~AdsPerDayFrequencyCap() override;

  bool ShouldAllow() override;

  std::string get_last_message() const override;

 private:
  std::string last_message_;

  bool DoesRespectCap(const std::deque<uint64_t>& history);

  AdsPerDayFrequencyCap(const AdsPerDayFrequencyCap&) = delete;
  AdsPerDayFrequencyCap& operator=(const AdsPerDayFrequencyCap&) = delete;
};

}  // namespace ads

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_FREQUENCY_CAPPING_PERMISSION_RULES_ADS_PER_DAY_FREQUENCY_CAP_H_
