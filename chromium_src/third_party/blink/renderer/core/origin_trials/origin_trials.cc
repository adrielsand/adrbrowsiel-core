/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "third_party/blink/renderer/core/origin_trials/origin_trials.h"

namespace blink {
namespace origin_trials {
bool IsTrialValid_ChromiumImpl(const StringView& trial_name);
}  // namespace origin_trials
}  // namespace blink

#define IsTrialValid IsTrialValid_ChromiumImpl
#include "../gen/third_party/blink/renderer/core/origin_trials/origin_trials.cc"
#undef IsTrialValid

namespace blink {
namespace origin_trials {

bool IsTrialDisabledInadrbrowsiel(const StringView& trial_name) {
  // When updating also update the array in the overload below.
  static const char* const kadrbrowsielDisabledTrialNames[] = {
      "DigitalGoods",
      "HandwritingRecognition",
      "SignedExchangeSubresourcePrefetch",
      "SubresourceWebBundles",
      "TrustTokens",
  };

  if (base::Contains(kadrbrowsielDisabledTrialNames, trial_name)) {
    // Check if this is still a valid trial name in Chromium. If not, it needs
    // to be changed as in Chromium or removed.
    DCHECK(IsTrialValid_ChromiumImpl(trial_name));
    return true;
  }

  return false;
}

bool IsTrialDisabledInadrbrowsiel(OriginTrialFeature feature) {
  // When updating also update the array in the overload above.
  static const std::array<OriginTrialFeature, 5> kadrbrowsielDisabledTrialFeatures = {
      OriginTrialFeature::kDigitalGoods,
      OriginTrialFeature::kHandwritingRecognition,
      OriginTrialFeature::kSignedExchangeSubresourcePrefetch,
      OriginTrialFeature::kSubresourceWebBundles,
      OriginTrialFeature::kTrustTokens,
  };

  return base::Contains(kadrbrowsielDisabledTrialFeatures, feature);
}

bool IsTrialValid(const StringView& trial_name) {
  if (IsTrialDisabledInadrbrowsiel(trial_name))
    return false;

  return IsTrialValid_ChromiumImpl(trial_name);
}

}  // namespace origin_trials
}  // namespace blink
