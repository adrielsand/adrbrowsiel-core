// Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef adrbrowsiel_BROWSER_UI_WEBUI_NEW_TAB_PAGE_adrbrowsiel_NEW_TAB_UI_H_
#define adrbrowsiel_BROWSER_UI_WEBUI_NEW_TAB_PAGE_adrbrowsiel_NEW_TAB_UI_H_

#include <string>

#include "content/public/browser/web_ui_controller.h"

class adrbrowsielNewTabUI : public content::WebUIController {
 public:
  adrbrowsielNewTabUI(content::WebUI* web_ui, const std::string& name);
  ~adrbrowsielNewTabUI() override;
  adrbrowsielNewTabUI(const adrbrowsielNewTabUI&) = delete;
  adrbrowsielNewTabUI& operator=(const adrbrowsielNewTabUI&) = delete;
};

#endif  // adrbrowsiel_BROWSER_UI_WEBUI_NEW_TAB_PAGE_adrbrowsiel_NEW_TAB_UI_H_
