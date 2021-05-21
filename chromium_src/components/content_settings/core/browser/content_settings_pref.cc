/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "build/build_config.h"

#if !defined(OS_IOS)
#include "adrbrowsiel/components/content_settings/core/browser/adrbrowsiel_content_settings_utils.h"

#define adrbrowsiel_SET_WEBSITE_SETTING \
  content_settings::IsShieldsContentSettingsType(content_type_) ||
#else
#define adrbrowsiel_SET_WEBSITE_SETTING
#endif

#include "../../../../../../components/content_settings/core/browser/content_settings_pref.cc"

#undef adrbrowsiel_SET_WEBSITE_SETTING
