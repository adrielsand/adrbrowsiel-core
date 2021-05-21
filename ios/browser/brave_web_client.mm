/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#import "adrbrowsiel/ios/browser/adrbrowsiel_web_client.h"

#include <string>

#include "base/bind.h"
#include "adrbrowsiel/ios/browser/adrbrowsiel_web_main_parts.h"
#include "ui/base/resource/resource_bundle.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

adrbrowsielWebClient::adrbrowsielWebClient() : web_main_parts_(nullptr) {}

adrbrowsielWebClient::~adrbrowsielWebClient() {
}

std::unique_ptr<web::WebMainParts> adrbrowsielWebClient::CreateWebMainParts() {
  auto web_main_parts = std::make_unique<adrbrowsielWebMainParts>();
  web_main_parts_ = web_main_parts.get();
  return web_main_parts;
}

void adrbrowsielWebClient::SetUserAgent(const std::string& user_agent) {
  user_agent_ = user_agent;
}

std::string adrbrowsielWebClient::GetUserAgent(web::UserAgentType type) const {
  return user_agent_;
}

base::StringPiece adrbrowsielWebClient::GetDataResource(
    int resource_id,
    ui::ScaleFactor scale_factor) const {
  return ui::ResourceBundle::GetSharedInstance().GetRawDataResourceForScale(
      resource_id, scale_factor);
}

base::RefCountedMemory* adrbrowsielWebClient::GetDataResourceBytes(
    int resource_id) const {
  return ui::ResourceBundle::GetSharedInstance().LoadDataResourceBytes(
      resource_id);
}
