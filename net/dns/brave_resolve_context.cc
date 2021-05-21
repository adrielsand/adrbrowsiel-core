/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/net/dns/adrbrowsiel_resolve_context.h"

#include <string>

#include "adrbrowsiel/net/decentralized_dns/constants.h"
#include "net/dns/dns_session.h"

namespace {

bool IsDecentralizedDNSResolver(const std::string& server) {
  return server == decentralized_dns::kUnstoppableDomainsDoHResolver ||
         server == decentralized_dns::kENSDoHResolver;
}

}  // namespace

namespace net {

adrbrowsielResolveContext::adrbrowsielResolveContext(URLRequestContext* url_request_context,
                                         bool enable_caching)
    : ResolveContext(url_request_context, enable_caching) {}

adrbrowsielResolveContext::~adrbrowsielResolveContext() = default;

bool adrbrowsielResolveContext::IsFirstProbeCompleted(const ServerStats& stat) const {
  return !(stat.last_failure_count == 0 &&
           stat.current_connection_success == false);
}

bool adrbrowsielResolveContext::GetDohServerAvailability(
    size_t doh_server_index,
    const DnsSession* session) const {
  // Return decentralized DNS resolvers as available before the first probe is
  // completed. It is to avoid falling back to non-secure DNS servers before
  // the first probe is completed when users using automatic mode, which will
  // lead to an error page with HOSTNAME_NOT_RESOLVED error right after user
  // opt-in from the interstitial page.
  if (IsDecentralizedDNSResolver(session->config()
                                     .dns_over_https_servers[doh_server_index]
                                     .server_template) &&
      !IsFirstProbeCompleted(doh_server_stats_[doh_server_index]))
    return true;

  return ResolveContext::GetDohServerAvailability(doh_server_index, session);
}

size_t adrbrowsielResolveContext::NumAvailableDohServers(
    const DnsSession* session) const {
  size_t num = 0;

  // Treat decentralized DNS resolvers as available before the first probe is
  // completed.
  for (size_t i = 0; i < doh_server_stats_.size(); i++) {
    if (IsDecentralizedDNSResolver(
            session->config().dns_over_https_servers[i].server_template) &&
        !IsFirstProbeCompleted(doh_server_stats_[i]))
      num++;
  }

  return num + ResolveContext::NumAvailableDohServers(session);
}

}  // namespace net
