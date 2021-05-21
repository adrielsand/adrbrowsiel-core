/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_BROWSER_ETH_RESPONSE_PARSER_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_BROWSER_ETH_RESPONSE_PARSER_H_

#include <string>
#include "base/values.h"

namespace adrbrowsiel_wallet {

// Returns the balance of the account of given address.
bool ParseEthGetBalance(const std::string& json, std::string* hex_balance);
bool ParseEthCall(const std::string& json, std::string* result);

}  // namespace adrbrowsiel_wallet

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_BROWSER_ETH_RESPONSE_PARSER_H_
