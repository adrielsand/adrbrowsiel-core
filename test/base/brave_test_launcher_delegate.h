/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_TEST_BASE_adrbrowsiel_TEST_LAUNCHER_H_
#define adrbrowsiel_TEST_BASE_adrbrowsiel_TEST_LAUNCHER_H_

#include "chrome/test/base/chrome_test_launcher.h"
#include "base/macros.h"

class adrbrowsielTestLauncherDelegate : public ChromeTestLauncherDelegate {
 public:
  // Does not take ownership of ChromeTestSuiteRunner.
  explicit adrbrowsielTestLauncherDelegate(ChromeTestSuiteRunner* runner);
  ~adrbrowsielTestLauncherDelegate() override;

 private:
  // ChromeLauncherDelegate:
  content::ContentMainDelegate* CreateContentMainDelegate() override;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielTestLauncherDelegate);
};

#endif  // adrbrowsiel_TEST_BASE_adrbrowsiel_TEST_LAUNCHER_H_
