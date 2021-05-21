/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/adrbrowsiel_shields/browser/base_adrbrowsiel_shields_service.h"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "base/bind.h"
#include "base/callback_helpers.h"
#include "base/logging.h"
#include "base/macros.h"
#include "base/memory/ptr_util.h"

using adrbrowsiel_component_updater::adrbrowsielComponent;

namespace adrbrowsiel_shields {

BaseadrbrowsielShieldsService::BaseadrbrowsielShieldsService(
    adrbrowsielComponent::Delegate* delegate)
    : adrbrowsielComponent(delegate),
      initialized_(false) {
}

BaseadrbrowsielShieldsService::~BaseadrbrowsielShieldsService() {
}

bool BaseadrbrowsielShieldsService::IsInitialized() const {
  return initialized_;
}

void BaseadrbrowsielShieldsService::InitShields() {
  if (Init()) {
    std::lock_guard<std::mutex> guard(initialized_mutex_);
    initialized_ = true;
  }
}

bool BaseadrbrowsielShieldsService::Start() {
  if (initialized_) {
    return true;
  }

  InitShields();
  return false;
}

void BaseadrbrowsielShieldsService::ShouldStartRequest(
    const GURL& url,
    blink::mojom::ResourceType resource_type,
    const std::string& tab_host,
    bool* did_match_rule,
    bool* did_match_exception,
    bool* did_match_important,
    std::string* mock_data_url) {
}

}  // namespace adrbrowsiel_shields
