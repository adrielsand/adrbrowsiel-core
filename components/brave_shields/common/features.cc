// Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "adrbrowsiel/components/adrbrowsiel_shields/common/features.h"

#include "base/feature_list.h"

namespace adrbrowsiel_shields {
namespace features {

const base::Feature kadrbrowsielAdblockCosmeticFiltering{
    "adrbrowsielAdblockCosmeticFiltering",
    base::FEATURE_ENABLED_BY_DEFAULT};
const base::Feature kadrbrowsielAdblockCosmeticFilteringNative{
    "adrbrowsielAdblockCosmeticFilteringNative", base::FEATURE_DISABLED_BY_DEFAULT};
const base::Feature kadrbrowsielAdblockCspRules{
    "adrbrowsielAdblockCspRules", base::FEATURE_ENABLED_BY_DEFAULT};
// When enabled, adrbrowsiel will block domains listed in the user's selected adblock
// filters and present a security interstitial with choice to proceed and
// optionally whitelist the domain.
// Domain block filters look like this:
// ||ads.example.com^
const base::Feature kadrbrowsielDomainBlock{"adrbrowsielDomainBlock",
                                      base::FEATURE_ENABLED_BY_DEFAULT};
// When enabled, network requests initiated by extensions will be checked and
// potentially blocked by adrbrowsiel Shields.
const base::Feature kadrbrowsielExtensionNetworkBlocking{
    "adrbrowsielExtensionNetworkBlocking", base::FEATURE_DISABLED_BY_DEFAULT};

}  // namespace features
}  // namespace adrbrowsiel_shields
