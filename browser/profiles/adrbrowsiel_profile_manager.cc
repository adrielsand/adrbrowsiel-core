// Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "adrbrowsiel/browser/profiles/adrbrowsiel_profile_manager.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/metrics/histogram_macros.h"
#include "adrbrowsiel/browser/adrbrowsiel_ads/ads_service_factory.h"
#include "adrbrowsiel/browser/adrbrowsiel_rewards/rewards_service_factory.h"
#include "adrbrowsiel/browser/profiles/profile_util.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/buildflags/buildflags.h"
#include "adrbrowsiel/components/content_settings/core/browser/adrbrowsiel_content_settings_pref_provider.h"
#include "adrbrowsiel/components/decentralized_dns/buildflags/buildflags.h"
#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"
#include "adrbrowsiel/components/tor/tor_constants.h"
#include "adrbrowsiel/content/browser/webui/adrbrowsiel_shared_resources_data_source.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/chrome_notification_types.h"
#include "chrome/browser/profiles/profile_attributes_entry.h"
#include "chrome/browser/profiles/profile_attributes_storage.h"
#include "chrome/browser/profiles/profiles_state.h"
#include "chrome/common/chrome_constants.h"
#include "chrome/common/pref_names.h"
#include "chrome/grit/generated_resources.h"
#include "components/bookmarks/common/bookmark_pref_names.h"
#include "components/gcm_driver/gcm_buildflags.h"
#include "components/prefs/pref_service.h"
#include "components/signin/public/base/signin_pref_names.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/url_data_source.h"
#include "ui/base/l10n/l10n_util.h"

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
#include "adrbrowsiel/browser/adrbrowsiel_wallet/adrbrowsiel_wallet_service_factory.h"
#endif

#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
#include "adrbrowsiel/browser/gcm_driver/adrbrowsiel_gcm_channel_status.h"
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "adrbrowsiel/browser/ipfs/ipfs_service_factory.h"
#endif

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
#include "adrbrowsiel/browser/decentralized_dns/decentralized_dns_service_factory.h"
#endif

using content::BrowserThread;

namespace {

void AddadrbrowsielSharedResourcesDataSourceToProfile(Profile* profile) {
  content::URLDataSource::Add(
      profile,
      std::make_unique<adrbrowsiel_content::adrbrowsielSharedResourcesDataSource>());
}

}  // namespace

adrbrowsielProfileManager::adrbrowsielProfileManager(const base::FilePath& user_data_dir)
    : ProfileManager(user_data_dir) {
  MigrateProfileNames();

  AddObserver(this);
}

adrbrowsielProfileManager::~adrbrowsielProfileManager() {
  std::vector<Profile*> profiles = GetLoadedProfiles();
  for (Profile* profile : profiles) {
    if (adrbrowsiel::IsSessionProfile(profile)) {
      // passing false for `success` removes the profile from the info cache
      OnProfileCreated(profile, false, false);
    }
  }
  RemoveObserver(this);
}

void adrbrowsielProfileManager::InitProfileUserPrefs(Profile* profile) {
  // migrate obsolete plugin prefs to temporary migration pref because otherwise
  // they get deleteed by PrefProvider before we can migrate them in
  // adrbrowsielPrefProvider
  content_settings::adrbrowsielPrefProvider::CopyPluginSettingsForMigration(
      profile->GetPrefs());

  ProfileManager::InitProfileUserPrefs(profile);
  adrbrowsiel::RecordInitialP3AValues(profile);
  adrbrowsiel::SetDefaultSearchVersion(profile, profile->IsNewProfile());
}

void adrbrowsielProfileManager::DoFinalInitForServices(Profile* profile,
                                                 bool go_off_the_record) {
  ProfileManager::DoFinalInitForServices(profile, go_off_the_record);
  if (!do_final_services_init_)
    return;
  adrbrowsiel_ads::AdsServiceFactory::GetForProfile(profile);
  adrbrowsiel_rewards::RewardsServiceFactory::GetForProfile(profile);
#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
  adrbrowsielWalletServiceFactory::GetForContext(profile);
#endif
#if BUILDFLAG(IPFS_ENABLED)
  ipfs::IpfsServiceFactory::GetForContext(profile);
#endif
#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
  decentralized_dns::DecentralizedDnsServiceFactory::GetForContext(profile);
#endif
#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
  gcm::adrbrowsielGCMChannelStatus* status =
      gcm::adrbrowsielGCMChannelStatus::GetForProfile(profile);
  DCHECK(status);
  status->UpdateGCMDriverStatus();
#endif
}

