/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_PRIVATE_CDN_PRIVATE_CDN_HELPER_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_PRIVATE_CDN_PRIVATE_CDN_HELPER_H_

#include <string>

#include "base/memory/singleton.h"
#include "base/strings/string_piece.h"

namespace adrbrowsiel {

class PrivateCdnHelper final {
 public:
  PrivateCdnHelper(const PrivateCdnHelper&) = delete;
  PrivateCdnHelper& operator=(const PrivateCdnHelper&) = delete;

  static PrivateCdnHelper* GetInstance() {
    return base::Singleton<PrivateCdnHelper>::get();
  }

  bool RemovePadding(base::StringPiece* padded_string) const;

 private:
  friend struct base::DefaultSingletonTraits<PrivateCdnHelper>;

  PrivateCdnHelper();
  ~PrivateCdnHelper();
};

}  // namespace adrbrowsiel

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_PRIVATE_CDN_PRIVATE_CDN_HELPER_H_
