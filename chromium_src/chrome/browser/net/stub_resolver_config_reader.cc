/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/decentralized_dns/buildflags/buildflags.h"
#include "components/prefs/pref_service.h"

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
#include "base/strings/strcat.h"
#include "adrbrowsiel/components/decentralized_dns/utils.h"
#include "adrbrowsiel/net/decentralized_dns/constants.h"
#endif

namespace {

// Add DoH servers to support decentralized DNS such as Unstoppable Domains and
// ENS. These servers are controlled using its own prefs and not
// kDnsOverHttpsTemplates pref as the servers we added here are special and
// only applies to certain TLD which is different from user's global DoH
// provider settings.
void AddDoHServers(std::string* doh_templates,
                   PrefService* local_state,
                   bool force_check_parental_controls_for_automatic_mode) {
  // force_check_parental_controls_for_automatic_mode is only true for
  // settings UI which we specifically do not want to display those special
  // resolvers we added.
  if (force_check_parental_controls_for_automatic_mode)
    return;

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
  if (decentralized_dns::IsUnstoppableDomainsResolveMethodDoH(local_state) &&
      doh_templates->find(decentralized_dns::kUnstoppableDomainsDoHResolver) ==
          std::string::npos) {
    *doh_templates =
        base::StrCat({decentralized_dns::kUnstoppableDomainsDoHResolver, " ",
                      *doh_templates});
  }

  if (decentralized_dns::IsENSResolveMethodDoH(local_state) &&
      doh_templates->find(decentralized_dns::kENSDoHResolver) ==
          std::string::npos) {
    *doh_templates =
        base::StrCat({decentralized_dns::kENSDoHResolver, " ", *doh_templates});
  }
#endif
}

}  // namespace

#define adrbrowsiel_GET_AND_UPDATE_CONFIGURATION    \
  AddDoHServers(&doh_templates, local_state_, \
                force_check_parental_controls_for_automatic_mode);

#include "../../../../../chrome/browser/net/stub_resolver_config_reader.cc"
#undef adrbrowsiel_GET_AND_UPDATE_CONFIGURATION
