/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_COMPONENT_UPDATER_BROWSER_adrbrowsiel_ON_DEMAND_UPDATER_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_COMPONENT_UPDATER_BROWSER_adrbrowsiel_ON_DEMAND_UPDATER_H_

#include <string>

#include "base/callback.h"
#include "base/macros.h"

namespace base {
template <typename T>
struct DefaultSingletonTraits;
}  // namespace base

namespace adrbrowsiel_component_updater {

class adrbrowsielOnDemandUpdater {
 public:
  using Callback = base::RepeatingCallback<void(const std::string&)>;
  static adrbrowsielOnDemandUpdater* GetInstance();

  ~adrbrowsielOnDemandUpdater();
  void OnDemandUpdate(const std::string& id);

  void RegisterOnDemandUpdateCallback(Callback callback);

 private:
  friend struct base::DefaultSingletonTraits<adrbrowsielOnDemandUpdater>;
  adrbrowsielOnDemandUpdater();

  Callback on_demand_update_callback_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielOnDemandUpdater);
};

}  // namespace adrbrowsiel_component_updater

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_COMPONENT_UPDATER_BROWSER_adrbrowsiel_ON_DEMAND_UPDATER_H_
