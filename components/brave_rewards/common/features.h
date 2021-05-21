/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_REWARDS_COMMON_FEATURES_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_REWARDS_COMMON_FEATURES_H_

namespace base {
struct Feature;
}  // namespace base

namespace adrbrowsiel_rewards {
namespace features {

extern const base::Feature kBitflyerFeature;
extern const base::Feature kVerboseLoggingFeature;

}  // namespace features
}  // namespace adrbrowsiel_rewards

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_REWARDS_COMMON_FEATURES_H_
