/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define adrbrowsiel_IPFS L"ipfs"
#define adrbrowsiel_IPNS L"ipns"

#define adrbrowsiel_GET_TARGET_FOR_DEFAULT_APP_SETTINGS                         \
  if (base::EqualsCaseInsensitiveASCII(protocol, adrbrowsiel_IPFS))             \
    return base::StringPrintf(kSystemSettingsDefaultAppsFormat, L"IPFS"); \
  if (base::EqualsCaseInsensitiveASCII(protocol, adrbrowsiel_IPNS))             \
    return base::StringPrintf(kSystemSettingsDefaultAppsFormat, L"IPNS");

#include "../../../../../chrome/installer/util/shell_util.cc"
#undef adrbrowsiel_GET_TARGET_FOR_DEFAULT_APP_SETTINGS
#undef adrbrowsiel_IPFS
#undef adrbrowsiel_IPNS
