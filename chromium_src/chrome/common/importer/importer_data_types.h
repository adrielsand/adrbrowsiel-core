/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_CHROME_COMMON_IMPORTER_IMPORTER_DATA_TYPES_H_
#define adrbrowsiel_CHROMIUM_SRC_CHROME_COMMON_IMPORTER_IMPORTER_DATA_TYPES_H_

#define adrbrowsiel_IMPORT_ITEM \
  EXTENSIONS         = 1 << 7, \
  PAYMENTS           = 1 << 8, \
  ALL                = (1 << 9) - 1  // All the bits should be 1, hence the -1.

#define adrbrowsiel_VISIT_SOURCE \
  VISIT_SOURCE_CHROME_IMPORTED = 4,

#include "../../../../../chrome/common/importer/importer_data_types.h"

#undef adrbrowsiel_VISIT_SOURCE

#endif  // adrbrowsiel_CHROMIUM_SRC_CHROME_COMMON_IMPORTER_IMPORTER_DATA_TYPES_H_