bool adrbrowsielProfileManager::IsAllowedProfilePath(
    const base::FilePath& path) const {
  // Chromium only allows profiles to be created in the user_data_dir, but we
  // want to also be able to create profile in subfolders of user_data_dir.
  return ProfileManager::IsAllowedProfilePath(path) ||
         user_data_dir().IsParent(path.DirName());
}

bool adrbrowsielProfileManager::LoadProfileByPath(const base::FilePath& profile_path,
                         bool incognito,
                         ProfileLoadedCallback callback) {
  // Prevent legacy tor session profile to be loaded so we won't hit
  // DCHECK(!GetProfileAttributesWithPath(...)). Workaround for legacy tor guest
  // profile won't work because when AddProfile to storage we will hit
  // DCHECK(user_data_dir_ == profile_path.DirName()), legacy tor session
  // profile was not under user_data_dir like legacy tor guest profile did.
  if (profile_path.BaseName().value() == tor::kTorProfileDir) {
    return false;
  }

  return ProfileManager::LoadProfileByPath(profile_path, incognito,
                                           std::move(callback));
}

// This overridden method doesn't clear |kDefaultSearchProviderDataPrefName|.
// W/o this, prefs set by TorWindowSearchEngineProviderService is cleared
// during the initialization.
void adrbrowsielProfileManager::SetNonPersonalProfilePrefs(Profile* profile) {
  PrefService* prefs = profile->GetPrefs();
  prefs->SetBoolean(prefs::kSigninAllowed, false);
  prefs->SetBoolean(bookmarks::prefs::kEditBookmarksEnabled, false);
  prefs->SetBoolean(bookmarks::prefs::kShowBookmarkBar, false);
}

void adrbrowsielProfileManager::MigrateProfileNames() {
#if !defined(OS_ANDROID) && !defined(OS_CHROMEOS)
  // If any profiles have a default name using an
  // older version of the default name string format,
  // then name it with the new default name string format.
  // e.g. 'Person X' --> 'Profile X'.
  ProfileAttributesStorage& storage = GetProfileAttributesStorage();
  std::vector<ProfileAttributesEntry*> entries =
      storage.GetAllProfilesAttributesSortedByName();
  // Make sure we keep the numbering the same.
  for (auto* entry : entries) {
    // Rename the necessary profiles. Don't check for legacy names as profile
    // info cache should have migrated them by now.
    if (entry->IsUsingDefaultName() &&
        !storage.IsDefaultProfileName(
            entry->GetName(),
            /*include_check_for_legacy_profile_name=*/false)) {
      auto icon_index = entry->GetAvatarIconIndex();
      entry->SetLocalProfileName(storage.ChooseNameForNewProfile(icon_index),
                                 /*is_default_name=*/true);
    }
  }
#endif
}

void adrbrowsielProfileManager::OnProfileAdded(Profile* profile) {
  // Observe new profiles for creation of OTR profiles so that we can add our
  // shared resources to them.
  observed_profiles_.Add(profile);
  AddadrbrowsielSharedResourcesDataSourceToProfile(profile);
}

void adrbrowsielProfileManager::OnOffTheRecordProfileCreated(
    Profile* off_the_record) {
  AddadrbrowsielSharedResourcesDataSourceToProfile(off_the_record);
}

void adrbrowsielProfileManager::OnProfileWillBeDestroyed(Profile* profile) {
  if (!profile->IsOffTheRecord()) {
    observed_profiles_.Remove(profile);
  }
}

adrbrowsielProfileManagerWithoutInit::adrbrowsielProfileManagerWithoutInit(
    const base::FilePath& user_data_dir)
    : adrbrowsielProfileManager(user_data_dir) {
  set_do_final_services_init(false);
}
