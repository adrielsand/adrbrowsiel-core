/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_SYNC_DRIVER_adrbrowsiel_SYNC_PROFILE_SYNC_SERVICE_H_
#define adrbrowsiel_COMPONENTS_SYNC_DRIVER_adrbrowsiel_SYNC_PROFILE_SYNC_SERVICE_H_

#include <memory>
#include <string>

#include "base/memory/weak_ptr.h"
#include "adrbrowsiel/components/adrbrowsiel_sync/adrbrowsiel_sync_prefs.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/sync/driver/profile_sync_service.h"

class Profile;

namespace syncer {

class adrbrowsielSyncAuthManager;
class ProfileSyncServiceDelegate;

class adrbrowsielProfileSyncService : public ProfileSyncService {
 public:
  explicit adrbrowsielProfileSyncService(
      InitParams init_params,
      std::unique_ptr<ProfileSyncServiceDelegate> profile_service_delegate);
  ~adrbrowsielProfileSyncService() override;

  // SyncService implementation
  bool IsSetupInProgress() const override;

  std::string GetOrCreateSyncCode();
  bool SetSyncCode(const std::string& sync_code);

  // This should only be called by helper function, adrbrowsiel_sync::ResetSync, or by
  // OnDeviceInfoChange internally
  void OnSelfDeviceInfoDeleted(base::OnceClosure cb);

  // These functions are for disabling device_info_observer_ from firing
  // when the device is doing own reset sync operation, to prevent early call
  // of StopAndClear prior to device sends delete record
  void SuspendDeviceObserverForOwnReset();
  void ResumeDeviceObserver();

  void Initialize() override;

 private:
  adrbrowsielSyncAuthManager* GetadrbrowsielSyncAuthManager();

  void OnadrbrowsielSyncPrefsChanged(const std::string& path);

  adrbrowsiel_sync::Prefs adrbrowsiel_sync_prefs_;

  PrefChangeRegistrar adrbrowsiel_sync_prefs_change_registrar_;

  std::unique_ptr<ProfileSyncServiceDelegate> profile_service_delegate_;

  base::WeakPtrFactory<adrbrowsielProfileSyncService> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielProfileSyncService);
};
}  // namespace syncer

#endif  // adrbrowsiel_COMPONENTS_SYNC_DRIVER_adrbrowsiel_SYNC_PROFILE_SYNC_SERVICE_H_
