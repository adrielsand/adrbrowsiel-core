/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/permissions/permission_manager_factory.h"

#include "adrbrowsiel/browser/geolocation/adrbrowsiel_geolocation_permission_context_delegate.h"
#include "adrbrowsiel/browser/permissions/permission_lifetime_manager_factory.h"
#include "adrbrowsiel/components/permissions/permission_lifetime_manager.h"
#include "components/permissions/features.h"

#define GeolocationPermissionContextDelegate \
  adrbrowsielGeolocationPermissionContextDelegate

#define BuildServiceInstanceFor BuildServiceInstanceFor_ChromiumImpl

#include "../../../../../chrome/browser/permissions/permission_manager_factory.cc"

#undef GeolocationPermissionContextDelegate
#undef BuildServiceInstanceFor

KeyedService* PermissionManagerFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  Profile* profile = Profile::FromBrowserContext(context);
  auto permission_contexts = CreatePermissionContexts(profile);
  if (base::FeatureList::IsEnabled(
          permissions::features::kPermissionLifetime)) {
    auto* lifetime_manager =
        PermissionLifetimeManagerFactory::GetInstance()->GetForProfile(profile);
    for (auto& permission_context : permission_contexts) {
      permission_context.second->SetPermissionLifetimeManager(lifetime_manager);
    }
  }
  return new permissions::PermissionManager(profile,
                                            std::move(permission_contexts));
}
