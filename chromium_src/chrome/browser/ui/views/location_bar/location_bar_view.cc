/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/omnibox/adrbrowsiel_omnibox_client_impl.h"

#define adrbrowsiel_LAYOUT_TRAILING_DECORATIONS                                \
  auto right_most = GetTrailingViews();                                  \
  for (auto it = right_most.rbegin(); it != right_most.rend(); it++) {   \
    if ((*it)->GetVisible())                                             \
      trailing_decorations.AddDecoration(0, height(), false, 0, 0, *it); \
  }

#define ChromeOmniboxClient adrbrowsielOmniboxClientImpl
#include "../../../../../../../chrome/browser/ui/views/location_bar/location_bar_view.cc"
#undef ChromeOmniboxClient
#undef adrbrowsiel_LAYOUT_TRAILING_DECORATIONS

std::vector<views::View*> LocationBarView::GetTrailingViews() {
  return std::vector<views::View*>();
}
