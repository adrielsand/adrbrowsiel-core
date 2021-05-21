/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_THEMES_THEME_HELPER_H_
#define adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_THEMES_THEME_HELPER_H_

#define adrbrowsiel_THEME_HELPER_H_    \
 private:                        \
  friend class adrbrowsielThemeHelper; \
                                 \
 public:
#define GetOmniboxColor virtual GetOmniboxColor
#include "../../../../../chrome/browser/themes/theme_helper.h"
#undef GetOmniboxColor
#undef adrbrowsiel_THEME_HELPER_H_

#endif  // adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_THEMES_THEME_HELPER_H_
