/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/common/adrbrowsiel_switches.h"

#include "base/command_line.h"

namespace switches {

// Allows disabling the adrbrowsiel extension.
// This is commonly used for loading the extension manually to debug things
// in debug mode with auto-reloading.
const char kDisableadrbrowsielExtension[] = "disable-adrbrowsiel-extension";

// Allows disabling the adrbrowsiel Rewards extension.
const char kDisableadrbrowsielRewardsExtension[] = "disable-adrbrowsiel-rewards-extension";

// This switch disables update module(Sparkle).
const char kDisableadrbrowsielUpdate[] = "disable-adrbrowsiel-update";

// Allows disabling the WebTorrent extension.
const char kDisableWebTorrentExtension[] = "disable-webtorrent-extension";

// Allows disabling the Wayback Machine extension.
const char kDisableadrbrowsielWaybackMachineExtension[] =
    "disable-adrbrowsiel-wayback-machine-extension";

// Specifies overriding the built-in dark mode setting.
// Valid values are: "dark" | "light".
const char kDarkMode[] = "dark-mode";

// Allows disabling the machine ID generation on Windows.
const char kDisableMachineId[] = "disable-machine-id";

// Allows disabling encryption on Windows for cookies, passwords, settings...
// WARNING! Use ONLY if your hard drive is encrypted or if you know
// what you are doing.
const char kDisableEncryptionWin[] = "disable-encryption-win";

const char kFastWidevineBundleUpdate[] = "fast-widevine-bundle-update";

// Use custom update interval in sec
const char kComponentUpdateIntervalInSec[] = "component-update-interval-in-sec";

// Disables DOH using a runtime flag mainly for network audit
const char kDisableDnsOverHttps[] = "disable-doh";
}  // namespace switches
