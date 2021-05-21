/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_DECENTRALIZED_DNS_UTILS_H_
#define adrbrowsiel_COMPONENTS_DECENTRALIZED_DNS_UTILS_H_

class GURL;
class PrefService;

namespace base {
class Value;
}

namespace decentralized_dns {

enum class Provider;

bool IsDecentralizedDnsEnabled();

bool IsUnstoppableDomainsTLD(const GURL& url);
bool IsUnstoppableDomainsResolveMethodAsk(PrefService* local_state);
bool IsUnstoppableDomainsResolveMethodDoH(PrefService* local_state);
bool IsUnstoppableDomainsResolveMethodEthereum(PrefService* local_state);

bool IsENSTLD(const GURL& url);
bool IsENSResolveMethodAsk(PrefService* local_state);
bool IsENSResolveMethodDoH(PrefService* local_state);

base::Value GetResolveMethodList(Provider provider);

}  // namespace decentralized_dns

#endif  // adrbrowsiel_COMPONENTS_DECENTRALIZED_DNS_UTILS_H_
