// Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "adrbrowsiel/browser/ftx/ftx_service_factory.h"

#include "adrbrowsiel/browser/profiles/profile_util.h"
#include "adrbrowsiel/components/ftx/browser/ftx_service.h"
#include "chrome/browser/profiles/incognito_helpers.h"
#include "chrome/browser/profiles/profile.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"

// static
FTXServiceFactory* FTXServiceFactory::GetInstance() {
  return base::Singleton<FTXServiceFactory>::get();
}

// static
FTXService* FTXServiceFactory::GetForProfile(Profile* profile) {
  if (!adrbrowsiel::IsRegularProfile(profile)) {
    return nullptr;
  }

  return static_cast<FTXService*>(
      GetInstance()->GetServiceForBrowserContext(profile, true));
}

FTXServiceFactory::FTXServiceFactory()
    : BrowserContextKeyedServiceFactory(
          "FTXService",
          BrowserContextDependencyManager::GetInstance()) {}

FTXServiceFactory::~FTXServiceFactory() {}

KeyedService* FTXServiceFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  return new FTXService(context);
}

content::BrowserContext* FTXServiceFactory::GetBrowserContextToUse(
    content::BrowserContext* context) const {
  return chrome::GetBrowserContextRedirectedInIncognito(context);
}
