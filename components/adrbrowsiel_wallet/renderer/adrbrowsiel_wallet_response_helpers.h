/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_RENDERER_adrbrowsiel_WALLET_RESPONSE_HELPERS_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_RENDERER_adrbrowsiel_WALLET_RESPONSE_HELPERS_H_

#include <memory>
#include <string>

#include "base/values.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/web3_provider_constants.h"

namespace adrbrowsiel_wallet {

std::unique_ptr<base::Value> FormProviderResponse(ProviderErrors code,
                                                  const std::string& message);
std::unique_ptr<base::Value> FormProviderResponse(
    const std::string& controller_response,
    bool* reject);
std::string FormProviderErrorResponse(const std::string& controller_response);

}  // namespace adrbrowsiel_wallet

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_RENDERER_adrbrowsiel_WALLET_RESPONSE_HELPERS_H_
