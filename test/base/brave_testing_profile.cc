/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/test/base/adrbrowsiel_testing_profile.h"

#include "adrbrowsiel/browser/profiles/profile_util.h"
#include "adrbrowsiel/common/pref_names.h"
#include "components/gcm_driver/gcm_buildflags.h"
#include "components/prefs/pref_service.h"

adrbrowsielTestingProfile::adrbrowsielTestingProfile(const base::FilePath& path,
                                         Delegate* delegate)
    : TestingProfile(path, delegate) {
  if (adrbrowsiel::IsSessionProfilePath(path)) {
    adrbrowsiel::CreateParentProfileData(this);
  }
}

adrbrowsielTestingProfile::adrbrowsielTestingProfile()
    : TestingProfile() {
#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
  GetPrefs()->SetBoolean(kadrbrowsielGCMChannelStatus, true);
#endif
}
