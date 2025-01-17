/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsielLEDGER_CONTRIBUTION_UNVERIFIED_H_
#define adrbrowsielLEDGER_CONTRIBUTION_UNVERIFIED_H_

#include <stdint.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "base/timer/timer.h"
#include "bat/ledger/ledger.h"

namespace ledger {
class LedgerImpl;

namespace contribution {

class Unverified {
 public:
  explicit Unverified(LedgerImpl* ledger);

  ~Unverified();

  void Contribute();

 private:
  void WasPublisherProcessed(
      const type::Result result,
      const std::string& publisher_key,
      const std::string& name);

  void ProcessedPublisherSaved(
      const type::Result result,
      const std::string& publisher_key,
      const std::string& name);

  void OnRemovePendingContribution(type::Result result);

  void OnContributeUnverifiedBalance(
      type::Result result,
      type::BalancePtr properties);

  void OnContributeUnverifiedPublishers(
      double balance,
      const type::PendingContributionInfoList& list);

  void QueueSaved(
      const type::Result result,
      const uint64_t pending_contribution_id);

  LedgerImpl* ledger_;  // NOT OWNED
  base::OneShotTimer unverified_publishers_timer_;
};

}  // namespace contribution
}  // namespace ledger
#endif  // adrbrowsielLEDGER_CONTRIBUTION_UNVERIFIED_H_
