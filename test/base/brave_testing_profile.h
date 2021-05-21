/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_TEST_BASE_adrbrowsiel_TESTING_PROFILE_H_
#define adrbrowsiel_TEST_BASE_adrbrowsiel_TESTING_PROFILE_H_

#include "chrome/test/base/testing_profile.h"

class adrbrowsielTestingProfile : public TestingProfile {
 public:
  adrbrowsielTestingProfile();
  adrbrowsielTestingProfile(const base::FilePath& path, Delegate* delegate);
  ~adrbrowsielTestingProfile() override = default;
};

#endif  // adrbrowsiel_TEST_BASE_adrbrowsiel_TESTING_PROFILE_H_
