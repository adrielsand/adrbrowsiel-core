/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define IsComponentExtensionAllowlisted IsComponentExtensionAllowlisted_ChromiumImpl  // NOLINT
#include "../../../../../../chrome/browser/extensions/component_extensions_allowlist/allowlist.cc"  // NOLINT
#undef IsComponentExtensionAllowlisted

#include "base/stl_util.h"
#include "adrbrowsiel/components/adrbrowsiel_extension/grit/adrbrowsiel_extension.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/resources/extension/grit/adrbrowsiel_rewards_extension_resources.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_webtorrent/grit/adrbrowsiel_webtorrent_resources.h"
#include "components/grit/adrbrowsiel_components_resources.h"
#include "extensions/common/constants.h"

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/adrbrowsiel_wallet_constants.h"
#endif

namespace extensions {

  bool IsComponentExtensionAllowlisted(const std::string& extension_id) {
    const char* const kAllowed[] = {
      adrbrowsiel_extension_id,
      adrbrowsiel_rewards_extension_id,
#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
      ethereum_remote_client_extension_id,
#endif
      adrbrowsiel_webtorrent_extension_id
    };

    for (size_t i = 0; i < base::size(kAllowed); ++i) {
      if (extension_id == kAllowed[i])
        return true;
    }

    return IsComponentExtensionAllowlisted_ChromiumImpl(extension_id);
  }

  bool IsComponentExtensionAllowlisted(int manifest_resource_id) {
    switch (manifest_resource_id) {
      // Please keep the list in alphabetical order.
      case IDR_adrbrowsiel_EXTENSION:
      case IDR_adrbrowsiel_REWARDS:
      case IDR_adrbrowsiel_WEBTORRENT:
        return true;
    }

    return IsComponentExtensionAllowlisted_ChromiumImpl(manifest_resource_id);
  }

}  // namespace extensions
