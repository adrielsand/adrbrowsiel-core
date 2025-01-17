/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_AD_TARGETING_DATA_TYPES_BEHAVIORAL_BANDITS_EPSILON_GREEDY_BANDIT_ARMS_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_AD_TARGETING_DATA_TYPES_BEHAVIORAL_BANDITS_EPSILON_GREEDY_BANDIT_ARMS_H_

#include <map>
#include <string>

#include "bat/ads/internal/ad_targeting/data_types/behavioral/bandits/epsilon_greedy_bandit_arm_info.h"

namespace ads {
namespace ad_targeting {

using EpsilonGreedyBanditArmMap =
    std::map<std::string, EpsilonGreedyBanditArmInfo>;

class EpsilonGreedyBanditArms {
 public:
  EpsilonGreedyBanditArms();

  ~EpsilonGreedyBanditArms();

  static EpsilonGreedyBanditArmMap FromJson(const std::string& json);

  static std::string ToJson(const EpsilonGreedyBanditArmMap& arms);
};

}  // namespace ad_targeting
}  // namespace ads

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_AD_TARGETING_DATA_TYPES_BEHAVIORAL_BANDITS_EPSILON_GREEDY_BANDIT_ARMS_H_
