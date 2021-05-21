/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_CONTENT_SETTINGS_CONTENT_SETTING_IMAGE_MODEL_H_
#define adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_CONTENT_SETTINGS_CONTENT_SETTING_IMAGE_MODEL_H_

#define GenerateContentSettingImageModels                       \
  GenerateContentSettingImageModels_ChromiumImpl();             \
  static std::vector<std::unique_ptr<ContentSettingImageModel>> \
      GenerateContentSettingImageModels

#include "../../../../../../chrome/browser/ui/content_settings/content_setting_image_model.h"
#undef GenerateContentSettingImageModels

#endif  // adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_CONTENT_SETTINGS_CONTENT_SETTING_IMAGE_MODEL_H_
