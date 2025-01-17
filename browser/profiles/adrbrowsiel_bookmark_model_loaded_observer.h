/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_PROFILES_adrbrowsiel_BOOKMARK_MODEL_LOADED_OBSERVER_H_
#define adrbrowsiel_BROWSER_PROFILES_adrbrowsiel_BOOKMARK_MODEL_LOADED_OBSERVER_H_

#include "chrome/browser/profiles/bookmark_model_loaded_observer.h"

class adrbrowsielBookmarkModelLoadedObserver
    : public BookmarkModelLoadedObserver {
 public:
  explicit adrbrowsielBookmarkModelLoadedObserver(Profile* profile);

 private:
  void BookmarkModelLoaded(bookmarks::BookmarkModel* model,
                           bool ids_reassigned) override;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielBookmarkModelLoadedObserver);
};

#endif  // adrbrowsiel_BROWSER_PROFILES_adrbrowsiel_BOOKMARK_MODEL_LOADED_OBSERVER_H_
