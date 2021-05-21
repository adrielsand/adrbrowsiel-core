// Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "adrbrowsiel/browser/ntp_background_images/view_counter_service_factory.h"

#include <memory>

#include "adrbrowsiel/browser/adrbrowsiel_ads/ads_service_factory.h"
#include "adrbrowsiel/browser/adrbrowsiel_browser_process.h"
#include "adrbrowsiel/browser/profiles/profile_util.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_ads/browser/ads_service.h"
#include "adrbrowsiel/components/ntp_background_images/browser/ntp_background_images_service.h"
#include "adrbrowsiel/components/ntp_background_images/browser/ntp_background_images_source.h"
#include "adrbrowsiel/components/ntp_background_images/browser/view_counter_service.h"
#include "adrbrowsiel/components/ntp_background_images/common/pref_names.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"
#include "components/keyed_service/content/browser_context_keyed_service_factory.h"
#include "components/pref_registry/pref_registry_syncable.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/url_data_source.h"

namespace ntp_background_images {

// static
ViewCounterService* ViewCounterServiceFactory::GetForProfile(Profile* profile) {
  return static_cast<ViewCounterService*>(
      GetInstance()->GetServiceForBrowserContext(profile, true));
}

// static
ViewCounterServiceFactory* ViewCounterServiceFactory::GetInstance() {
  return base::Singleton<ViewCounterServiceFactory>::get();
}

ViewCounterServiceFactory::ViewCounterServiceFactory()
    : BrowserContextKeyedServiceFactory(
          "ViewCounterService",
          BrowserContextDependencyManager::GetInstance()) {
  DependsOn(adrbrowsiel_ads::AdsServiceFactory::GetInstance());
}

ViewCounterServiceFactory::~ViewCounterServiceFactory() {}

KeyedService* ViewCounterServiceFactory::BuildServiceInstanceFor(
    content::BrowserContext* browser_context) const {
  // Only NTP in normal profile uses sponsored services.
  if (!adrbrowsiel::IsRegularProfile(browser_context))
    return nullptr;

  if (auto* service =
          g_adrbrowsiel_browser_process->ntp_background_images_service()) {
    Profile* profile = Profile::FromBrowserContext(browser_context);
    bool is_supported_locale = false;
    auto* ads_service = adrbrowsiel_ads::AdsServiceFactory::GetForProfile(profile);
    if (ads_service) {
      is_supported_locale = ads_service->IsSupportedLocale();
    }
    content::URLDataSource::Add(
        browser_context,
        std::make_unique<NTPBackgroundImagesSource>(service));

    return new ViewCounterService(service, ads_service, profile->GetPrefs(),
                                  g_browser_process->local_state(),
                                  is_supported_locale);
  }

  return nullptr;
}

void ViewCounterServiceFactory::RegisterProfilePrefs(
     user_prefs::PrefRegistrySyncable* registry) {
  ViewCounterService::RegisterProfilePrefs(registry);
}

bool ViewCounterServiceFactory::ServiceIsCreatedWithBrowserContext() const {
  return true;
}

}  // namespace ntp_background_images
