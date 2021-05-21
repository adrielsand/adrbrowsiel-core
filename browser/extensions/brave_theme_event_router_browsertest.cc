/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/extensions/adrbrowsiel_theme_event_router.h"
#include "adrbrowsiel/browser/themes/adrbrowsiel_dark_mode_utils.h"
#include "adrbrowsiel/browser/themes/adrbrowsiel_theme_service.h"
#include "adrbrowsiel/common/pref_names.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/themes/theme_service_factory.h"
#include "chrome/browser/ui/browser.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_test.h"
#include "testing/gmock/include/gmock/gmock.h"

using adrbrowsielThemeEventRouterBrowserTest = InProcessBrowserTest;

namespace extensions {

class MockadrbrowsielThemeEventRouter : public adrbrowsielThemeEventRouter {
 public:
  using adrbrowsielThemeEventRouter::adrbrowsielThemeEventRouter;
  ~MockadrbrowsielThemeEventRouter() override {}

  MOCK_METHOD0(Notify, void());
};

}  // namespace extensions

IN_PROC_BROWSER_TEST_F(adrbrowsielThemeEventRouterBrowserTest,
                       ThemeChangeTest) {
  dark_mode::SetadrbrowsielDarkModeType(
      dark_mode::adrbrowsielDarkModeType::adrbrowsiel_DARK_MODE_TYPE_DARK);

  extensions::MockadrbrowsielThemeEventRouter* mock_router =
      new extensions::MockadrbrowsielThemeEventRouter(browser()->profile());
  adrbrowsielThemeService* service = static_cast<adrbrowsielThemeService*>(
      ThemeServiceFactory::GetForProfile(browser()->profile()));
  service->SetadrbrowsielThemeEventRouterForTesting(mock_router);

  EXPECT_CALL(*mock_router, Notify()).Times(1);
  dark_mode::SetadrbrowsielDarkModeType(
      dark_mode::adrbrowsielDarkModeType::adrbrowsiel_DARK_MODE_TYPE_LIGHT);

  EXPECT_CALL(*mock_router, Notify()).Times(1);
  dark_mode::SetadrbrowsielDarkModeType(
      dark_mode::adrbrowsielDarkModeType::adrbrowsiel_DARK_MODE_TYPE_DARK);

  EXPECT_CALL(*mock_router, Notify()).Times(0);
  dark_mode::SetadrbrowsielDarkModeType(
      dark_mode::adrbrowsielDarkModeType::adrbrowsiel_DARK_MODE_TYPE_DARK);
}
