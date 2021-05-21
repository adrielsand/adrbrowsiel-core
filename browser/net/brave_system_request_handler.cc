/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/net/adrbrowsiel_system_request_handler.h"

#include "adrbrowsiel/browser/net/adrbrowsiel_block_safebrowsing_urls.h"
#include "adrbrowsiel/browser/net/adrbrowsiel_common_static_redirect_network_delegate_helper.h"
#include "adrbrowsiel/browser/net/adrbrowsiel_static_redirect_network_delegate_helper.h"
#include "adrbrowsiel/common/network_constants.h"
#include "extensions/common/url_pattern.h"
#include "services/network/public/cpp/resource_request.h"
#include "url/gurl.h"

namespace adrbrowsiel {

std::string adrbrowsielServicesKeyForTesting() {
  return adrbrowsiel_SERVICES_KEY;
}

void AddadrbrowsielServicesKeyHeader(network::ResourceRequest* url_request) {
  static URLPattern adrbrowsiel_proxy_pattern(URLPattern::SCHEME_HTTPS,
                                        kadrbrowsielProxyPattern);
  static URLPattern adrbrowsielsoftware_proxy_pattern(URLPattern::SCHEME_HTTPS,
                                                kadrbrowsielSoftwareProxyPattern);
  if (adrbrowsiel_proxy_pattern.MatchesURL(url_request->url) ||
      adrbrowsielsoftware_proxy_pattern.MatchesURL(url_request->url)) {
    url_request->headers.SetHeaderIfMissing(kadrbrowsielServicesKeyHeader,
                                            adrbrowsiel_SERVICES_KEY);
  }
  return;
}

network::ResourceRequest OnBeforeSystemRequest(
    const network::ResourceRequest& url_request) {
  GURL new_url;
  adrbrowsiel::OnBeforeURLRequest_BlockSafeBrowsingReportingURLs(url_request.url,
                                                           &new_url);
  adrbrowsiel::OnBeforeURLRequest_StaticRedirectWorkForGURL(url_request.url,
                                                      &new_url);
  adrbrowsiel::OnBeforeURLRequest_CommonStaticRedirectWorkForGURL(url_request.url,
                                                            &new_url);
  network::ResourceRequest patched_request = url_request;
  if (!new_url.is_empty()) {
    patched_request.url = new_url;
  }
  AddadrbrowsielServicesKeyHeader(&patched_request);
  return patched_request;
}

}  // namespace adrbrowsiel
