/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_NET_adrbrowsiel_TORRENT_REDIRECT_NETWORK_DELEGATE_HELPER_H_
#define adrbrowsiel_BROWSER_NET_adrbrowsiel_TORRENT_REDIRECT_NETWORK_DELEGATE_HELPER_H_

#include <memory>

#include "adrbrowsiel/browser/net/url_context.h"

namespace adrbrowsiel {
struct adrbrowsielRequestInfo;
}

namespace net {
class URLRequest;
}

namespace webtorrent {

int OnHeadersReceived_TorrentRedirectWork(
    const net::HttpResponseHeaders* original_response_headers,
    scoped_refptr<net::HttpResponseHeaders>* override_response_headers,
    GURL* allowed_unsafe_redirect_url,
    const adrbrowsiel::ResponseCallback& next_callback,
    std::shared_ptr<adrbrowsiel::adrbrowsielRequestInfo> ctx);

}  // namespace webtorrent

#endif  // adrbrowsiel_BROWSER_NET_adrbrowsiel_TORRENT_REDIRECT_NETWORK_DELEGATE_HELPER_H_
