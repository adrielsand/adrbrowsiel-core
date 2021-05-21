/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_CHROME_TEST_VIEWS_ACCESSIBILITY_CHECKER_H_
#define adrbrowsiel_CHROMIUM_SRC_CHROME_TEST_VIEWS_ACCESSIBILITY_CHECKER_H_

#include "chrome/browser/ui/views/chrome_views_delegate.h"

#if defined(OS_LINUX)
#include "adrbrowsiel/browser/ui/views/adrbrowsiel_views_delegate_linux.h"
#define ChromeViewsDelegate adrbrowsielViewsDelegateLinux
#endif
#include "../../../../../chrome/test/views/accessibility_checker.h"
#if defined(OS_LINUX)
#undef ChromeViewsDelegate
#endif

#endif  // adrbrowsiel_CHROMIUM_SRC_CHROME_TEST_VIEWS_ACCESSIBILITY_CHECKER_H_
