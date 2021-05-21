/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/extensions/api/webrtc_logging_private/webrtc_logging_private_api.h"

#define adrbrowsiel_DISABLE_RPH_FROM_REQUEST \
    *error = "Unsupported"; return nullptr;

#include "../../../../../../../chrome/browser/extensions/api/webrtc_logging_private/webrtc_logging_private_api.cc"
#undef adrbrowsiel_DISABLE_RPH_FROM_REQUEST
