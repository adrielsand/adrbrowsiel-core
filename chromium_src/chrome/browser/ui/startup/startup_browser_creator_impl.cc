/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/startup/default_adrbrowsiel_browser_prompt.h"
#include "chrome/browser/ui/startup/google_api_keys_infobar_delegate.h"
#include "components/infobars/core/confirm_infobar_delegate.h"

#define GoogleApiKeysInfoBarDelegate adrbrowsielGoogleKeysInfoBarDelegate

class adrbrowsielGoogleKeysInfoBarDelegate {
 public:
  static void Create(InfoBarService* infobar_service) {
    // lulz
  }
};

#define adrbrowsiel_STARTUPBROWSERCREATORIMPL_DETERMINEURLSANDLAUNCH \
  welcome_enabled = true;

#define ShowDefaultBrowserPrompt ShowDefaultadrbrowsielBrowserPrompt

#include "../../../../../../chrome/browser/ui/startup/startup_browser_creator_impl.cc"  // NOLINT

#undef adrbrowsiel_STARTUPBROWSERCREATORIMPL_DETERMINEURLSANDLAUNCH
#undef GoogleApiKeysInfoBarDelegate
#undef ShowDefaultBrowserPrompt
