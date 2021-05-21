/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_adrbrowsiel_BROWSER_H_
#define adrbrowsiel_BROWSER_UI_adrbrowsiel_BROWSER_H_

#include <memory>

#include "adrbrowsiel/components/sidebar/buildflags/buildflags.h"
#include "chrome/browser/ui/browser.h"

#if BUILDFLAG(ENABLE_SIDEBAR)
namespace sidebar {
class SidebarController;
}  // namespace sidebar

class adrbrowsielBrowserWindow;
#endif

class adrbrowsielBrowser : public Browser {
 public:
  explicit adrbrowsielBrowser(const CreateParams& params);
  ~adrbrowsielBrowser() override;

  adrbrowsielBrowser(const adrbrowsielBrowser&) = delete;
  adrbrowsielBrowser& operator=(const adrbrowsielBrowser&) = delete;

  // Browser overrides:
  void ScheduleUIUpdate(content::WebContents* source,
                        unsigned changed_flags) override;
  void OnTabStripModelChanged(
      TabStripModel* tab_strip_model,
      const TabStripModelChange& change,
      const TabStripSelectionChange& selection) override;

#if BUILDFLAG(ENABLE_SIDEBAR)
  sidebar::SidebarController* sidebar_controller() {
    return sidebar_controller_.get();
  }
#endif

  adrbrowsielBrowserWindow* adrbrowsiel_window();

 private:
#if BUILDFLAG(ENABLE_SIDEBAR)
  std::unique_ptr<sidebar::SidebarController> sidebar_controller_;
#endif
};

#endif  // adrbrowsiel_BROWSER_UI_adrbrowsiel_BROWSER_H_
