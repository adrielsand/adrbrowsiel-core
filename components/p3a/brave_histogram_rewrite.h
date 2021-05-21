/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_P3A_adrbrowsiel_HISTOGRAM_REWRITE_H_
#define adrbrowsiel_COMPONENTS_P3A_adrbrowsiel_HISTOGRAM_REWRITE_H_

namespace adrbrowsiel {

// Set callbacks for existing Chromium histograms that will be adrbrowsielized,
// i.e. reemitted using a different name and custom buckets.
void SetupHistogramsadrbrowsielization();

}  // namespace adrbrowsiel

#endif  // adrbrowsiel_COMPONENTS_P3A_adrbrowsiel_HISTOGRAM_REWRITE_H_
