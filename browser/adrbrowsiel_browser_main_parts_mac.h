/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_adrbrowsiel_BROWSER_MAIN_PARTS_MAC_H_
#define adrbrowsiel_BROWSER_adrbrowsiel_BROWSER_MAIN_PARTS_MAC_H_

#include "chrome/browser/chrome_browser_main_mac.h"

class adrbrowsielBrowserMainPartsMac : public ChromeBrowserMainPartsMac {
 public:
  using ChromeBrowserMainPartsMac::ChromeBrowserMainPartsMac;
  ~adrbrowsielBrowserMainPartsMac() override = default;

 private:
  // ChromeBrowserMainPartsMac overrides:
  void PreMainMessageLoopStart() override;
};

#endif  // adrbrowsiel_BROWSER_adrbrowsiel_BROWSER_MAIN_PARTS_MAC_H_
