/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_NET_adrbrowsiel_REFERRALS_NETWORK_DELEGATE_HELPER_H_
#define adrbrowsiel_BROWSER_NET_adrbrowsiel_REFERRALS_NETWORK_DELEGATE_HELPER_H_

#include <memory>

#include "adrbrowsiel/browser/net/url_context.h"

struct adrbrowsielRequestInfo;

namespace net {
class HttpRequestHeaders;
class URLRequest;
}

namespace adrbrowsiel {

int OnBeforeStartTransaction_ReferralsWork(
    net::HttpRequestHeaders* headers,
    const ResponseCallback& next_callback,
    std::shared_ptr<adrbrowsielRequestInfo> ctx);

}  // namespace adrbrowsiel

#endif  // adrbrowsiel_BROWSER_NET_adrbrowsiel_REFERRALS_NETWORK_DELEGATE_HELPER_H_
