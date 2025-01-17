/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "components/permissions/request_type.h"

#define adrbrowsiel_GET_ICON_ID_DESKTOP \
  case RequestType::kWidevine:    \
    return vector_icons::kExtensionIcon;

#if BUILDFLAG(ENABLE_WIDEVINE)
#define adrbrowsiel_PERMISSION_KEY_FOR_REQUEST_TYPE \
  case permissions::RequestType::kWidevine:   \
    return "widevine";
#else
#define adrbrowsiel_PERMISSION_KEY_FOR_REQUEST_TYPE
#endif

#include "../../../../components/permissions/request_type.cc"

#undef adrbrowsiel_PERMISSION_KEY_FOR_REQUEST_TYPE
#undef adrbrowsiel_GET_ICON_ID_DESKTOP
