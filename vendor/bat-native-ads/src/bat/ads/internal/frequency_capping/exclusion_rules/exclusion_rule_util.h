/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_FREQUENCY_CAPPING_EXCLUSION_RULES_EXCLUSION_RULE_UTIL_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_FREQUENCY_CAPPING_EXCLUSION_RULES_EXCLUSION_RULE_UTIL_H_

#include <string>

#include "bat/ads/internal/frequency_capping/exclusion_rules/exclusion_rule.h"
#include "bat/ads/internal/logging.h"

namespace ads {

template <typename T>
bool ShouldExclude(const T& ad, ExclusionRule<T>* exclusion_rule) {
  DCHECK(exclusion_rule);

  if (!exclusion_rule->ShouldExclude(ad)) {
    return false;
  }

  const std::string reason = exclusion_rule->get_last_message();
  if (!reason.empty()) {
    BLOG(2, reason);
  }

  return true;
}

}  // namespace ads

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_FREQUENCY_CAPPING_EXCLUSION_RULES_EXCLUSION_RULE_UTIL_H_
