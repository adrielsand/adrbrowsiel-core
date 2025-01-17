/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ACCOUNT_STATEMENT_STATEMENT_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ACCOUNT_STATEMENT_STATEMENT_H_

#include <cstdint>

#include "base/time/time.h"

namespace ads {

class AdRewards;
struct StatementInfo;

class Statement {
 public:
  explicit Statement(AdRewards* ad_rewards);

  ~Statement();

  StatementInfo Get(const int64_t from_timestamp,
                    const int64_t to_timestamp) const;

 private:
  double GetEarningsForThisMonth() const;
  double GetEarningsForLastMonth() const;

  uint64_t GetAdsReceivedThisMonth() const;

  AdRewards* ad_rewards_;  // NOT OWNED
};

}  // namespace ads

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ACCOUNT_STATEMENT_STATEMENT_H_
