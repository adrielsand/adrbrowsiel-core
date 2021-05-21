/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_VERSION_INFO_VERSION_INFO_H_
#define adrbrowsiel_COMPONENTS_VERSION_INFO_VERSION_INFO_H_

#include <string>

namespace version_info {

std::string GetadrbrowsielVersionWithoutChromiumMajorVersion();
std::string GetadrbrowsielVersionNumberForDisplay();
std::string GetadrbrowsielChromiumVersionNumber();

}  // namespace version_info

#endif  // adrbrowsiel_COMPONENTS_VERSION_INFO_VERSION_INFO_H_
