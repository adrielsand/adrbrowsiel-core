/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_THEMES_adrbrowsiel_THEME_SERVICE_H_
#define adrbrowsiel_BROWSER_THEMES_adrbrowsiel_THEME_SERVICE_H_

#include <memory>

#include "chrome/browser/themes/theme_service.h"

namespace extensions {
class adrbrowsielThemeEventRouter;
}  // namespace extensions

class Profile;

class adrbrowsielThemeService : public ThemeService {
 public:
  explicit adrbrowsielThemeService(Profile* profile, const ThemeHelper& theme_helper);
  ~adrbrowsielThemeService() override;

 private:
  FRIEND_TEST_ALL_PREFIXES(adrbrowsielThemeEventRouterBrowserTest, ThemeChangeTest);

  // Own |mock_router|.
  void SetadrbrowsielThemeEventRouterForTesting(
      extensions::adrbrowsielThemeEventRouter* mock_router);

  std::unique_ptr<extensions::adrbrowsielThemeEventRouter> adrbrowsiel_theme_event_router_;
};

#endif  // adrbrowsiel_BROWSER_THEMES_adrbrowsiel_THEME_SERVICE_H_
