/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/webui/settings/adrbrowsiel_appearance_handler.h"

#include "base/bind.h"
#include "base/metrics/histogram_macros.h"
#include "base/strings/string_number_conversions.h"
#include "adrbrowsiel/browser/new_tab/new_tab_shows_options.h"
#include "adrbrowsiel/browser/profiles/profile_util.h"
#include "adrbrowsiel/browser/themes/adrbrowsiel_dark_mode_utils.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/ntp_background_images/common/pref_names.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/search/instant_service.h"
#include "chrome/browser/search/instant_service_factory.h"
#include "chrome/common/pref_names.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_ui.h"


adrbrowsielAppearanceHandler::adrbrowsielAppearanceHandler() {
  local_state_change_registrar_.Init(g_browser_process->local_state());
  local_state_change_registrar_.Add(
      kadrbrowsielDarkMode,
      base::Bind(&adrbrowsielAppearanceHandler::OnadrbrowsielDarkModeChanged,
                 base::Unretained(this)));
}

adrbrowsielAppearanceHandler::~adrbrowsielAppearanceHandler() = default;

// TODO(simonhong): Use separate handler for NTP settings.
void adrbrowsielAppearanceHandler::RegisterMessages() {
  profile_ = Profile::FromWebUI(web_ui());
  profile_state_change_registrar_.Init(profile_->GetPrefs());
  profile_state_change_registrar_.Add(
      kNewTabPageShowsOptions,
      base::BindRepeating(&adrbrowsielAppearanceHandler::OnPreferenceChanged,
      base::Unretained(this)));
  profile_state_change_registrar_.Add(
      prefs::kHomePageIsNewTabPage,
      base::BindRepeating(&adrbrowsielAppearanceHandler::OnPreferenceChanged,
      base::Unretained(this)));
  profile_state_change_registrar_.Add(
      prefs::kHomePage,
      base::BindRepeating(&adrbrowsielAppearanceHandler::OnPreferenceChanged,
      base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setadrbrowsielThemeType",
      base::BindRepeating(&adrbrowsielAppearanceHandler::SetadrbrowsielThemeType,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getadrbrowsielThemeType",
      base::BindRepeating(&adrbrowsielAppearanceHandler::GetadrbrowsielThemeType,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getNewTabShowsOptionsList",
      base::BindRepeating(&adrbrowsielAppearanceHandler::GetNewTabShowsOptionsList,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "shouldShowNewTabDashboardSettings",
      base::BindRepeating(
          &adrbrowsielAppearanceHandler::ShouldShowNewTabDashboardSettings,
          base::Unretained(this)));
}

void adrbrowsielAppearanceHandler::SetadrbrowsielThemeType(const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  AllowJavascript();

  int int_type;
  args->GetInteger(0, &int_type);
  dark_mode::SetadrbrowsielDarkModeType(
      static_cast<dark_mode::adrbrowsielDarkModeType>(int_type));
}

void adrbrowsielAppearanceHandler::GetadrbrowsielThemeType(const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);

  AllowJavascript();
  // GetadrbrowsielThemeType() should be used because settings option displays all
  // available options including default.
  ResolveJavascriptCallback(
      args->GetList()[0],
      base::Value(static_cast<int>(dark_mode::GetadrbrowsielDarkModeType())));
}

void adrbrowsielAppearanceHandler::OnadrbrowsielDarkModeChanged() {
  // GetadrbrowsielThemeType() should be used because settings option displays all
  // available options including default.
  if (IsJavascriptAllowed()) {
    FireWebUIListener(
        "adrbrowsiel-theme-type-changed",
        base::Value(static_cast<int>(dark_mode::GetadrbrowsielDarkModeType())));
  }
}

void adrbrowsielAppearanceHandler::OnBackgroundPreferenceChanged(
    const std::string& pref_name) {
  adrbrowsiel::RecordSponsoredImagesEnabledP3A(profile_);
}

void adrbrowsielAppearanceHandler::OnPreferenceChanged(const std::string& pref_name) {
  if (IsJavascriptAllowed()) {
    if (pref_name == kNewTabPageShowsOptions ||
        pref_name == prefs::kHomePage ||
        pref_name == prefs::kHomePageIsNewTabPage) {
      FireWebUIListener(
          "show-new-tab-dashboard-settings-changed",
          base::Value(adrbrowsiel::ShouldNewTabShowDashboard(profile_)));
      return;
    }
  }
}

void adrbrowsielAppearanceHandler::GetNewTabShowsOptionsList(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(args->GetList()[0],
                            adrbrowsiel::GetNewTabShowsOptionsList(profile_));
}

void adrbrowsielAppearanceHandler::ShouldShowNewTabDashboardSettings(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(
      args->GetList()[0],
      base::Value(adrbrowsiel::ShouldNewTabShowDashboard(profile_)));
}
