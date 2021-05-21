/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/version_info/version_info.h"

namespace version_info {

std::string GetadrbrowsielVersionWithoutChromiumMajorVersion() {
  return std::string(adrbrowsiel_BROWSER_VERSION);
}

std::string GetadrbrowsielVersionNumberForDisplay() {
  return std::string(adrbrowsiel_BROWSER_VERSION) +
         "  Chromium: " + adrbrowsiel_CHROMIUM_VERSION;
}

std::string GetadrbrowsielChromiumVersionNumber() {
  return std::string(adrbrowsiel_CHROMIUM_VERSION);
}

}  // namespace version_info
