/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ledger/global_constants.h"
#include "bat/ledger/internal/endpoint/api/api_util.h"
#include "bat/ledger/ledger.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- adrbrowsiel_unit_tests --filter=APIUtilTest.*

namespace ledger {
namespace endpoint {
namespace api {

class APIUtilTest : public testing::Test {
};

TEST(APIUtilTest, GetServerUrlDevelopment) {
  ledger::_environment = type::Environment::DEVELOPMENT;
  const std::string url = GetServerUrl("/test");
  ASSERT_EQ(url, "https://api.rewards.adrbrowsiel.software/test");
}

TEST(APIUtilTest, GetServerUrlStaging) {
  ledger::_environment = type::Environment::STAGING;
  const std::string url = GetServerUrl("/test");
  ASSERT_EQ(url, "https://api.rewards.adrbrowsielsoftware.com/test");
}

TEST(APIUtilTest, GetServerUrlProduction) {
  ledger::_environment = type::Environment::PRODUCTION;
  const std::string url = GetServerUrl("/test");
  ASSERT_EQ(url, "https://api.rewards.adrbrowsiel.com/test");
}

}  // namespace api
}  // namespace endpoint
}  // namespace ledger
