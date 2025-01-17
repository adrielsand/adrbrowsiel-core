/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMMON_adrbrowsiel_PATHS_H_
#define adrbrowsiel_COMMON_adrbrowsiel_PATHS_H_

#include "build/build_config.h"

// This file declares path keys for the adrbrowsiel module.  These can be used
// with the PathService to access various special directories and files.

namespace adrbrowsiel {

enum {
  PATH_START = 12000,

  DIR_TEST_DATA,                // Directory where unit test data resides.
  PATH_END
};

// Call once to register the provider for the path keys defined above.
void RegisterPathProvider();

}  // namespace adrbrowsiel

#endif  // adrbrowsiel_COMMON_adrbrowsiel_PATHS_H_
