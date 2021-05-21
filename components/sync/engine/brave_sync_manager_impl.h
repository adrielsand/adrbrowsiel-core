/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_SYNC_ENGINE_adrbrowsiel_SYNC_MANAGER_IMPL_H_
#define adrbrowsiel_COMPONENTS_SYNC_ENGINE_adrbrowsiel_SYNC_MANAGER_IMPL_H_

#include <string>

#include "components/sync/engine/sync_manager_impl.h"

namespace syncer {

class adrbrowsielSyncManagerImpl : public SyncManagerImpl {
 public:
  adrbrowsielSyncManagerImpl(
      const std::string& name,
      network::NetworkConnectionTracker* network_connection_tracker);
  ~adrbrowsielSyncManagerImpl() override;

  void StartSyncingNormally(base::Time last_poll_time) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(adrbrowsielSyncManagerImpl);
};

}  // namespace syncer

#endif  // adrbrowsiel_COMPONENTS_SYNC_ENGINE_adrbrowsiel_SYNC_MANAGER_IMPL_H_
