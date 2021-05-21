/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/adrbrowsiel_browser.h"
#include "adrbrowsiel/browser/ui/adrbrowsiel_browser_command_controller.h"
#include "adrbrowsiel/browser/ui/adrbrowsiel_browser_content_setting_bubble_model_delegate.h"
#include "adrbrowsiel/browser/ui/tabs/adrbrowsiel_tab_strip_model.h"
#include "adrbrowsiel/browser/ui/toolbar/adrbrowsiel_location_bar_model_delegate.h"
#include "adrbrowsiel/components/sidebar/buildflags/buildflags.h"
#include "chrome/browser/ui/browser_command_controller.h"
#include "chrome/browser/ui/browser_content_setting_bubble_model_delegate.h"

#if !defined(OS_ANDROID)
#include "adrbrowsiel/browser/ui/bookmark/adrbrowsiel_bookmark_tab_helper.h"
#endif

#if BUILDFLAG(ENABLE_SIDEBAR)
#include "base/feature_list.h"
#include "adrbrowsiel/components/sidebar/features.h"
#endif

#define adrbrowsiel_BROWSER_CREATE return new adrbrowsielBrowser(params);
#define BrowserContentSettingBubbleModelDelegate \
  adrbrowsielBrowserContentSettingBubbleModelDelegate
#define BrowserCommandController adrbrowsielBrowserCommandController
#define BrowserLocationBarModelDelegate adrbrowsielLocationBarModelDelegate
#if !defined(OS_ANDROID)
#define BookmarkTabHelper adrbrowsielBookmarkTabHelper
#endif

#include "../../../../../chrome/browser/ui/browser.cc"  // NOLINT
#undef BrowserLocationBarModelDelegate
#undef BrowserContentSettingBubbleModelDelegate
#undef BrowserCommandController
#undef adrbrowsiel_BROWSER_CREATE

#if !defined(OS_ANDROID)
#undef BookmarkTabHelper
#endif
