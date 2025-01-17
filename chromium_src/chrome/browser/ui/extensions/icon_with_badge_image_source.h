/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_EXTENSIONS_ICON_WITH_BADGE_IMAGE_SOURCE_H_
#define adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_EXTENSIONS_ICON_WITH_BADGE_IMAGE_SOURCE_H_

class adrbrowsielActionIconWithBadgeImageSource;

#define adrbrowsiel_ICON_WITH_BADGE_IMAGE_SOURCE_H_            \
 private:                                                \
  friend class adrbrowsielActionIconWithBadgeImageSource;      \
  virtual base::Optional<int> GetCustomGraphicSize();    \
  virtual base::Optional<int> GetCustomGraphicXOffset(); \
  virtual base::Optional<int> GetCustomGraphicYOffset(); \
                                                         \
 public:                                                 \
// #define adrbrowsiel_ICON_WITH_BADGE_IMAGE_SOURCE_H_

#define PaintBadge virtual PaintBadge
#define GetIconAreaRect virtual GetIconAreaRect
#include "../../../../../../chrome/browser/ui/extensions/icon_with_badge_image_source.h"
#undef GetIconAreaRect
#undef PaintBadge
#undef adrbrowsiel_ICON_WITH_BADGE_IMAGE_SOURCE_H_

#endif  // adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_EXTENSIONS_ICON_WITH_BADGE_IMAGE_SOURCE_H_
