/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_ANDROID_PREFERENCES_WEBSITE_DESKTOP_MODE_TAB_HELPER_H_
#define adrbrowsiel_BROWSER_ANDROID_PREFERENCES_WEBSITE_DESKTOP_MODE_TAB_HELPER_H_

#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

// This manages desktop mode option for each WebContents.
class DesktopModeTabHelper
    : public content::WebContentsObserver,
      public content::WebContentsUserData<DesktopModeTabHelper> {
 public:
  explicit DesktopModeTabHelper(content::WebContents* contents);
  ~DesktopModeTabHelper() override;

  // content::WebContentsObserver overrides:
  void NavigationEntryCommitted(
      const content::LoadCommittedDetails& load_details) override;
  void DidStartNavigation(
      content::NavigationHandle* navigation_handle) override;

  WEB_CONTENTS_USER_DATA_KEY_DECL();

 private:
  bool need_override_ua_ = false;

  DISALLOW_COPY_AND_ASSIGN(DesktopModeTabHelper);
};

#endif  // adrbrowsiel_BROWSER_ANDROID_PREFERENCES_WEBSITE_DESKTOP_MODE_TAB_HELPER_H_
