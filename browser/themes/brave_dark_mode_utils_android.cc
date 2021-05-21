/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/themes/adrbrowsiel_dark_mode_utils.h"

namespace dark_mode {

void MigrateadrbrowsielDarkModePrefs(Profile* profile) {
}

void RegisteradrbrowsielDarkModeLocalStatePrefs(PrefRegistrySimple* registry) {
}

void RegisteradrbrowsielDarkModePrefsForMigration(
    user_prefs::PrefRegistrySyncable* registry) {
}

bool SystemDarkModeEnabled() {
  return false;
}

void SetUseSystemDarkModeEnabledForTest(bool enabled) {
}

std::string GetStringFromadrbrowsielDarkModeType(adrbrowsielDarkModeType type) {
  return "Default";
}

void SetadrbrowsielDarkModeType(const std::string& type) {
}

void SetadrbrowsielDarkModeType(adrbrowsielDarkModeType type) {
}

adrbrowsielDarkModeType GetActiveadrbrowsielDarkModeType() {
  return adrbrowsielDarkModeType::adrbrowsiel_DARK_MODE_TYPE_DEFAULT;
}

adrbrowsielDarkModeType GetadrbrowsielDarkModeType() {
  return adrbrowsielDarkModeType::adrbrowsiel_DARK_MODE_TYPE_DEFAULT;
}

base::Value GetadrbrowsielDarkModeTypeList() {
  return base::Value(base::Value::Type::LIST);
}

void SetSystemDarkMode(adrbrowsielDarkModeType type) {
}

}  // namespace dark_mode
