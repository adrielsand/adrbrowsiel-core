/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * you can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_VIEWS_TABS_adrbrowsiel_TAB_SEARCH_BUTTON_H_
#define adrbrowsiel_BROWSER_UI_VIEWS_TABS_adrbrowsiel_TAB_SEARCH_BUTTON_H_

#include "chrome/browser/ui/views/tabs/tab_search_button.h"

class adrbrowsielTabSearchButton : public TabSearchButton {
 public:
  using TabSearchButton::TabSearchButton;
  ~adrbrowsielTabSearchButton() override;
  adrbrowsielTabSearchButton(const adrbrowsielTabSearchButton&) = delete;
  adrbrowsielTabSearchButton& operator=(const adrbrowsielTabSearchButton&) = delete;

  // TabSearchButton overrides:
  int GetCornerRadius() const override;
};

#endif  // adrbrowsiel_BROWSER_UI_VIEWS_TABS_adrbrowsiel_TAB_SEARCH_BUTTON_H_
