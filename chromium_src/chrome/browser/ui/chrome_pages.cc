/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// It has caller in the same file
#define adrbrowsiel_GET_SETTINGS_URL \
GURL GetSettingsUrl_ChromiumImpl(const std::string& sub_page) {
// adrbrowsiel_GET_SETTINGS_URL

#include "../../../../../chrome/browser/ui/chrome_pages.cc"

#undef adrbrowsiel_GET_SETTINGS_URL

#include "adrbrowsiel/common/webui_url_constants.h"

namespace chrome {

GURL GetSettingsUrl(const std::string& sub_page) {
  if (sub_page == chrome::kSyncSetupSubPage)
    return chrome::GetSettingsUrl(kadrbrowsielSyncSetupPath);
  return GetSettingsUrl_ChromiumImpl(sub_page);
}

}  // namespace chrome
