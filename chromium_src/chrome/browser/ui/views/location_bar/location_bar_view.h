/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_LOCATION_BAR_VIEW_H_
#define adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_LOCATION_BAR_VIEW_H_

#define adrbrowsiel_LOCATION_BAR_VIEW_H_   \
 private:                            \
  friend class adrbrowsielLocationBarView; \
                                     \
 public:                             \
  virtual std::vector<views::View*> GetTrailingViews();

#define GetBorderRadius virtual GetBorderRadius
#include "../../../../../../../chrome/browser/ui/views/location_bar/location_bar_view.h"
#undef GetBorderRadius
#undef adrbrowsiel_LOCATION_BAR_VIEW_H_

#endif  // adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_LOCATION_BAR_VIEW_H_
