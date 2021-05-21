/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/profiles/adrbrowsiel_renderer_updater_factory.h"

#include "adrbrowsiel/browser/profiles/adrbrowsiel_renderer_updater.h"
#include "chrome/browser/profiles/incognito_helpers.h"
#include "chrome/browser/profiles/profile.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"

adrbrowsielRendererUpdaterFactory::adrbrowsielRendererUpdaterFactory()
    : BrowserContextKeyedServiceFactory(
          "adrbrowsielRendererUpdater",
          BrowserContextDependencyManager::GetInstance()) {}

adrbrowsielRendererUpdaterFactory::~adrbrowsielRendererUpdaterFactory() {}

// static
adrbrowsielRendererUpdaterFactory* adrbrowsielRendererUpdaterFactory::GetInstance() {
  return base::Singleton<adrbrowsielRendererUpdaterFactory>::get();
}

// static
adrbrowsielRendererUpdater* adrbrowsielRendererUpdaterFactory::GetForProfile(
    Profile* profile) {
  return static_cast<adrbrowsielRendererUpdater*>(
      GetInstance()->GetServiceForBrowserContext(profile, true));
}

KeyedService* adrbrowsielRendererUpdaterFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  return new adrbrowsielRendererUpdater(static_cast<Profile*>(context));
}

bool adrbrowsielRendererUpdaterFactory::ServiceIsCreatedWithBrowserContext() const {
  return true;
}

content::BrowserContext* adrbrowsielRendererUpdaterFactory::GetBrowserContextToUse(
    content::BrowserContext* context) const {
  return chrome::GetBrowserContextRedirectedInIncognito(context);
}
