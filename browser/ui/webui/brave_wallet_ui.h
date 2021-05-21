/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_WEBUI_adrbrowsiel_WALLET_UI_H_
#define adrbrowsiel_BROWSER_UI_WEBUI_adrbrowsiel_WALLET_UI_H_

#include <string>

#include "content/public/browser/web_ui_controller.h"

class adrbrowsielWalletUI : public content::WebUIController {
 public:
  adrbrowsielWalletUI(content::WebUI* web_ui, const std::string& host);
  ~adrbrowsielWalletUI() override;
  adrbrowsielWalletUI(const adrbrowsielWalletUI&) = delete;
  adrbrowsielWalletUI& operator=(const adrbrowsielWalletUI&) = delete;
};

#endif  // adrbrowsiel_BROWSER_UI_WEBUI_adrbrowsiel_WALLET_UI_H_
