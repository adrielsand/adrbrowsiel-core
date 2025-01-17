/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "testing/gtest/include/gtest/gtest.h"

namespace base {

TEST(ToolsSanityTest, DISABLED_AddressSanitizerLocalOOBCrashTest) {
  // Intentionally crash to make sure AddressSanitizer is instrumenting
  // the local variables.
  int array[5];
  int* volatile access = &array[5];
  *access = 43;
}

}  // namespace base
