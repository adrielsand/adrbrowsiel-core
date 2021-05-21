/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <string>

#include "base/values.h"
#include "base/json/json_reader.h"
#include "extensions/common/constants.h"

namespace adrbrowsiel {

bool adrbrowsielRewriteManifest(const std::string& extension_id,
    base::DictionaryValue* manifest) {
  if (extension_id == ipfs_companion_extension_id ||
      extension_id == ipfs_companion_beta_extension_id) {
    // Set appropriate sockets permissions for IPFS Companion
    // extensions
    std::string json = R"(
      {
        "udp": {
          "send": "*",
          "bind": "*"
        },
        "tcp": {
          "connect": "*"
        },
        "tcpServer": {
          "listen": "*:*"
        }
      }
    )";

    base::Value sockets = base::JSONReader::Read(json).value();
    manifest->SetKey("sockets", std::move(sockets));
    return true;
  }
  return false;
}

}  // namespace adrbrowsiel

#define adrbrowsiel_SANDBOXEDUNPACKER_REWRITEMANIFESTFILE \
  base::DictionaryValue* dict_manifest;             \
  final_manifest.GetAsDictionary(&dict_manifest);   \
  adrbrowsiel::adrbrowsielRewriteManifest(extension_id_, dict_manifest);

#include "../../../../extensions/browser/sandboxed_unpacker.cc"
#undef adrbrowsiel_SANDBOXEDUNPACKER_REWRITEMANIFESTFILE
