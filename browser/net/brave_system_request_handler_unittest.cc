/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/net/adrbrowsiel_system_request_handler.h"

#include <string>

#include "services/network/public/cpp/resource_request.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace adrbrowsiel {

TEST(adrbrowsielSystemRequestHandlerTest, AddadrbrowsielServiceKeyHeaderForadrbrowsiel) {
  GURL url("https://demo.adrbrowsiel.com");
  network::ResourceRequest request;

  request.url = url;
  adrbrowsiel::AddadrbrowsielServicesKeyHeader(&request);
  std::string key;
  EXPECT_TRUE(request.headers.GetHeader(kadrbrowsielServicesKeyHeader, &key));
  EXPECT_EQ(key, adrbrowsielServicesKeyForTesting());
}

TEST(adrbrowsielSystemRequestHandlerTest, AddadrbrowsielServiceKeyHeaderForadrbrowsielSoftware) {
  GURL url("https://demo.adrbrowsielsoftware.com");
  network::ResourceRequest request;

  request.url = url;
  adrbrowsiel::AddadrbrowsielServicesKeyHeader(&request);
  std::string key;
  EXPECT_TRUE(request.headers.GetHeader(kadrbrowsielServicesKeyHeader, &key));
  EXPECT_EQ(key, adrbrowsielServicesKeyForTesting());
}

TEST(adrbrowsielSystemRequestHandlerTest, DontAddadrbrowsielServiceKeyHeader) {
  GURL url("https://demo.example.com");
  network::ResourceRequest request;

  request.url = url;
  adrbrowsiel::AddadrbrowsielServicesKeyHeader(&request);
  std::string key;
  EXPECT_FALSE(request.headers.GetHeader(kadrbrowsielServicesKeyHeader, &key));
}

}  // namespace adrbrowsiel
