/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_AD_SERVER_GET_CATALOG_URL_REQUEST_BUILDER_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_AD_SERVER_GET_CATALOG_URL_REQUEST_BUILDER_H_

#include <string>

#include "bat/ads/internal/server/url_request_builder.h"

namespace ads {

class GetCatalogUrlRequestBuilder : UrlRequestBuilder {
 public:
  GetCatalogUrlRequestBuilder();

  ~GetCatalogUrlRequestBuilder() override;

  UrlRequestPtr Build() override;

 private:
  std::string BuildUrl() const;
};

}  // namespace ads

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_AD_SERVER_GET_CATALOG_URL_REQUEST_BUILDER_H_
