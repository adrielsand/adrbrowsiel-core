/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_REWARDS_BROWSER_ANDROID_UTIL_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_REWARDS_BROWSER_ANDROID_UTIL_H_

#include <string>

#include "bat/ledger/ledger_client.h"

namespace android_util {

ledger::type::ClientInfoPtr GetAndroidClientInfo();

}  // namespace android_util

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_REWARDS_BROWSER_ANDROID_UTIL_H_
