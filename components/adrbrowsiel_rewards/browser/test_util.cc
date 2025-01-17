/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <utility>

#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/test_util.h"

#include "base/files/file_util.h"
#include "base/strings/utf_string_conversions.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/rewards_service.h"
#include "adrbrowsiel/browser/adrbrowsiel_rewards/rewards_service_factory.h"
#include "chrome/browser/bitmap_fetcher/bitmap_fetcher_service_factory.h"
#include "chrome/browser/prefs/browser_prefs.h"
#include "chrome/test/base/testing_profile.h"
#include "chrome/test/base/testing_profile_manager.h"
#include "components/sync_preferences/pref_service_mock_factory.h"
#include "components/sync_preferences/testing_pref_service_syncable.h"

namespace adrbrowsiel_rewards {

std::unique_ptr<Profile> CreateadrbrowsielRewardsProfile(const base::FilePath& path) {
  // Bitmap fetcher service needed for rewards service
  BitmapFetcherServiceFactory::GetInstance();
  RewardsServiceFactory::GetInstance();
  sync_preferences::PrefServiceMockFactory factory;
  auto registry = base::MakeRefCounted<user_prefs::PrefRegistrySyncable>();
  std::unique_ptr<sync_preferences::PrefServiceSyncable> prefs(
      factory.CreateSyncable(registry.get()));
  RegisterUserProfilePrefs(registry.get());
  RewardsService::RegisterProfilePrefs(registry.get());
  TestingProfile::Builder profile_builder;
  profile_builder.SetPrefService(std::move(prefs));
  profile_builder.SetPath(path);
  return profile_builder.Build();
}

}  // namespace adrbrowsiel_rewards
