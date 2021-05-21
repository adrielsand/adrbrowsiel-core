/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/eth_call_data_builder.h"

#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/adrbrowsiel_wallet_utils.h"

namespace adrbrowsiel_wallet {

namespace erc20 {

bool BalanceOf(const std::string& address, std::string* data) {
  const std::string function_hash = GetFunctionHash("balanceOf(address)");
  std::string params;
  if (!adrbrowsiel_wallet::PadHexEncodedParameter(address, &params)) {
    return false;
  }
  return adrbrowsiel_wallet::ConcatHexStrings(function_hash, params, data);
}

}  // namespace erc20

namespace unstoppable_domains {

bool GetMany(const std::vector<std::string>& keys,
             const std::string& domain,
             std::string* data) {
  const std::string function_hash =
      GetFunctionHash("getMany(string[],uint256)");

  std::string offset_for_array;
  if (!PadHexEncodedParameter(Uint256ValueToHex(64), &offset_for_array)) {
    return false;
  }

  std::string tokenID = Namehash(domain);

  std::string encoded_keys;
  if (!EncodeStringArray(keys, &encoded_keys)) {
    return false;
  }

  std::vector<std::string> hex_strings = {function_hash, offset_for_array,
                                          tokenID, encoded_keys};
  if (!ConcatHexStrings(hex_strings, data)) {
    return false;
  }

  return true;
}

}  // namespace unstoppable_domains

}  // namespace adrbrowsiel_wallet
