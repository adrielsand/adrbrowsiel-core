/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/speedreader/speedreader_service_factory.h"

#include "adrbrowsiel/components/speedreader/speedreader_service.h"
#include "chrome/browser/profiles/incognito_helpers.h"
#include "chrome/browser/profiles/profile.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"

namespace speedreader {

// static
SpeedreaderServiceFactory* SpeedreaderServiceFactory::GetInstance() {
  return base::Singleton<SpeedreaderServiceFactory>::get();
}

SpeedreaderService* SpeedreaderServiceFactory::GetForProfile(Profile* profile) {
  return static_cast<SpeedreaderService*>(
      SpeedreaderServiceFactory::GetInstance()->GetServiceForBrowserContext(
          profile, true /*create*/));
}

SpeedreaderServiceFactory::SpeedreaderServiceFactory()
    : BrowserContextKeyedServiceFactory(
          "SpeedreaderService",
          BrowserContextDependencyManager::GetInstance()) {}

SpeedreaderServiceFactory::~SpeedreaderServiceFactory() {}

content::BrowserContext*
SpeedreaderServiceFactory::GetBrowserContextToUse(
    content::BrowserContext* context) const {
  return chrome::GetBrowserContextOwnInstanceInIncognito(context);
}

KeyedService* SpeedreaderServiceFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  return new SpeedreaderService(
      Profile::FromBrowserContext(context)->GetPrefs());
}

bool SpeedreaderServiceFactory::ServiceIsCreatedWithBrowserContext() const {
  return true;
}

}  // namespace speedreader
