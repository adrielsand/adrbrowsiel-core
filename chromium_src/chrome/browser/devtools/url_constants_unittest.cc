/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ui/webui/devtools_ui.h"
#include "testing/gtest/include/gtest/gtest.h"

const char kadrbrowsielDevtoolsURLHost[] = "devtools.adrbrowsiel.com";

TEST(adrbrowsielDevtoolsURLConstants, TestDevtoolsURL) {
  EXPECT_EQ(DevToolsUI::GetRemoteBaseURL().host(), kadrbrowsielDevtoolsURLHost);
}
