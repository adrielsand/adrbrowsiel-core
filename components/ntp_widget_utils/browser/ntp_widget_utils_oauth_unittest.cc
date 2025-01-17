/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/ntp_widget_utils/browser/ntp_widget_utils_oauth.h"

#include "chrome/test/base/chrome_render_view_host_test_harness.h"

// npm run test -- adrbrowsiel_unit_tests --filter=NTPWidgetUtilsOauthUtilTest.*

namespace {

typedef testing::Test NTPWidgetUtilsOauthUtilTest;

TEST_F(NTPWidgetUtilsOauthUtilTest, GetCodeChallengeStripChars) {
  std::string verifier =
      "FA87A1758E149A8BCD3A6D43DEAFAA013BCE2F132639ADA66C5BF101";
  ASSERT_EQ(
    "1vw-WOmdXSW7OHQPgnuMsZjhaQKxi3LO5L7uX0YEtHs",
    ntp_widget_utils::GetCodeChallenge(verifier, true));
}

TEST_F(NTPWidgetUtilsOauthUtilTest, GetCodeChallengeNoStripChars) {
  std::string verifier =
      "aGVsbG9fd29ybGRfdGhpc19pc19hX3Rlc3Q=";
  ASSERT_EQ(
    "mTWSN0meBbs9rauVM4rSmWDYVKTWFhkFeECqn6W2ZC0=",
    ntp_widget_utils::GetCodeChallenge(verifier, false));
}

}  // namespace
