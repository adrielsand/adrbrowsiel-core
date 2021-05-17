// Copyright (c) 2021 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "brave/components/brave_search/browser/brave_search_default_host.h"

#include <utility>

namespace brave_search {

BraveSearchDefaultHost::BraveSearchDefaultHost()
    : weak_factory_(this) {}

BraveSearchDefaultHost::~BraveSearchDefaultHost() {}

void BraveSearchDefaultHost::IsDefaultSearchProvider(
                                    IsDefaultSearchProviderCallback callback) {
  LOG(ERROR) << "called IsDefaultSearchProvider";
  std::move(callback).Run(false);
}

void BraveSearchDefaultHost::SetIsDefaultSearchProvider() {
  LOG(ERROR) << "called SetIsDefaultSearchProvider";
}

}  // namespace brave_search
