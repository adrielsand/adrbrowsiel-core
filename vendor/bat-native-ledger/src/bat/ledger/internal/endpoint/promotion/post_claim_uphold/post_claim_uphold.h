/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsielLEDGER_ENDPOINT_PROMOTION_POST_CLAIM_UPHOLD_POST_CLAIM_UPHOLD_H_
#define adrbrowsielLEDGER_ENDPOINT_PROMOTION_POST_CLAIM_UPHOLD_POST_CLAIM_UPHOLD_H_

#include <string>

#include "bat/ledger/ledger.h"

// POST /v3/wallet/uphold/{payment_id}/claim
//
// Request body:
// {
//   "signedLinkingRequest": "......",
//   "anonymousAddress": "asfeq4gerg34gl3g34lg34g"
// }
//
// Success code:
// HTTP_OK (200)
//
// Error codes:
// HTTP_BAD_REQUEST (400)
// HTTP_FORBIDDEN (403)
// HTTP_NOT_FOUND (404)
// HTTP_CONFLICT (409)
// HTTP_INTERNAL_SERVER_ERROR (500)
//
// Response body:
// {Empty}

namespace ledger {
class LedgerImpl;

namespace endpoint {
namespace promotion {

using PostClaimUpholdCallback = std::function<void(
    const type::Result result)>;

class PostClaimUphold {
 public:
  explicit PostClaimUphold(LedgerImpl* ledger);
  ~PostClaimUphold();

  void Request(
      const double user_funds,
      PostClaimUpholdCallback callback);

 private:
  std::string GetUrl();

  std::string GeneratePayload(const double user_funds);

  type::Result CheckStatusCode(const int status_code);

  type::Result ParseBody(
      const std::string& body,
      std::string* payment_id);

  void OnRequest(
      const type::UrlResponse& response,
      PostClaimUpholdCallback callback);

  LedgerImpl* ledger_;  // NOT OWNED
};

}  // namespace promotion
}  // namespace endpoint
}  // namespace ledger

#endif  // adrbrowsielLEDGER_ENDPOINT_PROMOTION_POST_CLAIM_UPHOLD_POST_CLAIM_UPHOLD_H_
