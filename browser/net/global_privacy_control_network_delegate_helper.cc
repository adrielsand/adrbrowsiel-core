/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/net/global_privacy_control_network_delegate_helper.h"

#include <memory>

#include "base/feature_list.h"
#include "adrbrowsiel/common/adrbrowsiel_features.h"
#include "adrbrowsiel/common/network_constants.h"
#include "net/base/net_errors.h"
#include "net/http/http_request_headers.h"

namespace adrbrowsiel {

int OnBeforeStartTransaction_GlobalPrivacyControlWork(
    net::HttpRequestHeaders* headers,
    const ResponseCallback& next_callback,
    std::shared_ptr<adrbrowsielRequestInfo> ctx) {
  headers->SetHeader(kSecGpcHeader, "1");
  return net::OK;
}

}  // namespace adrbrowsiel
