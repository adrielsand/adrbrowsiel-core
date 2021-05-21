/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/adrbrowsiel_ads/common/pref_names.h"

namespace adrbrowsiel_ads {

namespace prefs {

// Stores whether ads were disabled at least once
const char kAdsWereDisabled[] = "adrbrowsiel.adrbrowsiel_ads.were_disabled";

// Indicates whether we have any initial state of the ads status metric, besides
// "No Wallet".
const char kHasAdsP3AState[] = "adrbrowsiel.adrbrowsiel_ads.has_p3a_state";

// Prefix for preference names pertaining to p2a weekly metrics
const char kP2AStoragePrefNamePrefix[] = "adrbrowsiel.weekly_storage.";

// Stores whether we should show the My First Ad notification
const char kShouldShowMyFirstAdNotification[] =
    "adrbrowsiel.adrbrowsiel_ads.should_show_my_first_ad_notification";

// Stores the supported country codes current schema version number
const char kSupportedCountryCodesLastSchemaVersion[] =
    "adrbrowsiel.adrbrowsiel_ads.supported_regions_last_schema_version_number";
const char kSupportedCountryCodesSchemaVersion[] =
    "adrbrowsiel.adrbrowsiel_ads.supported_regions_schema_version_number";
const int kSupportedCountryCodesSchemaVersionNumber = 9;

// Stores the ad notifications last screen position
const char kAdNotificationLastScreenPositionX[] =
    "adrbrowsiel.adrbrowsiel_ads.ad_notification.last_screen_position_x";
const char kAdNotificationLastScreenPositionY[] =
    "adrbrowsiel.adrbrowsiel_ads.ad_notification.last_screen_position_y";

// Stores the preferences version number
const char kVersion[] = "adrbrowsiel.adrbrowsiel_ads.prefs.version";

const int kCurrentVersionNumber = 10;

}  // namespace prefs

}  // namespace adrbrowsiel_ads
