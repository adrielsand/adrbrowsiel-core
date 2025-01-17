/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_WEBUI_SETTINGS_adrbrowsiel_RELAUNCH_HANDLER_H_
#define adrbrowsiel_BROWSER_UI_WEBUI_SETTINGS_adrbrowsiel_RELAUNCH_HANDLER_H_

#include "chrome/browser/ui/webui/settings/settings_page_ui_handler.h"

class Profile;

class adrbrowsielRelaunchHandler : public settings::SettingsPageUIHandler {
 public:
  adrbrowsielRelaunchHandler() = default;
  ~adrbrowsielRelaunchHandler() override = default;

 private:
  // SettingsPageUIHandler overrides:
  void RegisterMessages() override;
  void OnJavascriptAllowed() override {}
  void OnJavascriptDisallowed() override {}

  void Relaunch(const base::ListValue* args);

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielRelaunchHandler);
};

#endif  // adrbrowsiel_BROWSER_UI_WEBUI_SETTINGS_adrbrowsiel_RELAUNCH_HANDLER_H_
