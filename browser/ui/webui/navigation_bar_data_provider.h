// Copyright (c) 2019 The adrbrowsiel Authors
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef adrbrowsiel_BROWSER_UI_WEBUI_NAVIGATION_BAR_DATA_PROVIDER_H_
#define adrbrowsiel_BROWSER_UI_WEBUI_NAVIGATION_BAR_DATA_PROVIDER_H_

namespace content {
class WebUIDataSource;
}

class NavigationBarDataProvider {
 public:
  // Sets load-time constants on |source|. This handles a flicker-free initial
  // page load (i.e. loadTimeData.getString('brToolbarSettingsTitle')).
  static void Initialize(content::WebUIDataSource* source);
};

#endif  // adrbrowsiel_BROWSER_UI_WEBUI_NAVIGATION_BAR_DATA_PROVIDER_H_
