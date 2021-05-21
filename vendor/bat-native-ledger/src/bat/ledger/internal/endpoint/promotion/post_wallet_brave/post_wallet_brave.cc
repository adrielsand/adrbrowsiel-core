/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "bat/ledger/internal/endpoint/promotion/post_wallet_adrbrowsiel/post_wallet_adrbrowsiel.h"

#include <utility>

#include "base/json/json_reader.h"
#include "base/strings/stringprintf.h"
#include "bat/ledger/internal/common/security_util.h"
#include "bat/ledger/internal/endpoint/promotion/promotions_util.h"
#include "bat/ledger/internal/ledger_impl.h"
#include "bat/ledger/internal/common/request_util.h"
#include "net/http/http_status_code.h"

using std::placeholders::_1;

namespace ledger {
namespace endpoint {
namespace promotion {

PostWalletadrbrowsiel::PostWalletadrbrowsiel(LedgerImpl* ledger):
    ledger_(ledger) {
  DCHECK(ledger_);
}

PostWalletadrbrowsiel::~PostWalletadrbrowsiel() = default;

std::string PostWalletadrbrowsiel::GetUrl() {
  return GetServerUrl("/v3/wallet/adrbrowsiel");
}

type::Result PostWalletadrbrowsiel::CheckStatusCode(const int status_code) {
  if (status_code == net::HTTP_BAD_REQUEST) {
    BLOG(0, "Invalid request");
    return type::Result::LEDGER_ERROR;
  }

  if (status_code == net::HTTP_SERVICE_UNAVAILABLE) {
    BLOG(0, "No conversion rate yet in ratios service");
    return type::Result::BAD_REGISTRATION_RESPONSE;
  }

  if (status_code != net::HTTP_CREATED) {
    BLOG(0, "Unexpected HTTP status: " << status_code);
    return type::Result::LEDGER_ERROR;
  }

  return type::Result::LEDGER_OK;
}

type::Result PostWalletadrbrowsiel::ParseBody(
    const std::string& body,
    std::string* payment_id) {
  DCHECK(payment_id);

  base::Optional<base::Value> value = base::JSONReader::Read(body);
  if (!value || !value->is_dict()) {
    BLOG(0, "Invalid JSON");
    return type::Result::LEDGER_ERROR;
  }

  base::DictionaryValue* dictionary = nullptr;
  if (!value->GetAsDictionary(&dictionary)) {
    BLOG(0, "Invalid JSON");
    return type::Result::LEDGER_ERROR;
  }

  const auto* payment_id_string = dictionary->FindStringKey("paymentId");
  if (!payment_id_string || payment_id_string->empty()) {
    BLOG(1, "Payment id is wrong");
    return type::Result::LEDGER_ERROR;
  }

  *payment_id = *payment_id_string;

  return type::Result::LEDGER_OK;
}

void PostWalletadrbrowsiel::Request(
    PostWalletadrbrowsielCallback callback) {
  const auto wallet = ledger_->wallet()->GetWallet();
  if (!wallet) {
    BLOG(0, "Wallet is null");
    callback(type::Result::LEDGER_ERROR, "");
    return;
  }

  const auto headers = util::BuildSignHeaders(
      "post /v3/wallet/adrbrowsiel",
      "",
      util::Security::GetPublicKeyHexFromSeed(wallet->recovery_seed),
      wallet->recovery_seed);

  auto url_callback = std::bind(&PostWalletadrbrowsiel::OnRequest,
      this,
      _1,
      callback);

  auto request = type::UrlRequest::New();
  request->url = GetUrl();
  request->headers = headers;
  request->method = type::UrlMethod::POST;
  ledger_->LoadURL(std::move(request), url_callback);
}

void PostWalletadrbrowsiel::OnRequest(
    const type::UrlResponse& response,
    PostWalletadrbrowsielCallback callback) {
  ledger::LogUrlResponse(__func__, response);

  std::string payment_id;
  type::Result result = CheckStatusCode(response.status_code);

  if (result != type::Result::LEDGER_OK) {
    callback(result, payment_id);
    return;
  }

  result = ParseBody(response.body, &payment_id);
  callback(result, payment_id);
}

}  // namespace promotion
}  // namespace endpoint
}  // namespace ledger
