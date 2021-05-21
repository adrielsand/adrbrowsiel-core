/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/adrbrowsiel_ads/browser/component_updater/component_info.h"

namespace adrbrowsiel_ads {

ComponentInfo::ComponentInfo() = default;

ComponentInfo::ComponentInfo(const std::string& id,
                             const std::string& public_key)
    : id(id), public_key(public_key) {}

ComponentInfo::~ComponentInfo() = default;

}  // namespace adrbrowsiel_ads
