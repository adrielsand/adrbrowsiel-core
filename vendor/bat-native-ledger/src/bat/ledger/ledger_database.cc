/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ledger/ledger_database.h"
#include "bat/ledger/internal/ledger_database_impl.h"

namespace ledger {

LedgerDatabase* LedgerDatabase::CreateInstance(const base::FilePath& path) {
  return new LedgerDatabaseImpl(path);
}

}  // namespace ledger
