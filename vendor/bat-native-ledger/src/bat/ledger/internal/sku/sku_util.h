/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsielLEDGER_SKU_SKU_UTIL_H_
#define adrbrowsielLEDGER_SKU_SKU_UTIL_H_

#include <string>
#include <vector>

#include "bat/ledger/mojom_structs.h"

namespace ledger {
namespace sku {

std::string GetadrbrowsielDestination(const std::string& wallet_type);

std::string GetAnonCardDestination();

std::string GetUpholdDestination();

}  // namespace sku
}  // namespace ledger

#endif  // adrbrowsielLEDGER_SKU_SKU_UTIL_H_
