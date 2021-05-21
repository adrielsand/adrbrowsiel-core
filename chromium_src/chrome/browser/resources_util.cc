/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/grit/adrbrowsiel_theme_resources_map.h"

#define adrbrowsiel_RESOURCES_UTIL                              \
  for (size_t i = 0; i < kadrbrowsielThemeResourcesSize; ++i) { \
    storage.emplace_back(kadrbrowsielThemeResources[i].path,    \
                         kadrbrowsielThemeResources[i].id);     \
  }

#include "../../../../chrome/browser/resources_util.cc"
#undef adrbrowsiel_RESOURCES_UTIL
