/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/themes/adrbrowsiel_dark_mode_utils.h"

#include "adrbrowsiel/browser/themes/adrbrowsiel_dark_mode_utils_internal.h"
#include "ui/native_theme/native_theme_win.h"

namespace ui {
// This resets dark mode to os theme when user changes adrbrowsiel theme from
// dark or light to Same as Windows by using the value of registry because
// we changed ui::NativeTheme::dark_mode_ explicitly for using adrbrowsiel theme
// like a system theme.
void UpdateDarkModeStatus() {
  static_cast<ui::NativeThemeWin*>(
      ui::NativeTheme::GetInstanceForNativeUi())->UpdateDarkModeStatus();
}
}  // namespace ui

namespace dark_mode {

void SetSystemDarkMode(adrbrowsielDarkModeType type) {
  // On Windows, we should block os theme change notification if user doesn't
  // choose same as Windows option. Windows doesn't support per-application
  // theme mode. If users stick to dark or light, we should prevent it.
  // On macOS, this isn't needed because it supports per-application theme.
  // So, if application set dark/light appearance explicitely, macOS respect it.
  ui::IgnoreSystemDarkModeChange(
      type != dark_mode::adrbrowsielDarkModeType::adrbrowsiel_DARK_MODE_TYPE_DEFAULT);
  // Reset and follow os dark mode.
  if (type == adrbrowsielDarkModeType::adrbrowsiel_DARK_MODE_TYPE_DEFAULT) {
    DCHECK(
        ui::NativeTheme::GetInstanceForNativeUi()->SystemDarkModeSupported());
    // This sets preferred color scheme on its own.
    ui::UpdateDarkModeStatus();
    return;
  }

  internal::SetSystemDarkModeForNonDefaultMode(
      type == adrbrowsielDarkModeType::adrbrowsiel_DARK_MODE_TYPE_DARK);
}

}  // namespace dark_mode
