/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_COMPONENTS_SYNC_DEVICE_INFO_DEVICE_INFO_H_
#define adrbrowsiel_CHROMIUM_SRC_COMPONENTS_SYNC_DEVICE_INFO_DEVICE_INFO_H_

#include <string>

#define ToValue \
  ToValue_ChromiumImpl() const; \
  std::unique_ptr<base::DictionaryValue> ToValue

#include "../../../../components/sync_device_info/device_info.h"
#undef ToValue

#endif  // adrbrowsiel_CHROMIUM_SRC_COMPONENTS_SYNC_DEVICE_INFO_DEVICE_INFO_H_
