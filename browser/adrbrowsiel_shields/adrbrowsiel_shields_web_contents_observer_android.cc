/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/adrbrowsiel_shields/adrbrowsiel_shields_web_contents_observer.h"

#include <string>

#include "adrbrowsiel/browser/android/adrbrowsiel_shields_content_settings.h"
#include "chrome/browser/android/tab_android.h"
#include "content/public/browser/web_contents.h"

using content::WebContents;

namespace adrbrowsiel_shields {
// static
void adrbrowsielShieldsWebContentsObserver::DispatchBlockedEventForWebContents(
    const std::string& block_type,
    const std::string& subresource,
    WebContents* web_contents) {
  if (!web_contents) {
    return;
  }

  int tabId = 0;
  TabAndroid* tab = TabAndroid::FromWebContents(web_contents);
  DCHECK(tab);
  if (tab) {
    tabId = tab->GetAndroidId();
  }
  chrome::android::adrbrowsielShieldsContentSettings::DispatchBlockedEvent(
      tabId, block_type, subresource);
}

}  // namespace adrbrowsiel_shields
