/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_THEMES_THEME_SERVICE_AURA_LINUX_H_
#define adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_THEMES_THEME_SERVICE_AURA_LINUX_H_

#include "adrbrowsiel/browser/themes/adrbrowsiel_theme_service.h"

#undef ThemeService
#define ThemeService adrbrowsielThemeService
#include "../../../../../chrome/browser/themes/theme_service_aura_linux.h"
#undef ThemeService

#endif  // adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_THEMES_THEME_SERVICE_AURA_LINUX_H_
