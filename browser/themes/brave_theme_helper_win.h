/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_THEMES_adrbrowsiel_THEME_HELPER_WIN_H_
#define adrbrowsiel_BROWSER_THEMES_adrbrowsiel_THEME_HELPER_WIN_H_

#include "chrome/browser/themes/theme_helper_win.h"

class adrbrowsielThemeHelperWin : public ThemeHelperWin {
 public:
  adrbrowsielThemeHelperWin() = default;
  ~adrbrowsielThemeHelperWin() override = default;

 private:
  // ThemeHelperWin overrides:
  SkColor GetDefaultColor(
      int id,
      bool incognito,
      const CustomThemeSupplier* theme_supplier) const override;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielThemeHelperWin);
};

#endif  // adrbrowsiel_BROWSER_THEMES_adrbrowsiel_THEME_HELPER_WIN_H_
