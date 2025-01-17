/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_H_
#define adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_H_

#define PrimaryID PrimaryID(); static const OTRProfileID TorID
#define HasPrimaryOTRProfile IsTor() const override; bool HasPrimaryOTRProfile
#define IsIncognitoProfile                 \
  IsIncognitoProfile_ChromiumImpl() const; \
  bool IsIncognitoProfile
#include "../../../../../chrome/browser/profiles/profile.h"
#undef IsIncognitoProfile
#undef PrimaryID
#undef HasPrimaryOTRProfile

#endif  // adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_H_
