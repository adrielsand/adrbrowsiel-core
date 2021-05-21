/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "third_party/blink/public/platform/web_content_settings_client.h"
#include "third_party/blink/renderer/core/execution_context/execution_context.h"

#define adrbrowsiel_CANVAS_ASYNC_BLOB_CREATOR                                \
  if (WebContentSettingsClient* settings =                             \
          adrbrowsiel::GetContentSettingsClientFor(context_)) {              \
    adrbrowsiel::adrbrowsielSessionCache::From(*context_).PerturbPixels(           \
        settings, static_cast<const unsigned char*>(src_data_.addr()), \
        src_data_.computeByteSize());                                  \
  }

#include "../../../../../../../../third_party/blink/renderer/core/html/canvas/canvas_async_blob_creator.cc"

#undef adrbrowsiel_CANVAS_ASYNC_BLOB_CREATOR
