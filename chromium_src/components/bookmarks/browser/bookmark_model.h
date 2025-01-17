/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_COMPONENTS_BOOKMARKS_BROWSER_BOOKMARK_MODEL_H_
#define adrbrowsiel_CHROMIUM_SRC_COMPONENTS_BOOKMARKS_BROWSER_BOOKMARK_MODEL_H_

class adrbrowsielSyncServiceTestDelayedLoadModel;

#define adrbrowsiel_BOOKMARK_MODEL_H \
 private: \
  friend class ::adrbrowsielSyncServiceTestDelayedLoadModel;

#include "../../../../../components/bookmarks/browser/bookmark_model.h"

namespace bookmarks {
void adrbrowsielMigrateOtherNodeFolder(BookmarkModel* model);
void adrbrowsielClearSyncV1MetaInfo(BookmarkModel* model);
}  // namespace bookmarks

#endif  // adrbrowsiel_CHROMIUM_SRC_COMPONENTS_BOOKMARKS_BROWSER_BOOKMARK_MODEL_H_
