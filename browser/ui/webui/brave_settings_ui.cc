/* Copyright (c) 2019 The adrbrowsiel Authors
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/webui/adrbrowsiel_settings_ui.h"

#include <string>

#include "adrbrowsiel/browser/ntp_background_images/view_counter_service_factory.h"
#include "adrbrowsiel/browser/resources/settings/grit/adrbrowsiel_settings_resources.h"
#include "adrbrowsiel/browser/resources/settings/grit/adrbrowsiel_settings_resources_map.h"
#include "adrbrowsiel/browser/sparkle_buildflags.h"
#include "adrbrowsiel/browser/ui/webui/navigation_bar_data_provider.h"
#include "adrbrowsiel/browser/ui/webui/settings/adrbrowsiel_appearance_handler.h"
#include "adrbrowsiel/browser/ui/webui/settings/adrbrowsiel_default_extensions_handler.h"
#include "adrbrowsiel/browser/ui/webui/settings/adrbrowsiel_privacy_handler.h"
#include "adrbrowsiel/browser/ui/webui/settings/adrbrowsiel_sync_handler.h"
#include "adrbrowsiel/browser/ui/webui/settings/default_adrbrowsiel_shields_handler.h"
#include "adrbrowsiel/components/adrbrowsiel_sync/buildflags/buildflags.h"
#include "adrbrowsiel/components/ntp_background_images/browser/view_counter_service.h"
#include "adrbrowsiel/components/sidebar/buildflags/buildflags.h"
#include "adrbrowsiel/components/version_info/version_info.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/webui/settings/metrics_reporting_handler.h"
#include "content/public/browser/web_ui_data_source.h"

#if BUILDFLAG(ENABLE_adrbrowsiel_SYNC)
#include "components/sync/driver/sync_driver_switches.h"
#endif

#if BUILDFLAG(ENABLE_SPARKLE)
#include "adrbrowsiel/browser/ui/webui/settings/adrbrowsiel_relaunch_handler_mac.h"
#endif

#if BUILDFLAG(ENABLE_SIDEBAR)
#include "adrbrowsiel/browser/ui/sidebar/sidebar_utils.h"
#endif

using ntp_background_images::ViewCounterServiceFactory;

adrbrowsielSettingsUI::adrbrowsielSettingsUI(content::WebUI* web_ui,
                                 const std::string& host)
    : SettingsUI(web_ui) {
  web_ui->AddMessageHandler(
    std::make_unique<settings::MetricsReportingHandler>());
  web_ui->AddMessageHandler(std::make_unique<adrbrowsielPrivacyHandler>());
  web_ui->AddMessageHandler(std::make_unique<DefaultadrbrowsielShieldsHandler>());
  web_ui->AddMessageHandler(std::make_unique<adrbrowsielDefaultExtensionsHandler>());
  web_ui->AddMessageHandler(std::make_unique<adrbrowsielAppearanceHandler>());
  web_ui->AddMessageHandler(std::make_unique<adrbrowsielSyncHandler>());
#if BUILDFLAG(ENABLE_SPARKLE)
  // Use sparkle's relaunch api for browser relaunch on update.
  web_ui->AddMessageHandler(std::make_unique<adrbrowsielRelaunchHandler>());
#endif
}

adrbrowsielSettingsUI::~adrbrowsielSettingsUI() {
}

// static
void adrbrowsielSettingsUI::AddResources(content::WebUIDataSource* html_source,
                                   Profile* profile) {
  for (size_t i = 0; i < kadrbrowsielSettingsResourcesSize; ++i) {
    html_source->AddResourcePath(kadrbrowsielSettingsResources[i].path,
                                 kadrbrowsielSettingsResources[i].id);
  }

#if BUILDFLAG(ENABLE_adrbrowsiel_SYNC)
  html_source->AddBoolean("isSyncDisabled",
                          !switches::IsSyncAllowedByFlag());
#else
  html_source->AddBoolean("isSyncDisabled", true);
#endif
  html_source->AddString("adrbrowsielProductVersion",
    version_info::GetadrbrowsielVersionWithoutChromiumMajorVersion());
  NavigationBarDataProvider::Initialize(html_source);
  if (auto* service = ViewCounterServiceFactory::GetForProfile(profile))
    service->InitializeWebUIDataSource(html_source);
#if BUILDFLAG(ENABLE_SIDEBAR)
  // TODO(simonhong): Remove this when sidebar is shipped by default in all
  // channels.
  html_source->AddBoolean("isSidebarFeatureEnabled",
                          sidebar::CanUseSidebar(profile));
#endif
}
