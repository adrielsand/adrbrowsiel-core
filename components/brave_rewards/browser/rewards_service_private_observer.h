/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_REWARDS_BROWSER_REWARDS_SERVICE_PRIVATE_OBSERVER_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_REWARDS_BROWSER_REWARDS_SERVICE_PRIVATE_OBSERVER_H_

#include "base/observer_list_types.h"
#include "bat/ledger/mojom_structs.h"

namespace adrbrowsiel_rewards {

class RewardsService;

class RewardsServicePrivateObserver : public base::CheckedObserver {
 public:
  ~RewardsServicePrivateObserver() override {}

  virtual void OnPanelPublisherInfo(
      RewardsService* rewards_service,
      const ledger::type::Result result,
      const ledger::type::PublisherInfo* info,
      uint64_t windowId) {}
};

}  // namespace adrbrowsiel_rewards

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_REWARDS_BROWSER_REWARDS_SERVICE_PRIVATE_OBSERVER_H_
