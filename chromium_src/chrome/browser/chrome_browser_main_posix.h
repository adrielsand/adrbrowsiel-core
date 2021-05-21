/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_CHROME_BROWSER_MAIN_POSIX_H_
#define adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_CHROME_BROWSER_MAIN_POSIX_H_

#include "adrbrowsiel/browser/adrbrowsiel_browser_main_parts.h"
#include "chrome/browser/chrome_browser_main.h"

#define ChromeBrowserMainParts adrbrowsielBrowserMainParts
#include "../../../../chrome/browser/chrome_browser_main_posix.h"  // NOLINT
#undef ChromeBrowserMainParts

#endif  // adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_CHROME_BROWSER_MAIN_POSIX_H_
