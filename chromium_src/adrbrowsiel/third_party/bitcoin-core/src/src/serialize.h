/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_adrbrowsiel_THIRD_PARTY_BITCOIN_CORE_SRC_SRC_SERIALIZE_H_
#define adrbrowsiel_CHROMIUM_SRC_adrbrowsiel_THIRD_PARTY_BITCOIN_CORE_SRC_SRC_SERIALIZE_H_

#include <ios>
#include <string>

#include "base/check.h"

namespace std {
namespace adrbrowsiel {
using string = ::std::string;
}
}  // namespace std

#define throw CHECK(false) <<
#define ios_base adrbrowsiel
#define failure string
#include "../../../../../../../adrbrowsiel/third_party/bitcoin-core/src/src/serialize.h"
#undef throw
#undef ios_base
#undef string

#endif  // adrbrowsiel_CHROMIUM_SRC_adrbrowsiel_THIRD_PARTY_BITCOIN_CORE_SRC_SRC_SERIALIZE_H_
