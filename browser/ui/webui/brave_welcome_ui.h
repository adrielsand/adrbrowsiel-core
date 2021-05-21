/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_WEBUI_adrbrowsiel_WELCOME_UI_H_
#define adrbrowsiel_BROWSER_UI_WEBUI_adrbrowsiel_WELCOME_UI_H_

#include <string>

#include "content/public/browser/web_ui_controller.h"

class adrbrowsielWelcomeUI : public content::WebUIController {
 public:
  adrbrowsielWelcomeUI(content::WebUI* web_ui, const std::string& host);
  ~adrbrowsielWelcomeUI() override;
  adrbrowsielWelcomeUI(const adrbrowsielWelcomeUI&) = delete;
  adrbrowsielWelcomeUI& operator=(const adrbrowsielWelcomeUI&) = delete;
};

#endif  // adrbrowsiel_BROWSER_UI_WEBUI_adrbrowsiel_WELCOME_UI_H_
