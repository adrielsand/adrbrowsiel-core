/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_adrbrowsiel_BROWSER_MAIN_PARTS_H_
#define adrbrowsiel_BROWSER_adrbrowsiel_BROWSER_MAIN_PARTS_H_

#include "base/macros.h"
#include "chrome/browser/chrome_browser_main.h"

class adrbrowsielBrowserMainParts : public ChromeBrowserMainParts {
 public:
  using ChromeBrowserMainParts::ChromeBrowserMainParts;
  ~adrbrowsielBrowserMainParts() override = default;

  void PostBrowserStart() override;
  void PreShutdown() override;
  void PreProfileInit() override;
  void PostProfileInit() override;

 private:
  friend class ChromeBrowserMainExtraPartsTor;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielBrowserMainParts);
};

#endif  // adrbrowsiel_BROWSER_adrbrowsiel_BROWSER_MAIN_PARTS_H_
