/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/values.h"
#include "extensions/common/constants.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace adrbrowsiel {
bool adrbrowsielRewriteManifest(const std::string& extension_id,
    base::DictionaryValue* manifest);
}

TEST(adrbrowsielSandboxedUnpacker, DoesNotRewriteForDifferentExtension) {
  base::DictionaryValue manifest;
  ASSERT_FALSE(
      adrbrowsiel::adrbrowsielRewriteManifest("fffkpbalmllkdoenhmdmpbkajipdjfam",
      &manifest));
}

TEST(adrbrowsielSandboxedUnpacker, RewriteForIPFSEExtension) {
  // I would have liked to use SandboxedUnpacker here directly instead of
  // adrbrowsiel::adrbrowsielRewriteManifest instead. But it would have required
  // more patching for making that function protected or else more
  // time than I am willing to spend otherwise.
  base::DictionaryValue manifest1;
  ASSERT_TRUE(
      adrbrowsiel::adrbrowsielRewriteManifest(ipfs_companion_extension_id,
      &manifest1));
  ASSERT_TRUE(manifest1.FindKey("sockets"));
  base::DictionaryValue manifest2;
  ASSERT_TRUE(
      adrbrowsiel::adrbrowsielRewriteManifest(ipfs_companion_beta_extension_id,
      &manifest2));
  ASSERT_TRUE(manifest2.FindKey("sockets"));
}
