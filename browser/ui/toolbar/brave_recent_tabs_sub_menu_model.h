/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_TOOLBAR_adrbrowsiel_RECENT_TABS_SUB_MENU_MODEL_H_
#define adrbrowsiel_BROWSER_UI_TOOLBAR_adrbrowsiel_RECENT_TABS_SUB_MENU_MODEL_H_

#include "chrome/browser/ui/toolbar/recent_tabs_sub_menu_model.h"

class Browser;

namespace ui {
class AcceleratorProvider;
}

class adrbrowsielRecentTabsSubMenuModel : public RecentTabsSubMenuModel {
 public:
  adrbrowsielRecentTabsSubMenuModel(ui::AcceleratorProvider* accelerator_provider,
                         Browser* browser);

  ~adrbrowsielRecentTabsSubMenuModel() override;

  void ExecuteCommand(int command_id, int event_flags) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(adrbrowsielRecentTabsSubMenuModel);
};

#endif  // adrbrowsiel_BROWSER_UI_TOOLBAR_adrbrowsiel_RECENT_TABS_SUB_MENU_MODEL_H_
