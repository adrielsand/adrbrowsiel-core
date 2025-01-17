/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsielLEDGER_DATABASE_DATABASE_PENDING_CONTRIBUTION_H_
#define adrbrowsielLEDGER_DATABASE_DATABASE_PENDING_CONTRIBUTION_H_

#include <stdint.h>

#include <string>

#include "bat/ledger/internal/database/database_table.h"

namespace ledger {
namespace database {

class DatabasePendingContribution: public DatabaseTable {
 public:
  explicit DatabasePendingContribution(LedgerImpl* ledger);
  ~DatabasePendingContribution() override;

  void InsertOrUpdateList(
      type::PendingContributionList list,
      ledger::ResultCallback callback);

  void GetReservedAmount(ledger::PendingContributionsTotalCallback callback);

  void GetAllRecords(ledger::PendingContributionInfoListCallback callback);

  void DeleteRecord(const uint64_t id, ledger::ResultCallback callback);

  void DeleteAllRecords(ledger::ResultCallback callback);

 private:
  void OnGetReservedAmount(
      type::DBCommandResponsePtr response,
      ledger::PendingContributionsTotalCallback callback);

  void OnGetAllRecords(
      type::DBCommandResponsePtr response,
      ledger::PendingContributionInfoListCallback callback);
};

}  // namespace database
}  // namespace ledger

#endif  // adrbrowsielLEDGER_DATABASE_DATABASE_PENDING_CONTRIBUTION_H_
