/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_THIRD_PARTY_BLINK_PUBLIC_PLATFORM_WEB_CONTENT_SETTINGS_CLIENT_H_
#define adrbrowsiel_CHROMIUM_SRC_THIRD_PARTY_BLINK_PUBLIC_PLATFORM_WEB_CONTENT_SETTINGS_CLIENT_H_

#include "adrbrowsiel/third_party/blink/renderer/adrbrowsiel_farbling_constants.h"

#define AllowStorageAccessSync                                    \
  AllowAutoplay(bool play_requested) { return true; }             \
  virtual bool AllowFingerprinting(bool enabled_per_settings) {   \
    return enabled_per_settings;                                  \
  }                                                               \
  virtual adrbrowsielFarblingLevel GetadrbrowsielFarblingLevel() {            \
    return adrbrowsielFarblingLevel::OFF;                               \
  }                                                               \
  virtual bool UseEphemeralStorageSync(StorageType storageType) { \
    return false;                                                 \
  }                                                               \
  virtual bool AllowStorageAccessSync

#include "../../../../../../third_party/blink/public/platform/web_content_settings_client.h"

#undef AllowStorageAccessSync

#endif  // adrbrowsiel_CHROMIUM_SRC_THIRD_PARTY_BLINK_PUBLIC_PLATFORM_WEB_CONTENT_SETTINGS_CLIENT_H_
