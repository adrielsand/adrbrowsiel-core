/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_adrbrowsiel_ADS_SPACER_VIEW_H_
#define adrbrowsiel_BROWSER_UI_adrbrowsiel_ADS_SPACER_VIEW_H_

namespace views {
class View;
}  // namespace views

namespace adrbrowsiel_ads {

views::View* CreateFlexibleSpacerView(const int spacing);
views::View* CreateFixedSizeSpacerView(const int spacing);

}  // namespace adrbrowsiel_ads

#endif  // adrbrowsiel_BROWSER_UI_adrbrowsiel_ADS_SPACER_VIEW_H_
