/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_ADS_BROWSER_ADS_SERVICE_OBSERVER_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_ADS_BROWSER_ADS_SERVICE_OBSERVER_H_

#include "base/observer_list.h"

namespace adrbrowsiel_ads {

class AdsServiceObserver : public base::CheckedObserver {
 public:
  virtual void OnAdRewardsChanged() = 0;

 protected:
  ~AdsServiceObserver() override = default;
};

}  // namespace adrbrowsiel_ads

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_ADS_BROWSER_ADS_SERVICE_OBSERVER_H_
