/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_WEBUI_adrbrowsiel_SETTINGS_UI_H_
#define adrbrowsiel_BROWSER_UI_WEBUI_adrbrowsiel_SETTINGS_UI_H_

#include <memory>
#include <string>

#include "chrome/browser/ui/webui/settings/settings_ui.h"

namespace content {
class WebUIDataSource;
}

class Profile;

class adrbrowsielSettingsUI : public settings::SettingsUI {
 public:
  adrbrowsielSettingsUI(content::WebUI* web_ui, const std::string& host);
  ~adrbrowsielSettingsUI() override;

  static void AddResources(content::WebUIDataSource* html_source,
                           Profile* profile);

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielSettingsUI);
};

#endif  // adrbrowsiel_BROWSER_UI_WEBUI_adrbrowsiel_SETTINGS_UI_H_
