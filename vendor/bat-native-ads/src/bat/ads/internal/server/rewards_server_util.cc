/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/server/rewards_server_util.h"

#include "bat/ads/ads.h"

namespace ads {
namespace rewards {
namespace server {

namespace {

const char kProductionDomain[] = "https://grant.rewards.adrbrowsiel.com";
const char kStagingDomain[] = "https://grant.rewards.adrbrowsielsoftware.com";
const char kDevelopmentDomain[] = "https://grant.rewards.adrbrowsiel.software";

}  // namespace

std::string GetHost() {
  switch (g_environment) {
    case Environment::PRODUCTION: {
      return kProductionDomain;
    }

    case Environment::STAGING: {
      return kStagingDomain;
    }

    case Environment::DEVELOPMENT: {
      return kDevelopmentDomain;
    }
  }
}

}  // namespace server
}  // namespace rewards
}  // namespace ads
