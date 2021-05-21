/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/adrbrowsiel_ads/ads_service_factory.h"

#include <memory>

#include "base/time/time.h"
#include "adrbrowsiel/browser/profiles/profile_util.h"
#include "adrbrowsiel/components/adrbrowsiel_ads/browser/buildflags/buildflags.h"
#include "chrome/browser/profiles/incognito_helpers.h"
#include "chrome/browser/profiles/profile.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"

#if BUILDFLAG(adrbrowsiel_ADS_ENABLED)
#include "adrbrowsiel/browser/adrbrowsiel_rewards/rewards_service_factory.h"
#include "adrbrowsiel/components/adrbrowsiel_ads/browser/ads_service_impl.h"
#include "chrome/browser/dom_distiller/dom_distiller_service_factory.h"
#include "chrome/browser/history/history_service_factory.h"
#include "chrome/browser/notifications/notification_display_service_factory.h"
#include "components/history/core/browser/history_service.h"
#endif

namespace adrbrowsiel_ads {

// static
AdsService* AdsServiceFactory::GetForProfile(Profile* profile) {
  if (!adrbrowsiel::IsRegularProfile(profile)) {
    return nullptr;
  }

  return static_cast<AdsService*>(
      GetInstance()->GetServiceForBrowserContext(profile, true));
}

// static
AdsServiceFactory* AdsServiceFactory::GetInstance() {
  return base::Singleton<AdsServiceFactory>::get();
}

AdsServiceFactory::AdsServiceFactory()
    : BrowserContextKeyedServiceFactory(
          "AdsService",
          BrowserContextDependencyManager::GetInstance()) {
#if BUILDFLAG(adrbrowsiel_ADS_ENABLED)
  DependsOn(NotificationDisplayServiceFactory::GetInstance());
  DependsOn(dom_distiller::DomDistillerServiceFactory::GetInstance());
  DependsOn(adrbrowsiel_rewards::RewardsServiceFactory::GetInstance());
  DependsOn(HistoryServiceFactory::GetInstance());
#endif
}

AdsServiceFactory::~AdsServiceFactory() {}

KeyedService* AdsServiceFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
#if BUILDFLAG(adrbrowsiel_ADS_ENABLED)
  auto* profile = Profile::FromBrowserContext(context);
  std::unique_ptr<AdsServiceImpl> ads_service(new AdsServiceImpl(
      profile, HistoryServiceFactory::GetInstance()->GetForProfile(
                   profile, ServiceAccessType::EXPLICIT_ACCESS)));
  return ads_service.release();
#else
  return nullptr;
#endif
}

bool AdsServiceFactory::ServiceIsNULLWhileTesting() const {
  return false;
}

}  // namespace adrbrowsiel_ads
