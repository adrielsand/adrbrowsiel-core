/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_DECENTRALIZED_DNS_PREF_NAMES_H_
#define adrbrowsiel_COMPONENTS_DECENTRALIZED_DNS_PREF_NAMES_H_

namespace decentralized_dns {

// Used to determine which method should be used to resolve unstoppable
// domains, between:
// Disabled: Disable all unstoppable domains resolution.
// Ask: Ask users if they want to enable support of unstoppable domains.
// DNS Over HTTPS: Resolve domain name using a public DNS over HTTPS server.
constexpr char kUnstoppableDomainsResolveMethod[] =
    "adrbrowsiel.unstoppable_domains.resolve_method";

// Used to determine which method should be used to resolve ENS domains,
// between:
// Disabled: Disable all ENS domains resolution.
// Ask: Ask users if they want to enable support of ENS.
// DNS Over HTTPS: Resolve domain name using a public DNS over HTTPS server.
constexpr char kENSResolveMethod[] = "adrbrowsiel.ens.resolve_method";

}  // namespace decentralized_dns

#endif  // adrbrowsiel_COMPONENTS_DECENTRALIZED_DNS_PREF_NAMES_H_
