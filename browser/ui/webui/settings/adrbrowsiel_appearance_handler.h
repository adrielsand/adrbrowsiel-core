/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_WEBUI_SETTINGS_adrbrowsiel_APPEARANCE_HANDLER_H_
#define adrbrowsiel_BROWSER_UI_WEBUI_SETTINGS_adrbrowsiel_APPEARANCE_HANDLER_H_

#include <string>

#include "chrome/browser/ui/webui/settings/settings_page_ui_handler.h"
#include "components/prefs/pref_change_registrar.h"

class Profile;

class adrbrowsielAppearanceHandler : public settings::SettingsPageUIHandler {
 public:
  adrbrowsielAppearanceHandler();
  ~adrbrowsielAppearanceHandler() override;

  adrbrowsielAppearanceHandler(const adrbrowsielAppearanceHandler&) = delete;
  adrbrowsielAppearanceHandler& operator=(const adrbrowsielAppearanceHandler&) = delete;

 private:
  // SettingsPageUIHandler overrides:
  void RegisterMessages() override;
  void OnJavascriptAllowed() override {}
  void OnJavascriptDisallowed() override {}

  void OnadrbrowsielDarkModeChanged();
  void OnBackgroundPreferenceChanged(const std::string& pref_name);
  void OnPreferenceChanged(const std::string& pref_name);
  void SetadrbrowsielThemeType(const base::ListValue* args);
  void GetadrbrowsielThemeType(const base::ListValue* args);
  void GetNewTabShowsOptionsList(const base::ListValue* args);
  void ShouldShowNewTabDashboardSettings(const base::ListValue* args);

  Profile* profile_ = nullptr;
  PrefChangeRegistrar local_state_change_registrar_;
  PrefChangeRegistrar profile_state_change_registrar_;
};

#endif  // adrbrowsiel_BROWSER_UI_WEBUI_SETTINGS_adrbrowsiel_APPEARANCE_HANDLER_H_
