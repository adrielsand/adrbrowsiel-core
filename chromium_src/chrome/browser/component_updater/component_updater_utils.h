/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_COMPONENT_UPDATER_COMPONENT_UPDATER_UTILS_H_
#define adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_COMPONENT_UPDATER_COMPONENT_UPDATER_UTILS_H_

#include <string>

#include "../../../../../chrome/browser/component_updater/component_updater_utils.h"

namespace component_updater {

void adrbrowsielOnDemandUpdate(const std::string& component_id);

}  // namespace component_updater

#endif  // adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_COMPONENT_UPDATER_COMPONENT_UPDATER_UTILS_H_
