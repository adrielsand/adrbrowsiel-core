/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_PERMISSIONS_PERMISSION_LIFETIME_MANAGER_FACTORY_H_
#define adrbrowsiel_BROWSER_PERMISSIONS_PERMISSION_LIFETIME_MANAGER_FACTORY_H_

#include "base/memory/singleton.h"
#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

namespace content {
class BrowserContext;
}

namespace permissions {
class PermissionLifetimeManager;
}

class Profile;

class PermissionLifetimeManagerFactory
    : public BrowserContextKeyedServiceFactory {
 public:
  static permissions::PermissionLifetimeManager* GetForProfile(
      content::BrowserContext* profile);
  static PermissionLifetimeManagerFactory* GetInstance();

 private:
  friend struct base::DefaultSingletonTraits<PermissionLifetimeManagerFactory>;

  PermissionLifetimeManagerFactory();
  PermissionLifetimeManagerFactory(const PermissionLifetimeManagerFactory&) =
      delete;
  PermissionLifetimeManagerFactory& operator=(
      const PermissionLifetimeManagerFactory&) = delete;
  ~PermissionLifetimeManagerFactory() override;

  // BrowserContextKeyedServiceFactory methods:
  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const override;
  bool ServiceIsCreatedWithBrowserContext() const override;
  content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const override;
  void RegisterProfilePrefs(
      user_prefs::PrefRegistrySyncable* registry) override;
};

#endif  // adrbrowsiel_BROWSER_PERMISSIONS_PERMISSION_LIFETIME_MANAGER_FACTORY_H_
