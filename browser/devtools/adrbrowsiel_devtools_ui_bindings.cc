/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/devtools/adrbrowsiel_devtools_ui_bindings.h"

#include <string>
#include <utility>

#include "base/values.h"
#include "adrbrowsiel/browser/themes/adrbrowsiel_dark_mode_utils.h"
#include "chrome/common/pref_names.h"
#include "components/prefs/pref_service.h"

namespace {
std::string GetDevToolsUIThemeValue() {
  dark_mode::adrbrowsielDarkModeType theme_type =
      dark_mode::GetActiveadrbrowsielDarkModeType();
  // In devtools' theme, default is translated to light.
  return theme_type == dark_mode::adrbrowsielDarkModeType::adrbrowsiel_DARK_MODE_TYPE_DARK ?
      "\"dark\"" : "\"default\"";
}
}

void adrbrowsielDevToolsUIBindings::GetPreferences(DispatchCallback callback) {
  const base::DictionaryValue* prefs =
      profile_->GetPrefs()->GetDictionary(prefs::kDevToolsPreferences);

  if (prefs->FindKey("uiTheme"))
    return DevToolsUIBindings::GetPreferences(std::move(callback));

  base::Value new_prefs(prefs->Clone());
  new_prefs.SetKey("uiTheme", base::Value(GetDevToolsUIThemeValue()));
  std::move(callback).Run(&new_prefs);
}
