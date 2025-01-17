/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_TOR_TOR_TAB_HELPER_H_
#define adrbrowsiel_COMPONENTS_TOR_TOR_TAB_HELPER_H_

#include "base/memory/weak_ptr.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

namespace content {
class NavigationHandle;
class WebContents;
}  // namespace content

namespace tor {

class TorTabHelper : public content::WebContentsObserver,
                     public content::WebContentsUserData<TorTabHelper>,
                     public base::SupportsWeakPtr<TorTabHelper> {
 public:
  ~TorTabHelper() override;

  static void MaybeCreateForWebContents(content::WebContents* web_contents,
                                        bool is_tor_profile);

 private:
  friend class content::WebContentsUserData<TorTabHelper>;
  explicit TorTabHelper(content::WebContents* web_contents);

  // content::WebContentsObserver
  void DidFinishNavigation(
      content::NavigationHandle* navigation_handle) override;

  void ReloadTab(content::WebContents* web_contents);

  WEB_CONTENTS_USER_DATA_KEY_DECL();

  DISALLOW_COPY_AND_ASSIGN(TorTabHelper);
};

}  // namespace tor

#endif  // adrbrowsiel_COMPONENTS_TOR_TOR_TAB_HELPER_H_
