/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_LEDGER_INCLUDE_BAT_LEDGER_OPTION_KEYS_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_LEDGER_INCLUDE_BAT_LEDGER_OPTION_KEYS_H_

#include <string>

namespace ledger {
namespace option {

const char kPublisherListRefreshInterval[] = "publisher_list_refresh_interval";
const char kClaimUGP[] = "claim_ugp";
const char kContributionsDisabledForBAPMigration[] =
    "contributions_disabled_for_bap_migration";
const char kIsBitflyerRegion[] = "is_bitflyer_region";

}  // namespace option
}  // namespace ledger

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_LEDGER_INCLUDE_BAT_LEDGER_OPTION_KEYS_H_
