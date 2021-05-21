/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsielLEDGER_ENDPOINT_PROMOTION_POST_CLAIM_adrbrowsiel_POST_CLAIM_adrbrowsiel_H_
#define adrbrowsielLEDGER_ENDPOINT_PROMOTION_POST_CLAIM_adrbrowsiel_POST_CLAIM_adrbrowsiel_H_

#include <string>

#include "bat/ledger/ledger.h"

// POST /v3/wallet/adrbrowsiel/{payment_id}/claim
//
// Request body:
// {
//   "depositDestination": "83b3b77b-e7c3-455b-adda-e476fa0656d2"
// }
//
// Success code:
// HTTP_OK (200)
//
// Error codes:
// HTTP_BAD_REQUEST (400)
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

using PostClaimadrbrowsielCallback = std::function<void(const type::Result result)>;

class PostClaimadrbrowsiel {
 public:
  explicit PostClaimadrbrowsiel(LedgerImpl* ledger);
  ~PostClaimadrbrowsiel();

  void Request(
      const std::string& destination_payment_id,
      PostClaimadrbrowsielCallback callback);

 private:
  std::string GetUrl();

  std::string GeneratePayload(const std::string& destination_payment_id);

  type::Result CheckStatusCode(const int status_code);

  void OnRequest(
      const type::UrlResponse& response,
      PostClaimadrbrowsielCallback callback);

  LedgerImpl* ledger_;  // NOT OWNED
};

}  // namespace promotion
}  // namespace endpoint
}  // namespace ledger

#endif  // adrbrowsielLEDGER_ENDPOINT_PROMOTION_POST_CLAIM_adrbrowsiel_POST_CLAIM_adrbrowsiel_H_
