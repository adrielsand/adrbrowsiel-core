/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_VIEWS_TABS_adrbrowsiel_TAB_STRIP_H_
#define adrbrowsiel_BROWSER_UI_VIEWS_TABS_adrbrowsiel_TAB_STRIP_H_

#include "chrome/browser/ui/views/tabs/tab_strip.h"

class adrbrowsielTabStrip : public TabStrip {
 public:
  using TabStrip::TabStrip;
  ~adrbrowsielTabStrip() override;
  adrbrowsielTabStrip(const adrbrowsielTabStrip&) = delete;
  adrbrowsielTabStrip& operator=(const adrbrowsielTabStrip&) = delete;

 private:
  // TabStrip overrides:
  bool ShouldHideCloseButtonForTab(Tab* tab) const override;
  SkColor GetTabSeparatorColor() const override;
};

#endif  // adrbrowsiel_BROWSER_UI_VIEWS_TABS_adrbrowsiel_TAB_STRIP_H_
