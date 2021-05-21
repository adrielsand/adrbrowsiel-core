/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/stl_util.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"
#include "adrbrowsiel/components/ipfs/pref_names.h"
#include "adrbrowsiel/components/tor/buildflags/buildflags.h"
#include "adrbrowsiel/components/tor/pref_names.h"
#include "chrome/browser/policy/configuration_policy_handler_list_factory.h"
#include "components/policy/core/browser/configuration_policy_handler.h"
#include "components/policy/policy_constants.h"

namespace {

// Wrap whole array definition in build flags to avoid unused variable build
// error. It can happen if the platform doesn't support any of these features.
#if BUILDFLAG(ENABLE_TOR) || BUILDFLAG(IPFS_ENABLED)
const policy::PolicyToPreferenceMapEntry kadrbrowsielSimplePolicyMap[] = {
#if BUILDFLAG(ENABLE_TOR)
  { policy::key::kTorDisabled,
    tor::prefs::kTorDisabled,
    base::Value::Type::BOOLEAN },
#endif
#if BUILDFLAG(IPFS_ENABLED)
  { policy::key::kIPFSEnabled,
    kIPFSEnabled,
    base::Value::Type::BOOLEAN },
#endif
};
#endif  // BUILDFLAG(ENABLE_TOR) || BUILDFLAG(IPFS_ENABLED)

}  // namespace

#define BuildHandlerList BuildHandlerList_ChromiumImpl
#include "../../../../../chrome/browser/policy/configuration_policy_handler_list_factory.cc"  // NOLINT
#undef BuildHandlerList

namespace policy {

std::unique_ptr<ConfigurationPolicyHandlerList> BuildHandlerList(
    const Schema& chrome_schema) {
  std::unique_ptr<ConfigurationPolicyHandlerList> handlers =
      BuildHandlerList_ChromiumImpl(chrome_schema);

  // TODO(simonhong): Remove this guard when array size is not empty w/o tor.
  // base::size failed to instantiate with zero-size array.
#if BUILDFLAG(ENABLE_TOR) || BUILDFLAG(IPFS_ENABLED)
  for (size_t i = 0; i < base::size(kadrbrowsielSimplePolicyMap); ++i) {
    handlers->AddHandler(std::make_unique<SimplePolicyHandler>(
        kadrbrowsielSimplePolicyMap[i].policy_name,
        kadrbrowsielSimplePolicyMap[i].preference_path,
        kadrbrowsielSimplePolicyMap[i].value_type));
  }
#endif
  return handlers;
}

}  // namespace policy
