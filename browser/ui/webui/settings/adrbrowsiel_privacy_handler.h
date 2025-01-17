/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_WEBUI_SETTINGS_adrbrowsiel_PRIVACY_HANDLER_H_
#define adrbrowsiel_BROWSER_UI_WEBUI_SETTINGS_adrbrowsiel_PRIVACY_HANDLER_H_

#include "adrbrowsiel/components/p3a/buildflags.h"
#include "chrome/browser/ui/webui/settings/settings_page_ui_handler.h"
#include "components/prefs/pref_change_registrar.h"

namespace content {
class WebUIDataSource;
}

class Profile;

class adrbrowsielPrivacyHandler : public settings::SettingsPageUIHandler {
 public:
  adrbrowsielPrivacyHandler();
  ~adrbrowsielPrivacyHandler() override;
  static void AddLoadTimeData(content::WebUIDataSource* data_source,
                              Profile* profile);

 private:
  // SettingsPageUIHandler overrides:
  void RegisterMessages() override;
  void OnJavascriptAllowed() override {}
  void OnJavascriptDisallowed() override {}

#if BUILDFLAG(adrbrowsiel_P3A_ENABLED)
  void SetP3AEnabled(const base::ListValue* args);
  void GetP3AEnabled(const base::ListValue* args);
  void OnP3AEnabledChanged();
#endif

  Profile* profile_ = nullptr;
  PrefChangeRegistrar local_state_change_registrar_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielPrivacyHandler);
};

#endif  // adrbrowsiel_BROWSER_UI_WEBUI_SETTINGS_adrbrowsiel_PRIVACY_HANDLER_H_
