/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/sync/engine/adrbrowsiel_sync_manager_impl.h"

namespace syncer {

adrbrowsielSyncManagerImpl::adrbrowsielSyncManagerImpl(
    const std::string& name,
    network::NetworkConnectionTracker* network_connection_tracker)
    : SyncManagerImpl(name, network_connection_tracker) {}

adrbrowsielSyncManagerImpl::~adrbrowsielSyncManagerImpl() {}

void adrbrowsielSyncManagerImpl::StartSyncingNormally(base::Time last_poll_time) {
  SyncManagerImpl::StartSyncingNormally(last_poll_time);
  // Remove this hack when we have FCM invalidation integrated.
  // We only enable BOOKMARKS by default so only force refresh it
  RefreshTypes(ModelTypeSet(BOOKMARKS));
}

}  // namespace syncer
