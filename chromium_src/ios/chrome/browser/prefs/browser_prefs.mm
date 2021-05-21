/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/adrbrowsiel_sync/adrbrowsiel_sync_prefs.h"

void adrbrowsielRegisterBrowserStatePrefs(PrefRegistrySimple* registry) {
  adrbrowsiel_sync::Prefs::RegisterProfilePrefs(registry);
}

#define adrbrowsiel_REGISTER_BROWSER_STATE_PREFS adrbrowsielRegisterBrowserStatePrefs(registry);
#include "../../../../../../ios/chrome/browser/prefs/browser_prefs.mm"
