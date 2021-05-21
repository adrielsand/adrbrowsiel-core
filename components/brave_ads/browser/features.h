/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_ADS_BROWSER_FEATURES_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_ADS_BROWSER_FEATURES_H_

#include "build/build_config.h"

namespace base {
struct Feature;
}  // namespace base

namespace adrbrowsiel_ads {
namespace features {

extern const base::Feature kAdNotifications;

bool IsAdNotificationsEnabled();

bool ShouldShowCustomAdNotifications();

int AdNotificationTimeout();

#if !defined(OS_ANDROID)

int AdNotificationFadeDuration();

double AdNotificationNormalizedDisplayCoordinateX();
int AdNotificationInsetX();
double AdNotificationNormalizedDisplayCoordinateY();
int AdNotificationInsetY();

#endif  // !defined(OS_ANDROID)

}  // namespace features
}  // namespace adrbrowsiel_ads

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_ADS_BROWSER_FEATURES_H_
