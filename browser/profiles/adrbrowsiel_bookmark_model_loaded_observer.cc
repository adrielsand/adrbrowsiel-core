/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/profiles/adrbrowsiel_bookmark_model_loaded_observer.h"

#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_sync/adrbrowsiel_sync_prefs.h"
#include "chrome/browser/profiles/profile.h"
#include "components/bookmarks/browser/bookmark_model.h"
#include "components/prefs/pref_service.h"

using bookmarks::BookmarkModel;

adrbrowsielBookmarkModelLoadedObserver::adrbrowsielBookmarkModelLoadedObserver(
    Profile* profile)
    : BookmarkModelLoadedObserver(profile) {}

void adrbrowsielBookmarkModelLoadedObserver::BookmarkModelLoaded(
    BookmarkModel* model,
    bool ids_reassigned) {
  if (!profile_->GetPrefs()->GetBoolean(kOtherBookmarksMigrated)) {
    adrbrowsielMigrateOtherNodeFolder(model);
    profile_->GetPrefs()->SetBoolean(kOtherBookmarksMigrated, true);
  }

  adrbrowsiel_sync::Prefs adrbrowsiel_sync_prefs(profile_->GetPrefs());
  if (!adrbrowsiel_sync_prefs.IsSyncV1MetaInfoCleared()) {
    adrbrowsielClearSyncV1MetaInfo(model);
    adrbrowsiel_sync_prefs.SetSyncV1MetaInfoCleared(true);
  }

  BookmarkModelLoadedObserver::BookmarkModelLoaded(model, ids_reassigned);
}
