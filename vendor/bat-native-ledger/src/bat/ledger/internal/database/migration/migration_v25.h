/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsielLEDGER_DATABASE_MIGRATION_MIGRATION_V25_H_
#define adrbrowsielLEDGER_DATABASE_MIGRATION_MIGRATION_V25_H_

namespace ledger {
namespace database {
namespace migration {

const char v25[] = R"(
  ALTER TABLE promotion ADD legacy BOOLEAN DEFAULT 0 NOT NULL;
)";

}  // namespace migration
}  // namespace database
}  // namespace ledger

#endif  // adrbrowsielLEDGER_DATABASE_MIGRATION_MIGRATION_V25_H_
