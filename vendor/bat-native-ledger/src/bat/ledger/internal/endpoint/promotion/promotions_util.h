/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsielLEDGER_ENDPOINT_PROMOTION_PROMOTION_UTIL_H_
#define adrbrowsielLEDGER_ENDPOINT_PROMOTION_PROMOTION_UTIL_H_

#include <string>

namespace ledger {
namespace endpoint {
namespace promotion {

std::string GetServerUrl(const std::string& path);

}  // namespace promotion
}  // namespace endpoint
}  // namespace ledger

#endif  // adrbrowsielLEDGER_ENDPOINT_PROMOTION_PROMOTION_UTIL_H_
