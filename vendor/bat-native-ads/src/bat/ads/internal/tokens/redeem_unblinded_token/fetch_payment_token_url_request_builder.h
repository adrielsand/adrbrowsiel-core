/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_TOKENS_REDEEM_UNBLINDED_TOKEN_FETCH_PAYMENT_TOKEN_URL_REQUEST_BUILDER_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_TOKENS_REDEEM_UNBLINDED_TOKEN_FETCH_PAYMENT_TOKEN_URL_REQUEST_BUILDER_H_

#include <string>

#include "bat/ads/internal/account/confirmations/confirmation_info.h"
#include "bat/ads/internal/server/url_request_builder.h"

namespace ads {

class FetchPaymentTokenUrlRequestBuilder : UrlRequestBuilder {
 public:
  explicit FetchPaymentTokenUrlRequestBuilder(
      const ConfirmationInfo& confirmation);

  ~FetchPaymentTokenUrlRequestBuilder() override;

  UrlRequestPtr Build() override;

 private:
  ConfirmationInfo confirmation_;

  std::string BuildUrl() const;
};

}  // namespace ads

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_TOKENS_REDEEM_UNBLINDED_TOKEN_FETCH_PAYMENT_TOKEN_URL_REQUEST_BUILDER_H_
