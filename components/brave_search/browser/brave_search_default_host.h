/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_SEARCH_BROWSER_BRAVE_SEARCH_DEFAULT_HOST_H_
#define BRAVE_COMPONENTS_BRAVE_SEARCH_BROWSER_BRAVE_SEARCH_DEFAULT_HOST_H_

#include <memory>

#include "base/memory/weak_ptr.h"
#include "brave/components/brave_search/common/brave_search_default.mojom.h"

namespace brave_search {

class BraveSearchDefaultHost final :
                              public brave_search::mojom::BraveSearchDefault {
 public:
  BraveSearchDefaultHost(const BraveSearchDefaultHost&) = delete;
  BraveSearchDefaultHost& operator=(const BraveSearchDefaultHost&) = delete;
  explicit BraveSearchDefaultHost();
  ~BraveSearchDefaultHost() override;

  // brave_search::mojom::BraveSearchDefault:
  void IsDefaultSearchProvider(
                          IsDefaultSearchProviderCallback callback) override;
  void SetIsDefaultSearchProvider() override;

 private:
  base::WeakPtrFactory<BraveSearchDefaultHost> weak_factory_;
};

}  // namespace brave_search

#endif  // BRAVE_COMPONENTS_BRAVE_SEARCH_BROWSER_BRAVE_SEARCH_DEFAULT_HOST_H_
