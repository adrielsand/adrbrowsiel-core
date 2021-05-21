/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_IPFS_ADDRESSES_CONFIG_H_
#define adrbrowsiel_COMPONENTS_IPFS_ADDRESSES_CONFIG_H_

#include <string>
#include <vector>

namespace ipfs {

struct AddressesConfig {
  AddressesConfig();
  ~AddressesConfig();

  std::string api;
  std::string gateway;
  std::vector<std::string> swarm;
};

}  // namespace ipfs

#endif  // adrbrowsiel_COMPONENTS_IPFS_ADDRESSES_CONFIG_H_
