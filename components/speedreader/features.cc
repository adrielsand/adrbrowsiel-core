// Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "adrbrowsiel/components/speedreader/features.h"

#include "base/feature_list.h"
#include "adrbrowsiel/components/speedreader/buildflags.h"

namespace speedreader {

const base::Feature kSpeedreaderFeature {
  "Speedreader",
#if BUILDFLAG(ENABLE_SPEEDREADER_FEATURE)
      base::FEATURE_ENABLED_BY_DEFAULT
#else
      base::FEATURE_DISABLED_BY_DEFAULT
#endif
};

const base::Feature kSpeedreaderLegacyBackend{
    "Speedreader Legacy Backend", base::FEATURE_DISABLED_BY_DEFAULT};

}  // namespace speedreader
