/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_adrbrowsiel_ADS_BOUNDS_UTIL_H_
#define adrbrowsiel_BROWSER_UI_adrbrowsiel_ADS_BOUNDS_UTIL_H_

#include "ui/gfx/native_widget_types.h"

namespace gfx {
class Rect;
}  // namespace gfx

namespace adrbrowsiel_ads {

void AdjustBoundsToFitWorkAreaForNativeView(gfx::Rect* bounds,
                                            gfx::NativeView native_view);

}  // namespace adrbrowsiel_ads

#endif  // adrbrowsiel_BROWSER_UI_adrbrowsiel_ADS_BOUNDS_UTIL_H_
