/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/sync/driver/adrbrowsiel_sync_profile_sync_service.h"

#include <utility>
#include <vector>

#include "base/logging.h"
#include "base/strings/string_util.h"
#include "adrbrowsiel/components/adrbrowsiel_sync/crypto/crypto.h"
#include "adrbrowsiel/components/sync/driver/adrbrowsiel_sync_auth_manager.h"
#include "adrbrowsiel/components/sync/driver/profile_sync_service_delegate.h"
#include "components/prefs/pref_service.h"

namespace syncer {

adrbrowsielProfileSyncService::adrbrowsielProfileSyncService(
    InitParams init_params,
    std::unique_ptr<ProfileSyncServiceDelegate> profile_service_delegate)
    : ProfileSyncService(std::move(init_params)),
      adrbrowsiel_sync_prefs_(sync_client_->GetPrefService()),
      profile_service_delegate_(std::move(profile_service_delegate)),
      weak_ptr_factory_(this) {
  adrbrowsiel_sync_prefs_change_registrar_.Init(sync_client_->GetPrefService());
  adrbrowsiel_sync_prefs_change_registrar_.Add(
      adrbrowsiel_sync::Prefs::GetSeedPath(),
      base::Bind(&adrbrowsielProfileSyncService::OnadrbrowsielSyncPrefsChanged,
                 base::Unretained(this)));
  GetadrbrowsielSyncAuthManager()->DeriveSigningKeys(adrbrowsiel_sync_prefs_.GetSeed());

  profile_service_delegate_->set_profile_sync_service(this);
}

adrbrowsielProfileSyncService::~adrbrowsielProfileSyncService() {
  adrbrowsiel_sync_prefs_change_registrar_.RemoveAll();
}

void adrbrowsielProfileSyncService::Initialize() {
  ProfileSyncService::Initialize();
  if (!adrbrowsiel_sync_prefs_.IsSyncV1Migrated()) {
    StopImpl(CLEAR_DATA);
    adrbrowsiel_sync_prefs_.SetSyncV1Migrated(true);
  }
}

bool adrbrowsielProfileSyncService::IsSetupInProgress() const {
  return ProfileSyncService::IsSetupInProgress() &&
         !user_settings_->IsFirstSetupComplete();
}

std::string adrbrowsielProfileSyncService::GetOrCreateSyncCode() {
  std::string sync_code = adrbrowsiel_sync_prefs_.GetSeed();
  if (sync_code.empty()) {
    std::vector<uint8_t> seed = adrbrowsiel_sync::crypto::GetSeed();
    sync_code = adrbrowsiel_sync::crypto::PassphraseFromBytes32(seed);
  }
  return sync_code;
}

bool adrbrowsielProfileSyncService::SetSyncCode(const std::string& sync_code) {
  std::vector<uint8_t> seed;
  std::string sync_code_trimmed;
  base::TrimString(sync_code, " \n\t", &sync_code_trimmed);
  if (!adrbrowsiel_sync::crypto::PassphraseToBytes32(sync_code_trimmed, &seed))
    return false;
  if (!adrbrowsiel_sync_prefs_.SetSeed(sync_code_trimmed))
    return false;
  return true;
}

void adrbrowsielProfileSyncService::OnSelfDeviceInfoDeleted(base::OnceClosure cb) {
  // This function will follow normal reset process and set SyncRequested to
  // false
  StopAndClear();
  adrbrowsiel_sync_prefs_.Clear();
  // Sync prefs will be clear in ProfileSyncService::StopImpl
  std::move(cb).Run();
}

adrbrowsielSyncAuthManager* adrbrowsielProfileSyncService::GetadrbrowsielSyncAuthManager() {
  return static_cast<adrbrowsielSyncAuthManager*>(auth_manager_.get());
}

void adrbrowsielProfileSyncService::OnadrbrowsielSyncPrefsChanged(const std::string& path) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (path == adrbrowsiel_sync::Prefs::GetSeedPath()) {
    const std::string seed = adrbrowsiel_sync_prefs_.GetSeed();
    if (!seed.empty()) {
      GetadrbrowsielSyncAuthManager()->DeriveSigningKeys(seed);
      // Default enabled types: Bookmarks
      syncer::UserSelectableTypeSet selected_types;
      selected_types.Put(UserSelectableType::kBookmarks);
      GetUserSettings()->SetSelectedTypes(false, selected_types);
    } else {
      VLOG(1) << "adrbrowsiel sync seed cleared";
      GetadrbrowsielSyncAuthManager()->ResetKeys();
    }
  }
}

void adrbrowsielProfileSyncService::SuspendDeviceObserverForOwnReset() {
  profile_service_delegate_->SuspendDeviceObserverForOwnReset();
}

void adrbrowsielProfileSyncService::ResumeDeviceObserver() {
  profile_service_delegate_->ResumeDeviceObserver();
}

}  // namespace syncer
