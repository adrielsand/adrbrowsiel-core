/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_SYNC_DRIVER_adrbrowsiel_SYNC_AUTH_MANAGER_H_
#define adrbrowsiel_COMPONENTS_SYNC_DRIVER_adrbrowsiel_SYNC_AUTH_MANAGER_H_

#include <string>
#include <vector>

#include "base/time/time.h"
#include "components/sync/driver/sync_auth_manager.h"

namespace syncer {

class adrbrowsielSyncAuthManager : public SyncAuthManager {
 public:
  adrbrowsielSyncAuthManager(signin::IdentityManager* identity_manager,
                       const AccountStateChangedCallback& account_state_changed,
                       const CredentialsChangedCallback& credentials_changed);
  ~adrbrowsielSyncAuthManager() override;

  void DeriveSigningKeys(const std::string& seed);
  void ResetKeys();

  void RequestAccessToken() override;

  // signin::IdentityManager::Observer implementation.
  void OnRefreshTokenUpdatedForAccount(
      const CoreAccountInfo& account_info) override {}
  void OnRefreshTokenRemovedForAccount(
      const CoreAccountId& account_id) override {}
  void OnRefreshTokensLoaded() override {}

 private:
  SyncAccountInfo DetermineAccountToUse() const override;
  std::string GenerateAccessToken(const std::string& timestamp);
  void OnNetworkTimeFetched(const base::Time& time);

  std::vector<uint8_t> public_key_;
  std::vector<uint8_t> private_key_;

  base::WeakPtrFactory<adrbrowsielSyncAuthManager> weak_ptr_factory_{this};

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielSyncAuthManager);
};

}  // namespace syncer

#endif  // adrbrowsiel_COMPONENTS_SYNC_DRIVER_adrbrowsiel_SYNC_AUTH_MANAGER_H_
