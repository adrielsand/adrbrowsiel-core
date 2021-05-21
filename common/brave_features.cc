/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/common/adrbrowsiel_features.h"
#include "build/build_config.h"

namespace features {

#if defined(OS_ANDROID)
//  Flag for adrbrowsiel Rewards.
#if defined(ARCH_CPU_X86_FAMILY) && defined(OFFICIAL_BUILD)
const base::Feature kadrbrowsielRewards{"adrbrowsielRewards",
                                  base::FEATURE_DISABLED_BY_DEFAULT};
#else
const base::Feature kadrbrowsielRewards{"adrbrowsielRewards",
                                  base::FEATURE_ENABLED_BY_DEFAULT};
#endif
#endif  // defined(OS_ANDROID)

}  // namespace features
