/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_BOOKMARK_adrbrowsiel_BOOKMARK_TAB_HELPER_H_
#define adrbrowsiel_BROWSER_UI_BOOKMARK_adrbrowsiel_BOOKMARK_TAB_HELPER_H_

#include "chrome/browser/ui/bookmarks/bookmark_tab_helper.h"
#include "content/public/browser/web_contents_user_data.h"

class BookmarkTabHelperObserver;

// This proxies BookmarkTabHelper apis that used by Browser.
class adrbrowsielBookmarkTabHelper
    : public content::WebContentsUserData<adrbrowsielBookmarkTabHelper>{
 public:
  ~adrbrowsielBookmarkTabHelper() override;

  bool ShouldShowBookmarkBar();
  void AddObserver(BookmarkTabHelperObserver* observer);
  void RemoveObserver(BookmarkTabHelperObserver* observer);

 private:
  friend class content::WebContentsUserData<adrbrowsielBookmarkTabHelper>;

  explicit adrbrowsielBookmarkTabHelper(content::WebContents* web_contents);

  content::WebContents* web_contents_;

  WEB_CONTENTS_USER_DATA_KEY_DECL();

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielBookmarkTabHelper);
};

#endif  // adrbrowsiel_BROWSER_UI_BOOKMARK_adrbrowsiel_BOOKMARK_TAB_HELPER_H_
