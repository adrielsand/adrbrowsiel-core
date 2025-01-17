/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_LEDGER_SRC_BAT_LEDGER_INTERNAL_CORE_BAT_LEDGER_TEST_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_LEDGER_SRC_BAT_LEDGER_INTERNAL_CORE_BAT_LEDGER_TEST_H_

#include <string>

#include "base/test/bind.h"
#include "base/test/task_environment.h"
#include "bat/ledger/internal/core/bat_ledger_context.h"
#include "bat/ledger/internal/core/test_ledger_client.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace ledger {

// Base class for unit tests. |BATLedgerTest| provides a task environment,
// access to a |BATLedgerContext|, and an test implementation of |LedgerClient|.
class BATLedgerTest : public testing::Test {
 protected:
  // Returns the |TaskEnvironment| for this test.
  base::test::TaskEnvironment* task_environment() { return &task_environment_; }

  // Return the |BATLedgerContext| for this test.
  BATLedgerContext* context() { return &context_; }

  // Returns the |TestLedgerClient| instance for this test.
  TestLedgerClient* GetTestLedgerClient() { return &client_; }

  // Adds a mock network response for the specified URL and HTTP method.
  void AddNetworkResultForTesting(const std::string& url,
                                  mojom::UrlMethod method,
                                  mojom::UrlResponsePtr response);

  // Sets a callback that is executed when a message is logged to the client.
  void SetLogCallbackForTesting(TestLedgerClient::LogCallback callback);

 private:
  base::test::TaskEnvironment task_environment_;
  TestLedgerClient client_;
  BATLedgerContext context_{&client_};
};

}  // namespace ledger

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_LEDGER_SRC_BAT_LEDGER_INTERNAL_CORE_BAT_LEDGER_TEST_H_
