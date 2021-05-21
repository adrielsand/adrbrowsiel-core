/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_PROFILES_adrbrowsiel_RENDERER_UPDATER_FACTORY_H_
#define adrbrowsiel_BROWSER_PROFILES_adrbrowsiel_RENDERER_UPDATER_FACTORY_H_

#include "base/memory/singleton.h"
#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

class Profile;
class adrbrowsielRendererUpdater;

// Singleton that creates/deletes adrbrowsielRendererUpdater as new Profiles are
// created/shutdown.
class adrbrowsielRendererUpdaterFactory : public BrowserContextKeyedServiceFactory {
 public:
  // Returns an instance of the adrbrowsielRendererUpdaterFactory singleton.
  static adrbrowsielRendererUpdaterFactory* GetInstance();

  // Returns the instance of RendererUpdater for the passed |profile|.
  static adrbrowsielRendererUpdater* GetForProfile(Profile* profile);

  adrbrowsielRendererUpdaterFactory(const adrbrowsielRendererUpdaterFactory&) = delete;
  adrbrowsielRendererUpdaterFactory& operator=(const adrbrowsielRendererUpdaterFactory&) =
      delete;

 protected:
  // BrowserContextKeyedServiceFactory:
  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const override;
  bool ServiceIsCreatedWithBrowserContext() const override;

 private:
  friend struct base::DefaultSingletonTraits<adrbrowsielRendererUpdaterFactory>;

  content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const override;

  adrbrowsielRendererUpdaterFactory();
  ~adrbrowsielRendererUpdaterFactory() override;
};

#endif  // adrbrowsiel_BROWSER_PROFILES_adrbrowsiel_RENDERER_UPDATER_FACTORY_H_
