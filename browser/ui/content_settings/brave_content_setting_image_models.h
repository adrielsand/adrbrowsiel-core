/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_CONTENT_SETTINGS_adrbrowsiel_CONTENT_SETTING_IMAGE_MODELS_H_
#define adrbrowsiel_BROWSER_UI_CONTENT_SETTINGS_adrbrowsiel_CONTENT_SETTING_IMAGE_MODELS_H_

#include <memory>
#include <vector>

class ContentSettingImageModel;

void adrbrowsielGenerateContentSettingImageModels(
    std::vector<std::unique_ptr<ContentSettingImageModel>>*);

#endif  // adrbrowsiel_BROWSER_UI_CONTENT_SETTINGS_adrbrowsiel_CONTENT_SETTING_IMAGE_MODELS_H_
