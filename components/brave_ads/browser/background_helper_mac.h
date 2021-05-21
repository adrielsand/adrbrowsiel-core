/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_ADS_BROWSER_BACKGROUND_HELPER_MAC_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_ADS_BROWSER_BACKGROUND_HELPER_MAC_H_

#include "base/compiler_specific.h"
#include "base/mac/scoped_nsobject.h"
#include "base/macros.h"
#include "base/memory/singleton.h"
#include "adrbrowsiel/components/adrbrowsiel_ads/browser/background_helper.h"

@class BackgroundHelperDelegate;

namespace adrbrowsiel_ads {

class BackgroundHelperMac : public BackgroundHelper {
 public:
  BackgroundHelperMac(const BackgroundHelperMac&) = delete;
  BackgroundHelperMac& operator=(const BackgroundHelperMac&) = delete;

  static BackgroundHelperMac* GetInstance();

 private:
  friend struct base::DefaultSingletonTraits<BackgroundHelperMac>;

  BackgroundHelperMac();
  ~BackgroundHelperMac() override;

  base::scoped_nsobject<BackgroundHelperDelegate> delegate_;

  // BackgroundHelper impl
  bool IsForeground() const override;
};

}  // namespace adrbrowsiel_ads

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_ADS_BROWSER_BACKGROUND_HELPER_MAC_H_
