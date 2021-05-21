/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_adrbrowsiel_BROWSER_WINDOW_H_
#define adrbrowsiel_BROWSER_UI_adrbrowsiel_BROWSER_WINDOW_H_

#include "adrbrowsiel/components/sidebar/buildflags/buildflags.h"
#include "chrome/browser/ui/browser_window.h"

namespace sidebar {
class Sidebar;
}  // namespace sidebar

class adrbrowsielBrowserWindow : public BrowserWindow {
 public:
  ~adrbrowsielBrowserWindow() override {}

  virtual void StartTabCycling() = 0;

#if BUILDFLAG(ENABLE_SIDEBAR)
  virtual sidebar::Sidebar* InitSidebar() = 0;
#endif
};

#endif  // adrbrowsiel_BROWSER_UI_adrbrowsiel_BROWSER_WINDOW_H_
