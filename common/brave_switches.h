/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMMON_adrbrowsiel_SWITCHES_H_
#define adrbrowsiel_COMMON_adrbrowsiel_SWITCHES_H_

namespace switches {

// All switches in alphabetical order. The switches should be documented
// alongside the definition of their values in the .cc file.
extern const char kDisableadrbrowsielExtension[];

extern const char kDisableadrbrowsielRewardsExtension[];

extern const char kDisableadrbrowsielUpdate[];

extern const char kDisableWebTorrentExtension[];

extern const char kDisableadrbrowsielWaybackMachineExtension[];

extern const char kRewards[];

extern const char kDarkMode[];

extern const char kDisableMachineId[];

extern const char kDisableEncryptionWin[];

extern const char kFastWidevineBundleUpdate[];

extern const char kComponentUpdateIntervalInSec[];

extern const char kDisableDnsOverHttps[];

}  // namespace switches

#endif  // adrbrowsiel_COMMON_adrbrowsiel_SWITCHES_H_
