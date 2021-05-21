// Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "adrbrowsiel/browser/ui/webui/new_tab_page/adrbrowsiel_new_tab_ui.h"

#include <string>

#include "adrbrowsiel/browser/new_tab/new_tab_shows_options.h"
#include "adrbrowsiel/browser/ui/webui/adrbrowsiel_webui_source.h"
#include "adrbrowsiel/browser/ui/webui/new_tab_page/adrbrowsiel_new_tab_message_handler.h"
#include "adrbrowsiel/browser/ui/webui/new_tab_page/instant_service_message_handler.h"
#include "adrbrowsiel/components/adrbrowsiel_new_tab/resources/grit/adrbrowsiel_new_tab_generated_map.h"
#include "chrome/browser/profiles/profile.h"
#include "components/grit/adrbrowsiel_components_resources.h"
#include "components/strings/grit/components_strings.h"
#include "content/public/browser/web_ui_data_source.h"
#include "ui/base/l10n/l10n_util.h"

adrbrowsielNewTabUI::adrbrowsielNewTabUI(content::WebUI* web_ui, const std::string& name)
    : WebUIController(web_ui) {
  Profile* profile = Profile::FromWebUI(web_ui);

  if (adrbrowsiel::ShouldNewTabShowBlankpage(profile)) {
    content::WebUIDataSource* source =
        content::WebUIDataSource::Create(name);
    source->SetDefaultResource(IDR_adrbrowsiel_BLANK_NEW_TAB_HTML);
    content::WebUIDataSource::Add(profile, source);
  } else {
    content::WebUIDataSource* source = CreateAndAddWebUIDataSource(
        web_ui, name, kadrbrowsielNewTabGenerated, kadrbrowsielNewTabGeneratedSize,
        IDR_adrbrowsiel_NEW_TAB_HTML);
    web_ui->AddMessageHandler(
        base::WrapUnique(adrbrowsielNewTabMessageHandler::Create(source, profile)));
    web_ui->AddMessageHandler(
        base::WrapUnique(new InstantServiceMessageHandler(profile)));
  }

  web_ui->OverrideTitle(l10n_util::GetStringUTF16(IDS_NEW_TAB_TITLE));
}

adrbrowsielNewTabUI::~adrbrowsielNewTabUI() {
}
