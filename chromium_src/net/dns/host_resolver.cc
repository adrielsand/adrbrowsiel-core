/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/net/dns/adrbrowsiel_resolve_context.h"
#include "net/dns/context_host_resolver.h"

#define ResolveContext adrbrowsielResolveContext
#include "../../../../net/dns/host_resolver.cc"
#undef ResolveContext
