/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_TOR_TOR_PROFILE_MANAGER_H_
#define adrbrowsiel_BROWSER_TOR_TOR_PROFILE_MANAGER_H_

#include <string>

#include "base/containers/flat_map.h"
#include "base/no_destructor.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/browser/profiles/profile_observer.h"
#include "chrome/browser/ui/browser_list_observer.h"

class TorProfileManager : public BrowserListObserver, public ProfileObserver {
 public:
  static TorProfileManager& GetInstance();
  static void SwitchToTorProfile(Profile* original_profile,
                                 ProfileManager::CreateCallback callback);
  static void CloseTorProfileWindows(Profile* tor_profile);
  Profile* GetTorProfile(Profile* original_profile);

 private:
  friend class base::NoDestructor<TorProfileManager>;
  TorProfileManager();
  ~TorProfileManager() override;

  // BrowserListObserver:
  void OnBrowserRemoved(Browser* browser) override;

  // ProfileObserver:
  void OnProfileWillBeDestroyed(Profile* profile) override;

  void InitTorProfileUserPrefs(Profile* profile);

  // One regular profile can only have one tor profile
  base::flat_map<std::string, Profile*> tor_profiles_;

  TorProfileManager(const TorProfileManager&) = delete;
  TorProfileManager& operator=(const TorProfileManager&) = delete;
};

#endif  // adrbrowsiel_BROWSER_TOR_TOR_PROFILE_MANAGER_H_
