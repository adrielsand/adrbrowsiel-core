/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#define adrbrowsiel_INIT adrbrowsielInit();
#include "../../../../../../components/content_settings/core/browser/content_settings_registry.cc"
#undef adrbrowsiel_INIT

#include "adrbrowsiel/components/adrbrowsiel_shields/common/adrbrowsiel_shield_constants.h"
namespace content_settings {

namespace {

const struct {
  const char* name;
  ContentSettingsType type;
} kadrbrowsielContentSettingstypes[] = {
    {adrbrowsiel_shields::kAds, ContentSettingsType::adrbrowsiel_ADS},
    {adrbrowsiel_shields::kCosmeticFiltering,
     ContentSettingsType::adrbrowsiel_COSMETIC_FILTERING},
    {adrbrowsiel_shields::kTrackers, ContentSettingsType::adrbrowsiel_TRACKERS},
    {adrbrowsiel_shields::kHTTPUpgradableResources,
     ContentSettingsType::adrbrowsiel_HTTP_UPGRADABLE_RESOURCES},
    {adrbrowsiel_shields::kFingerprintingV2,
     ContentSettingsType::adrbrowsiel_FINGERPRINTING_V2},
    {adrbrowsiel_shields::kadrbrowsielShields, ContentSettingsType::adrbrowsiel_SHIELDS},
    {adrbrowsiel_shields::kReferrers, ContentSettingsType::adrbrowsiel_REFERRERS},
    {adrbrowsiel_shields::kCookies, ContentSettingsType::adrbrowsiel_COOKIES},
};

}  // namespace

void ContentSettingsRegistry::RegisteradrbrowsielContentSettingsTypes(
    const ContentSettingsType& type,
    const std::string& name) {
  Register(type, name, CONTENT_SETTING_BLOCK, WebsiteSettingsInfo::SYNCABLE,
           AllowlistedSchemes(kChromeUIScheme, kChromeDevToolsScheme),
           ValidSettings(CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK,
                         CONTENT_SETTING_ASK,
                         CONTENT_SETTING_DETECT_IMPORTANT_CONTENT),
           WebsiteSettingsInfo::SINGLE_ORIGIN_WITH_EMBEDDED_EXCEPTIONS_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IN_INCOGNITO,
           ContentSettingsInfo::PERSISTENT,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);
}

void ContentSettingsRegistry::adrbrowsielInit() {
  // Add CONTENT_SETTING_ASK for autoplay
  // Note ASK has been deprecated, only keeping it for
  // DiscardObsoleteAutoplayAsk test case
  content_settings_info_.erase(ContentSettingsType::AUTOPLAY);
  website_settings_registry_->UnRegister(ContentSettingsType::AUTOPLAY);
  Register(ContentSettingsType::AUTOPLAY, "autoplay", CONTENT_SETTING_ALLOW,
           WebsiteSettingsInfo::UNSYNCABLE, AllowlistedSchemes(),
           ValidSettings(CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK,
                         CONTENT_SETTING_ASK),
           WebsiteSettingsInfo::SINGLE_ORIGIN_ONLY_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IN_INCOGNITO,
           ContentSettingsInfo::PERSISTENT,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);

  // Register adrbrowsiel-specific types, defaulting them to CONTENT_SETTING_BLOCK.
  for (auto adrbrowsiel_type : kadrbrowsielContentSettingstypes)
    RegisteradrbrowsielContentSettingsTypes(adrbrowsiel_type.type, adrbrowsiel_type.name);

  // Disable background sync by default (adrbrowsiel/adrbrowsiel-browser#4709)
  content_settings_info_.erase(ContentSettingsType::BACKGROUND_SYNC);
  website_settings_registry_->UnRegister(ContentSettingsType::BACKGROUND_SYNC);
  Register(ContentSettingsType::BACKGROUND_SYNC, "background-sync",
           CONTENT_SETTING_BLOCK, WebsiteSettingsInfo::UNSYNCABLE,
           AllowlistedSchemes(),
           ValidSettings(CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK),
           WebsiteSettingsInfo::SINGLE_ORIGIN_ONLY_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IN_INCOGNITO,
           ContentSettingsInfo::PERSISTENT,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_ORIGINS_ONLY);

  // Disable motion sensors by default (adrbrowsiel/adrbrowsiel-browser#4789)
  content_settings_info_.erase(ContentSettingsType::SENSORS);
  website_settings_registry_->UnRegister(ContentSettingsType::SENSORS);
  Register(ContentSettingsType::SENSORS, "sensors", CONTENT_SETTING_BLOCK,
           WebsiteSettingsInfo::UNSYNCABLE, AllowlistedSchemes(),
           ValidSettings(CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK),
           WebsiteSettingsInfo::SINGLE_ORIGIN_ONLY_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IN_INCOGNITO,
           ContentSettingsInfo::PERSISTENT,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);
}

}  // namespace content_settings
