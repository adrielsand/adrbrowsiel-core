/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/adrbrowsiel_component_updater/browser/adrbrowsiel_on_demand_updater.h"

#include <string>

#include "base/memory/singleton.h"

namespace adrbrowsiel_component_updater {

adrbrowsielOnDemandUpdater* adrbrowsielOnDemandUpdater::GetInstance() {
  return base::Singleton<adrbrowsielOnDemandUpdater>::get();
}

adrbrowsielOnDemandUpdater::adrbrowsielOnDemandUpdater() {}

adrbrowsielOnDemandUpdater::~adrbrowsielOnDemandUpdater() {}

void adrbrowsielOnDemandUpdater::OnDemandUpdate(const std::string& id) {
  DCHECK(!on_demand_update_callback_.is_null());
  on_demand_update_callback_.Run(id);
}

void adrbrowsielOnDemandUpdater::RegisterOnDemandUpdateCallback(Callback callback) {
  on_demand_update_callback_ = callback;
}


}  // namespace adrbrowsiel_component_updater
