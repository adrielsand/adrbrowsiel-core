/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/pref_names.h"

namespace ads {

namespace prefs {

// Stores whether adrbrowsiel ads is enabled or disabled
const char kEnabled[] = "adrbrowsiel.adrbrowsiel_ads.enabled";

// Stores whether adrbrowsiel ads should allow conversion tracking
const char kShouldAllowConversionTracking[] =
    "adrbrowsiel.adrbrowsiel_ads.should_allow_ad_conversion_tracking";

// Stores the maximum amount of ads per hour
const char kAdsPerHour[] = "adrbrowsiel.adrbrowsiel_ads.ads_per_hour";

// Stores the idle time threshold before checking if an ad can be served
const char kIdleTimeThreshold[] = "adrbrowsiel.adrbrowsiel_ads.idle_threshold";

// Stores whether adrbrowsiel ads should allow subdivision ad targeting
const char kShouldAllowAdsSubdivisionTargeting[] =
    "adrbrowsiel.adrbrowsiel_ads.should_allow_ads_subdivision_targeting";

// Stores the selected ads subdivision targeting code
const char kAdsSubdivisionTargetingCode[] =
    "adrbrowsiel.adrbrowsiel_ads.ads_subdivision_targeting_code";

// Stores the automatically detected ads subdivision targeting code
const char kAutoDetectedAdsSubdivisionTargetingCode[] =
    "adrbrowsiel.adrbrowsiel_ads.automatically_detected_ads_subdivision_targeting_code";

// Stores catalog id
const char kCatalogId[] = "adrbrowsiel.adrbrowsiel_ads.catalog_id";

// Stores catalog version
const char kCatalogVersion[] = "adrbrowsiel.adrbrowsiel_ads.catalog_version";

// Stores catalog ping
const char kCatalogPing[] = "adrbrowsiel.adrbrowsiel_ads.catalog_ping";

// Stores catalog last updated
const char kCatalogLastUpdated[] = "adrbrowsiel.adrbrowsiel_ads.catalog_last_updated";

// Stores epsilon greedy bandit arms
const char kEpsilonGreedyBanditArms[] =
    "adrbrowsiel.adrbrowsiel_ads.epsilon_greedy_bandit_arms";

// Stores epsilon greedy bandit eligible segments
const char kEpsilonGreedyBanditEligibleSegments[] =
    "adrbrowsiel.adrbrowsiel_ads.epsilon_greedy_bandit_eligible_segments";

// Stores migration status
const char kHasMigratedConversionState[] =
    "adrbrowsiel.adrbrowsiel_ads.migrated.conversion_state";

}  // namespace prefs

}  // namespace ads
