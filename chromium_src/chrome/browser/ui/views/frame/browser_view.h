/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_VIEW_H_
#define adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_VIEW_H_

#include "adrbrowsiel/browser/ui/adrbrowsiel_browser_window.h"

#define adrbrowsiel_BROWSER_VIEW_H friend class adrbrowsielBrowserView;
#define BrowserWindow adrbrowsielBrowserWindow
#define GetContentsLayoutManager     \
  GetContentsLayoutManager_Unused(); \
  virtual ContentsLayoutManager* GetContentsLayoutManager

#include "../../../../../../../chrome/browser/ui/views/frame/browser_view.h"
#undef adrbrowsiel_BROWSER_VIEW_H
#undef BrowserWindow
#undef GetContentsLayoutManager

#endif  // adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_VIEW_H_
