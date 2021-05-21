/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_EXTENSIONS_adrbrowsiel_WALLET_UTIL_H_
#define adrbrowsiel_BROWSER_EXTENSIONS_adrbrowsiel_WALLET_UTIL_H_

#include <string>

namespace extensions {

std::string GetInfuraProjectID();

std::string GetadrbrowsielKey();

bool HasInfuraProjectID();

}  // namespace extensions

#endif  // adrbrowsiel_BROWSER_EXTENSIONS_adrbrowsiel_WALLET_UTIL_H_
