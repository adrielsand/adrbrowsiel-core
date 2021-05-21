/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_adrbrowsiel_ADS_ADS_SERVICE_FACTORY_H_
#define adrbrowsiel_BROWSER_adrbrowsiel_ADS_ADS_SERVICE_FACTORY_H_

#include "base/memory/singleton.h"
#include "adrbrowsiel/components/adrbrowsiel_ads/browser/ads_service.h"
#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

class Profile;

namespace adrbrowsiel_ads {

// Singleton that owns all AdsService and associates them with Profiles.
class AdsServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  AdsServiceFactory(const AdsServiceFactory&) = delete;
  AdsServiceFactory& operator=(const AdsServiceFactory&) = delete;

  static AdsService* GetForProfile(Profile* profile);

  static AdsServiceFactory* GetInstance();

 private:
  friend struct base::DefaultSingletonTraits<AdsServiceFactory>;

  AdsServiceFactory();
  ~AdsServiceFactory() override;

  // BrowserContextKeyedServiceFactory:
  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const override;
  bool ServiceIsNULLWhileTesting() const override;
};

}  // namespace adrbrowsiel_ads

#endif  // adrbrowsiel_BROWSER_adrbrowsiel_ADS_ADS_SERVICE_FACTORY_H_
