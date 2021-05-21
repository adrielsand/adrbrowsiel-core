/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_BROWSER_RLP_ENCODE_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_BROWSER_RLP_ENCODE_H_

#include <string>

#include "base/values.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/adrbrowsiel_wallet_types.h"

namespace adrbrowsiel_wallet {

// Converts a uint256_t value into a blob value type
base::Value RLPUint256ToBlobValue(uint256_t input);

// Recursive Length Prefix (RLP) encoding of base::Values consisting of string,
// blob, or int data
std::string RLPEncode(base::Value val);

}  // namespace adrbrowsiel_wallet

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_BROWSER_RLP_ENCODE_H_
