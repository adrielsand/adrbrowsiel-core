/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_PROFILES_adrbrowsiel_PROFILE_SHORTCUT_MANAGER_WIN_H_
#define adrbrowsiel_BROWSER_PROFILES_adrbrowsiel_PROFILE_SHORTCUT_MANAGER_WIN_H_

#include "chrome/browser/profiles/profile_shortcut_manager_win.h"

class adrbrowsielProfileShortcutManagerWin : public ProfileShortcutManagerWin {
 public:
  explicit adrbrowsielProfileShortcutManagerWin(ProfileManager* manager);
  ~adrbrowsielProfileShortcutManagerWin() override = default;

  void GetShortcutProperties(const base::FilePath& profile_path,
                             base::CommandLine* command_line,
                             std::wstring* name,
                             base::FilePath* icon_path) override;

 private:
  ProfileManager* profile_manager_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielProfileShortcutManagerWin);
};

#endif  // adrbrowsiel_BROWSER_PROFILES_adrbrowsiel_PROFILE_SHORTCUT_MANAGER_WIN_H_
