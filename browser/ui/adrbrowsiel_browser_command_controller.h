/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_adrbrowsiel_BROWSER_COMMAND_CONTROLLER_H_
#define adrbrowsiel_BROWSER_UI_adrbrowsiel_BROWSER_COMMAND_CONTROLLER_H_

#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"
#include "adrbrowsiel/components/tor/buildflags/buildflags.h"
#include "chrome/browser/ui/browser_command_controller.h"
#if BUILDFLAG(IPFS_ENABLED)
#include "components/prefs/pref_change_registrar.h"
#endif

// This namespace is needed for a chromium_src override
namespace chrome {

class adrbrowsielBrowserCommandController : public chrome::BrowserCommandController {
 public:
  explicit adrbrowsielBrowserCommandController(Browser* browser);

#if BUILDFLAG(ENABLE_TOR)
  void UpdateCommandForTor();
#endif

 private:
  // Overriden from CommandUpdater:
  bool SupportsCommand(int id) const override;
  bool IsCommandEnabled(int id) const override;
  bool ExecuteCommandWithDisposition(
      int id,
      WindowOpenDisposition disposition,
      base::TimeTicks time_stamp = base::TimeTicks::Now()) override;
  void AddCommandObserver(int id, CommandObserver* observer) override;
  void RemoveCommandObserver(int id, CommandObserver* observer) override;
  void RemoveCommandObserver(CommandObserver* observer) override;
  bool UpdateCommandEnabled(int id, bool state) override;

  void InitadrbrowsielCommandState();
  void UpdateCommandForadrbrowsielRewards();
  void UpdateCommandForadrbrowsielAdblock();
  void UpdateCommandForWebcompatReporter();
  void UpdateCommandForadrbrowsielSync();
  void UpdateCommandForadrbrowsielWallet();
  void UpdateCommandForSidebar();
  bool ExecuteadrbrowsielCommandWithDisposition(int id,
                                          WindowOpenDisposition disposition,
                                          base::TimeTicks time_stamp);

  Browser* const browser_;

  CommandUpdaterImpl adrbrowsiel_command_updater_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielBrowserCommandController);
};

}   // namespace chrome

#endif  // adrbrowsiel_BROWSER_UI_adrbrowsiel_BROWSER_COMMAND_CONTROLLER_H_
