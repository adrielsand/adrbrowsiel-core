/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/adrbrowsiel_sync/features.h"

#include "base/feature_list.h"
#include "adrbrowsiel/components/adrbrowsiel_sync/buildflags/buildflags.h"

namespace adrbrowsiel_sync {
namespace features {

#if BUILDFLAG(ENABLE_adrbrowsiel_SYNC)
const base::Feature kadrbrowsielSync{"adrbrowsielSync", base::FEATURE_ENABLED_BY_DEFAULT};
#else
const base::Feature kadrbrowsielSync{"adrbrowsielSync", base::FEATURE_DISABLED_BY_DEFAULT};
#endif

}  // namespace features
}  // namespace adrbrowsiel_sync
