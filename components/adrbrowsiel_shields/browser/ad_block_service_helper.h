/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_SHIELDS_BROWSER_AD_BLOCK_SERVICE_HELPER_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_SHIELDS_BROWSER_AD_BLOCK_SERVICE_HELPER_H_

#include <stdint.h>
#include <string>
#include <vector>

#include "base/values.h"
#include "adrbrowsiel/components/adblock_rust_ffi/src/wrapper.h"

namespace adrbrowsiel_shields {

std::vector<adblock::FilterList>::const_iterator FindAdBlockFilterListByUUID(
    const std::vector<adblock::FilterList>& region_lists,
    const std::string& uuid);
std::vector<adblock::FilterList>::const_iterator FindAdBlockFilterListByLocale(
    const std::vector<adblock::FilterList>& region_lists,
    const std::string& locale);

std::vector<adblock::FilterList> RegionalCatalogFromJSON(
    const std::string& catalog_json);

void MergeCspDirectiveInto(base::Optional<std::string> from,
                           base::Optional<std::string>* into);

void MergeResourcesInto(base::Value from, base::Value* into, bool force_hide);

}  // namespace adrbrowsiel_shields

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_SHIELDS_BROWSER_AD_BLOCK_SERVICE_HELPER_H_
