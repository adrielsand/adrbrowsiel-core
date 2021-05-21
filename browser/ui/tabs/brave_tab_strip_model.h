/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_TABS_adrbrowsiel_TAB_STRIP_MODEL_H_
#define adrbrowsiel_BROWSER_UI_TABS_adrbrowsiel_TAB_STRIP_MODEL_H_

#include <vector>

#include "chrome/browser/ui/tabs/tab_strip_model.h"

class adrbrowsielTabStripModel : public TabStripModel {
 public:
  explicit adrbrowsielTabStripModel(TabStripModelDelegate* delegate,
                              Profile* profile);

  ~adrbrowsielTabStripModel() override;

  adrbrowsielTabStripModel(const adrbrowsielTabStripModel&) = delete;
  adrbrowsielTabStripModel operator=(const adrbrowsielTabStripModel&) = delete;

  void SelectRelativeTab(bool forward, UserGestureDetails detail) override;

  // Set the next tab when doing a MRU cycling with Ctrl-tab
  void SelectMRUTab(
      bool forward,
      UserGestureDetails detail = UserGestureDetails(GestureType::kOther));

  // Stop MRU cycling, called when releasing the Ctrl key
  void StopMRUCycling();

 private:
  // List of tab indexes sorted by most recently used
  std::vector<int> mru_cycle_list_;
};

#endif  // adrbrowsiel_BROWSER_UI_TABS_adrbrowsiel_TAB_STRIP_MODEL_H_
