/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */


#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_SHIELDS_COMMON_adrbrowsiel_SHIELD_CONSTANTS_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_SHIELDS_COMMON_adrbrowsiel_SHIELD_CONSTANTS_H_

namespace adrbrowsiel_shields {

const char kAds[] = "shieldsAds";
const char kCosmeticFiltering[] = "cosmeticFiltering";
const char kTrackers[] = "trackers";
const char kHTTPUpgradableResources[] = "httpUpgradableResources";
const char kJavaScript[] = "javascript";
const char kFingerprintingV2[] = "fingerprintingV2";
const char kadrbrowsielShields[] = "adrbrowsielShields";
const char kReferrers[] = "referrers";
const char kCookies[] = "shieldsCookies";
const char kFacebookEmbeds[] = "fb-embeds";
const char kTwitterEmbeds[] = "twitter-embeds";
const char kLinkedInEmbeds[] = "linked-in-embeds";

// Values used before the migration away from ResourceIdentifier, kept around
// for migration purposes only.
const char kObsoleteAds[] = "ads";
const char kObsoleteCookies[] = "cookies";

// Some users were not properly migrated from fingerprinting V1.
const char kObsoleteFingerprinting[] = "fingerprinting";

}  // namespace adrbrowsiel_shields

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_SHIELDS_COMMON_adrbrowsiel_SHIELD_CONSTANTS_H_
