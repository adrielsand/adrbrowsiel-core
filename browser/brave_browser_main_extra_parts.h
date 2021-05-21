/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_adrbrowsiel_BROWSER_MAIN_EXTRA_PARTS_H_
#define adrbrowsiel_BROWSER_adrbrowsiel_BROWSER_MAIN_EXTRA_PARTS_H_

#include "base/compiler_specific.h"
#include "base/macros.h"
#include "chrome/browser/chrome_browser_main.h"
#include "chrome/browser/chrome_browser_main_extra_parts.h"

class adrbrowsielBrowserMainExtraParts : public ChromeBrowserMainExtraParts {
 public:
  adrbrowsielBrowserMainExtraParts();
  ~adrbrowsielBrowserMainExtraParts() override;

  // ChromeBrowserMainExtraParts overrides.
  void PostBrowserStart() override;
  void PreMainMessageLoopRun() override;

 private:
  DISALLOW_COPY_AND_ASSIGN(adrbrowsielBrowserMainExtraParts);
};

#endif  // adrbrowsiel_BROWSER_adrbrowsiel_BROWSER_MAIN_EXTRA_PARTS_H_
