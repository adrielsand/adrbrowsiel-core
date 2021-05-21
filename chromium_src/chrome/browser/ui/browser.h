/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_BROWSER_H_
#define adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_BROWSER_H_

#define ScheduleUIUpdate virtual ScheduleUIUpdate
#define adrbrowsiel_BROWSER_H              \
 private:                            \
  friend class BookmarkPrefsService; \
  friend class adrbrowsielBrowser;

#include "../../../../../chrome/browser/ui/browser.h"  // NOLINT

#undef adrbrowsiel_BROWSER_H
#undef ScheduleUIUpdate

#endif  // adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_BROWSER_H_
