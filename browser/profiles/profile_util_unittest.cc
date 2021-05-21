// Copyright 2019 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "adrbrowsiel/browser/profiles/profile_util.h"

#include "base/files/file_path.h"
#include "base/files/scoped_temp_dir.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/search_engines/adrbrowsiel_prepopulated_engines.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/test/base/testing_browser_process.h"
#include "chrome/test/base/testing_profile.h"
#include "chrome/test/base/testing_profile_manager.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gtest/include/gtest/gtest.h"

class adrbrowsielProfileUtilTest : public testing::Test {
 public:
  adrbrowsielProfileUtilTest()
      : testing_profile_manager_(TestingBrowserProcess::GetGlobal()) {}
  ~adrbrowsielProfileUtilTest() override {}

 protected:
  void SetUp() override {
    ASSERT_TRUE(temp_dir_.CreateUniqueTempDir());
    ASSERT_TRUE(testing_profile_manager_.SetUp(temp_dir_.GetPath()));
  }

  Profile* GetProfile() { return ProfileManager::GetActiveUserProfile(); }

  PrefService* GetPrefs() {
    return ProfileManager::GetActiveUserProfile()->GetPrefs();
  }

  content::BrowserTaskEnvironment task_environment_;
  TestingProfileManager testing_profile_manager_;
  base::ScopedTempDir temp_dir_;
};

// No entry yet. Check initialized value
TEST_F(adrbrowsielProfileUtilTest, SetDefaultSearchVersionExistingProfileNoEntryYet) {
  const PrefService::Preference* pref =
      GetPrefs()->FindPreference(kadrbrowsielDefaultSearchVersion);
  EXPECT_TRUE(pref->IsDefaultValue());
  adrbrowsiel::SetDefaultSearchVersion(GetProfile(), false);
  ASSERT_EQ(GetPrefs()->GetInteger(kadrbrowsielDefaultSearchVersion),
            TemplateURLPrepopulateData::kadrbrowsielFirstTrackedDataVersion);
}

TEST_F(adrbrowsielProfileUtilTest, SetDefaultSearchVersionNewProfileNoEntryYet) {
  const PrefService::Preference* pref =
      GetPrefs()->FindPreference(kadrbrowsielDefaultSearchVersion);
  EXPECT_TRUE(pref->IsDefaultValue());
  adrbrowsiel::SetDefaultSearchVersion(GetProfile(), true);
  ASSERT_EQ(GetPrefs()->GetInteger(kadrbrowsielDefaultSearchVersion),
            TemplateURLPrepopulateData::kadrbrowsielCurrentDataVersion);
}

// Entry there; ensure value is kept
TEST_F(adrbrowsielProfileUtilTest,
       SetDefaultSearchVersionExistingProfileHasEntryKeepsValue) {
  GetPrefs()->SetInteger(kadrbrowsielDefaultSearchVersion, 1);
  const PrefService::Preference* pref =
      GetPrefs()->FindPreference(kadrbrowsielDefaultSearchVersion);
  EXPECT_FALSE(pref->IsDefaultValue());
  adrbrowsiel::SetDefaultSearchVersion(GetProfile(), false);
  ASSERT_EQ(GetPrefs()->GetInteger(kadrbrowsielDefaultSearchVersion), 1);
}

TEST_F(adrbrowsielProfileUtilTest,
       SetDefaultSearchVersionNewProfileHasEntryKeepsValue) {
  // This is an anomaly case; new profile won't ever have a hard set value
  GetPrefs()->SetInteger(kadrbrowsielDefaultSearchVersion, 1);
  const PrefService::Preference* pref =
      GetPrefs()->FindPreference(kadrbrowsielDefaultSearchVersion);
  EXPECT_FALSE(pref->IsDefaultValue());
  adrbrowsiel::SetDefaultSearchVersion(GetProfile(), true);
  ASSERT_EQ(GetPrefs()->GetInteger(kadrbrowsielDefaultSearchVersion), 1);
}
