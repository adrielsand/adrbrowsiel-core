/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_TOOLBAR_RECENT_TABS_SUB_MENU_MODEL_H_
#define adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_TOOLBAR_RECENT_TABS_SUB_MENU_MODEL_H_

#define adrbrowsiel_RECENT_TABS_SUB_MENU_MODEL_H_ \
  private: \
    std::auto_ptr<sessions::SessionTab> stub_tab_; \
    friend class adrbrowsielRecentTabsSubMenuModel; \
  public:
#include "../../../../../../chrome/browser/ui/toolbar/recent_tabs_sub_menu_model.h"

#undef adrbrowsiel_RECENT_TABS_SUB_MENU_MODEL_H_

#endif  // adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_TOOLBAR_RECENT_TABS_SUB_MENU_MODEL_H_
