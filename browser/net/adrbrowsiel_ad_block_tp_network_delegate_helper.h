/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_NET_adrbrowsiel_AD_BLOCK_TP_NETWORK_DELEGATE_HELPER_H_
#define adrbrowsiel_BROWSER_NET_adrbrowsiel_AD_BLOCK_TP_NETWORK_DELEGATE_HELPER_H_

#include <memory>

#include "adrbrowsiel/browser/net/url_context.h"

namespace network {
class HostResolver;
}  // namespace network

namespace adrbrowsiel {

int OnBeforeURLRequest_AdBlockTPPreWork(
    const ResponseCallback& next_callback,
    std::shared_ptr<adrbrowsielRequestInfo> ctx);

// Be sure to reset this to `nullptr` when done testing to prevent future tests
// from being affected.
void SetAdblockCnameHostResolverForTesting(
    network::HostResolver* host_resolver);

}  // namespace adrbrowsiel

#endif  // adrbrowsiel_BROWSER_NET_adrbrowsiel_AD_BLOCK_TP_NETWORK_DELEGATE_HELPER_H_
