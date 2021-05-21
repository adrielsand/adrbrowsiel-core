/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_THEMES_THEME_HELPER_WIN_H_
#define adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_THEMES_THEME_HELPER_WIN_H_

#include "adrbrowsiel/browser/themes/adrbrowsiel_theme_helper.h"

#undef ThemeHelper
#define ThemeHelper adrbrowsielThemeHelper

#define adrbrowsiel_THEME_HELPER_WIN_H_   \
 private:                           \
  friend class adrbrowsielThemeHelperWin; \
                                    \
 public:
#include "../../../../../chrome/browser/themes/theme_helper_win.h"

#undef adrbrowsiel_THEME_HELPER_WIN_H_
#undef ThemeHelper
#define ThemeHelper ThemeHelper

#endif  // adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_THEMES_THEME_HELPER_WIN_H_
