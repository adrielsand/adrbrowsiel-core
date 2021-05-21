/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_NET_adrbrowsiel_COMMON_STATIC_REDIRECT_NETWORK_DELEGATE_HELPER_H_
#define adrbrowsiel_BROWSER_NET_adrbrowsiel_COMMON_STATIC_REDIRECT_NETWORK_DELEGATE_HELPER_H_

#include <memory>

#include "adrbrowsiel/browser/net/url_context.h"

class GURL;

namespace adrbrowsiel {

extern const char kUpdaterTestingEndpoint[];

int OnBeforeURLRequest_CommonStaticRedirectWork(
    const ResponseCallback& next_callback,
    std::shared_ptr<adrbrowsielRequestInfo> ctx);

int OnBeforeURLRequest_CommonStaticRedirectWorkForGURL(
    const GURL& url,
    GURL* new_url);

void SetUpdateURLHostForTesting(bool testing);
}  // namespace adrbrowsiel

#endif  // adrbrowsiel_BROWSER_NET_adrbrowsiel_COMMON_STATIC_REDIRECT_NETWORK_DELEGATE_HELPER_H_
