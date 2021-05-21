/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_NET_DNS_RESOLVE_CONTEXT_H_
#define adrbrowsiel_CHROMIUM_SRC_NET_DNS_RESOLVE_CONTEXT_H_

namespace net {
class adrbrowsielResolveContext;
}  // namespace net

#define GetDohServerAvailability virtual GetDohServerAvailability
#define NumAvailableDohServers virtual NumAvailableDohServers
#define adrbrowsiel_RESOLVE_CONTEXT_H \
 private:                       \
  friend class adrbrowsielResolveContext;

#include "../../../../net/dns/resolve_context.h"
#undef GetDohServerAvailability
#undef NumAvailableDohServers
#undef adrbrowsiel_RESOLVE_CONTEXT_H

#endif  // adrbrowsiel_CHROMIUM_SRC_NET_DNS_RESOLVE_CONTEXT_H_
