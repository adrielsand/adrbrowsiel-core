/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_DECENTRALIZED_DNS_FEATURES_H_
#define adrbrowsiel_COMPONENTS_DECENTRALIZED_DNS_FEATURES_H_

#include "base/feature_list.h"

namespace decentralized_dns {
namespace features {

constexpr base::Feature kDecentralizedDns{"DecentralizedDns",
                                          base::FEATURE_ENABLED_BY_DEFAULT};

}  // namespace features
}  // namespace decentralized_dns

#endif  // adrbrowsiel_COMPONENTS_DECENTRALIZED_DNS_FEATURES_H_
