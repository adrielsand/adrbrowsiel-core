/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/tabs/adrbrowsiel_tab_strip_model.h"

#include <algorithm>

#include "adrbrowsiel/browser/ui/adrbrowsiel_browser_window.h"
#include "adrbrowsiel/common/pref_names.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/tabs/tab_strip_model_delegate.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_contents.h"

adrbrowsielTabStripModel::adrbrowsielTabStripModel(TabStripModelDelegate* delegate,
                                       Profile* profile)
    : TabStripModel(delegate, profile) {}
adrbrowsielTabStripModel::~adrbrowsielTabStripModel() {}

void adrbrowsielTabStripModel::SelectRelativeTab(bool forward,
                                           UserGestureDetails detail) {
  if (contents_data_.empty())
    return;

  bool is_mru_enabled = profile()->GetPrefs()->GetBoolean(kMRUCyclingEnabled);

  if (is_mru_enabled) {
    SelectMRUTab(forward, detail);
  } else {
    TabStripModel::SelectRelativeTab(forward, detail);
  }
}

void adrbrowsielTabStripModel::SelectMRUTab(bool forward, UserGestureDetails detail) {
  if (mru_cycle_list_.empty()) {
    // Start cycling

    Browser* browser = chrome::FindBrowserWithWebContents(GetWebContentsAt(0));
    if (!browser)
      return;

    // Create a list of tab indexes sorted by time of last activation
    for (int i = 0; i < count(); ++i) {
      mru_cycle_list_.push_back(i);
    }

    std::sort(mru_cycle_list_.begin(), mru_cycle_list_.end(),
              [this](int a, int b) {
                return GetWebContentsAt(a)->GetLastActiveTime() >
                       GetWebContentsAt(b)->GetLastActiveTime();
              });

    // Tell the cycling controller that we start cycling to handle tabs keys
    static_cast<adrbrowsielBrowserWindow*>(browser->window())->StartTabCycling();
  }

  if (forward) {
    std::rotate(mru_cycle_list_.begin(),
                mru_cycle_list_.begin() + 1,
                mru_cycle_list_.end());
  } else {
    std::rotate(mru_cycle_list_.rbegin(),
                mru_cycle_list_.rbegin() + 1,
                mru_cycle_list_.rend());
  }

  ActivateTabAt(mru_cycle_list_[0], detail);
}

void adrbrowsielTabStripModel::StopMRUCycling() {
  mru_cycle_list_.clear();
}
