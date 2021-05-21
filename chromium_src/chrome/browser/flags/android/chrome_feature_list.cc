/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/common/adrbrowsiel_features.h"
#include "third_party/blink/public/common/features.h"

namespace chrome {
namespace android {

namespace {

const base::Feature* kadrbrowsielFeaturesExposedToJava[] = {
    &features::kadrbrowsielRewards,
    &blink::features::kForceWebContentsDarkMode,
};

const base::Feature* adrbrowsielFindFeatureExposedToJava(
    const std::string& feature_name) {
  for (size_t i = 0; i < base::size(kadrbrowsielFeaturesExposedToJava); ++i) {
    if (kadrbrowsielFeaturesExposedToJava[i]->name == feature_name)
      return kadrbrowsielFeaturesExposedToJava[i];
  }

  return nullptr;
}

}  // namespace

}  // namespace android
}  // namespace chrome

#define adrbrowsiel_FIND_FEATURE_EXPOSED_TO_JAVA                                    \
  const base::Feature* feature = adrbrowsielFindFeatureExposedToJava(feature_name); \
  if (feature)                                                                \
    return feature;

#include "../../../../../../chrome/browser/flags/android/chrome_feature_list.cc"
#undef adrbrowsiel_FIND_FEATURE_EXPOSED_TO_JAVA
