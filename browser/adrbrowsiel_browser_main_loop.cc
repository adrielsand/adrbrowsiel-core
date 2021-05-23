/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/adrbrowsiel_browser_main_loop.h"

#include "adrbrowsiel/browser/adrbrowsiel_browser_main_parts.h"

namespace adrbrowsiel {

void adrbrowsielBrowserMainLoop::PreShutdown() {
  parts()->PreShutdown();
  content::BrowserMainLoop::PreShutdown();
}

}  // namespace adrbrowsiel