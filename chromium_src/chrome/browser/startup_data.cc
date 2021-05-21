/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/macros.h"
#include "adrbrowsiel/browser/browser_context_keyed_service_factories.h"
#include "chrome/browser/profiles/chrome_browser_main_extra_parts_profiles.h"

namespace {

class adrbrowsielBrowserMainExtraPartsProfiles
    : public ChromeBrowserMainExtraPartsProfiles {
 public:
  adrbrowsielBrowserMainExtraPartsProfiles() = default;
  ~adrbrowsielBrowserMainExtraPartsProfiles() override = default;

  static void EnsureBrowserContextKeyedServiceFactoriesBuilt() {
    ChromeBrowserMainExtraPartsProfiles::
        EnsureBrowserContextKeyedServiceFactoriesBuilt();
    adrbrowsiel::EnsureBrowserContextKeyedServiceFactoriesBuilt();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(adrbrowsielBrowserMainExtraPartsProfiles);
};

}  // namespace

#define ChromeBrowserMainExtraPartsProfiles adrbrowsielBrowserMainExtraPartsProfiles
#include "../../../../chrome/browser/startup_data.cc"
#undef ChromeBrowserMainExtraPartsProfiles
