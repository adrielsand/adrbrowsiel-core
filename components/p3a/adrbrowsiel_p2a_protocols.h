/* Copyright 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_P3A_adrbrowsiel_P2A_PROTOCOLS_H_
#define adrbrowsiel_COMPONENTS_P3A_adrbrowsiel_P2A_PROTOCOLS_H_

#include <stdint.h>

namespace adrbrowsiel {

class DirectEncodingProtocol {
 public:
  DirectEncodingProtocol();
  ~DirectEncodingProtocol();

  static uint64_t Perturb(uint16_t bucket_count, uint64_t value);
};

}  // namespace adrbrowsiel

#endif  // adrbrowsiel_COMPONENTS_P3A_adrbrowsiel_P2A_PROTOCOLS_H_
