/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/views/tabs/adrbrowsiel_browser_tab_strip_controller.h"
#include "adrbrowsiel/browser/ui/views/tabs/adrbrowsiel_tab_strip.h"
#include "adrbrowsiel/browser/ui/views/toolbar/adrbrowsiel_toolbar_view.h"
#include "chrome/browser/ui/views/frame/tab_strip_region_view.h"

#define ToolbarView adrbrowsielToolbarView
#define BrowserTabStripController adrbrowsielBrowserTabStripController
#define TabStrip adrbrowsielTabStrip
#include "../../../../../../../chrome/browser/ui/views/frame/browser_view.cc"  // NOLINT
#undef ToolbarView
#undef BrowserTabStripController
#undef TabStrip
