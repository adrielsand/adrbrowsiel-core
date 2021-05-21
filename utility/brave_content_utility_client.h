/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_UTILITY_adrbrowsiel_CONTENT_UTILITY_CLIENT_H_
#define adrbrowsiel_UTILITY_adrbrowsiel_CONTENT_UTILITY_CLIENT_H_

#include <string>

#include "chrome/utility/chrome_content_utility_client.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"

class adrbrowsielContentUtilityClient : public ChromeContentUtilityClient {
 public:
  adrbrowsielContentUtilityClient();
  ~adrbrowsielContentUtilityClient() override;

 private:
  DISALLOW_COPY_AND_ASSIGN(adrbrowsielContentUtilityClient);
};

#endif  // adrbrowsiel_UTILITY_adrbrowsiel_CONTENT_UTILITY_CLIENT_H_
