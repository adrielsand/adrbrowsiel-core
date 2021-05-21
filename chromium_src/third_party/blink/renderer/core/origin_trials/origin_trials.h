/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_ORIGIN_TRIALS_ORIGIN_TRIALS_H_
#define adrbrowsiel_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_ORIGIN_TRIALS_ORIGIN_TRIALS_H_

#include "../../../../../../../third_party/blink/renderer/core/origin_trials/origin_trials.h"

namespace blink {
namespace origin_trials {

bool IsTrialDisabledInadrbrowsiel(const StringView& trial_name);
bool IsTrialDisabledInadrbrowsiel(OriginTrialFeature feature);

}  // namespace origin_trials
}  // namespace blink

#endif  // adrbrowsiel_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_CORE_ORIGIN_TRIALS_ORIGIN_TRIALS_H_
