/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/net/ipfs_redirect_network_delegate_helper.h"

#include <string>

#include "adrbrowsiel/components/ipfs/ipfs_utils.h"
#include "net/base/net_errors.h"

namespace ipfs {

int OnBeforeURLRequest_IPFSRedirectWork(
    const adrbrowsiel::ResponseCallback& next_callback,
    std::shared_ptr<adrbrowsiel::adrbrowsielRequestInfo> ctx) {
  if (!ctx->browser_context ||
      IsIpfsResolveMethodDisabled(ctx->browser_context)) {
    return net::OK;
  }

  GURL new_url;
  if (ipfs::TranslateIPFSURI(ctx->request_url, &new_url, ctx->ipfs_gateway_url,
                             false)) {
    // We only allow translating ipfs:// and ipns:// URIs if the initiator_url
    // is from the same adrbrowsiel ipfs/ipns gateway.
    // For the local case, we don't want a normal site to be able to populate
    // a user's IPFS local cache with content they didn't know about.
    // In which case that user would also be able to serve that content.
    // If the user is not using a local node, we want the experience to be
    // the same as the local case.
    if (ctx->resource_type == blink::mojom::ResourceType::kMainFrame ||
        (IsLocalGatewayURL(new_url) && IsLocalGatewayURL(ctx->initiator_url)) ||
        (IsDefaultGatewayURL(new_url, ctx->browser_context) &&
         IsDefaultGatewayURL(ctx->initiator_url, ctx->browser_context))) {
      ctx->new_url_spec = new_url.spec();
    } else {
      ctx->blocked_by = adrbrowsiel::kOtherBlocked;
    }
  }
  return net::OK;
}

int OnHeadersReceived_IPFSRedirectWork(
    const net::HttpResponseHeaders* response_headers,
    scoped_refptr<net::HttpResponseHeaders>* override_response_headers,
    GURL* allowed_unsafe_redirect_url,
    const adrbrowsiel::ResponseCallback& next_callback,
    std::shared_ptr<adrbrowsiel::adrbrowsielRequestInfo> ctx) {
  if (!ctx->browser_context ||
      IsIpfsResolveMethodDisabled(ctx->browser_context)) {
    return net::OK;
  }

  std::string ipfs_path;
  if (ctx->ipfs_auto_fallback && response_headers &&
      response_headers->GetNormalizedHeader("x-ipfs-path", &ipfs_path) &&
      // Make sure we don't infinite redirect
      !ctx->request_url.DomainIs(ctx->ipfs_gateway_url.host())) {
    GURL::Replacements replacements;
    replacements.SetPathStr(ipfs_path);
    GURL new_url = ctx->ipfs_gateway_url.ReplaceComponents(replacements);

    *override_response_headers =
        new net::HttpResponseHeaders(response_headers->raw_headers());
    (*override_response_headers)
        ->ReplaceStatusLine("HTTP/1.1 307 Temporary Redirect");
    (*override_response_headers)->RemoveHeader("Location");
    (*override_response_headers)->AddHeader("Location", new_url.spec());
    *allowed_unsafe_redirect_url = new_url;
  }

  return net::OK;
}

}  // namespace ipfs
