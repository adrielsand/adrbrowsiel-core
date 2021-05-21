/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/webui/adrbrowsiel_wallet_ui.h"

#include <memory>

#include "adrbrowsiel/browser/ui/webui/adrbrowsiel_webui_source.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet_page/resources/grit/adrbrowsiel_wallet_page_generated_map.h"
#include "components/grit/adrbrowsiel_components_resources.h"

adrbrowsielWalletUI::adrbrowsielWalletUI(content::WebUI* web_ui, const std::string& name)
    : WebUIController(web_ui) {
  CreateAndAddWebUIDataSource(
      web_ui, name, kadrbrowsielWalletPageGenerated, kadrbrowsielWalletPageGeneratedSize,
      IDR_adrbrowsiel_WALLET_HTML, true /*disable_trusted_types_csp*/);
}

adrbrowsielWalletUI::~adrbrowsielWalletUI() = default;
