/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_REQUEST_TYPE_H_
#define adrbrowsiel_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_REQUEST_TYPE_H_

#include "build/build_config.h"
#include "third_party/widevine/cdm/buildflags.h"

// clang-format off
#if BUILDFLAG(ENABLE_WIDEVINE)
#define adrbrowsiel_REQUEST_TYPES \
  kWidevine,
#else
#define adrbrowsiel_REQUEST_TYPES
#endif
// clang-format on

#include "../../../../components/permissions/request_type.h"
#undef adrbrowsiel_REQUEST_TYPES

#endif  // adrbrowsiel_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_REQUEST_TYPE_H_
