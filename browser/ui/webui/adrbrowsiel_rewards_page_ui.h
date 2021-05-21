/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_WEBUI_adrbrowsiel_REWARDS_PAGE_UI_H_
#define adrbrowsiel_BROWSER_UI_WEBUI_adrbrowsiel_REWARDS_PAGE_UI_H_

#include <string>

#include "content/public/browser/web_ui_controller.h"

class adrbrowsielRewardsPageUI : public content::WebUIController {
 public:
  adrbrowsielRewardsPageUI(content::WebUI* web_ui, const std::string& host);
  ~adrbrowsielRewardsPageUI() override;
  adrbrowsielRewardsPageUI(const adrbrowsielRewardsPageUI&) = delete;
  adrbrowsielRewardsPageUI& operator=(const adrbrowsielRewardsPageUI&) = delete;
};

#endif  // adrbrowsiel_BROWSER_UI_WEBUI_adrbrowsiel_REWARDS_PAGE_UI_H_
