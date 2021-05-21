/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_adrbrowsiel_ADS_PADDED_IMAGE_BUTTON_H_
#define adrbrowsiel_BROWSER_UI_adrbrowsiel_ADS_PADDED_IMAGE_BUTTON_H_

#include <memory>

#include "ui/views/controls/button/image_button.h"
#include "ui/views/metadata/metadata_header_macros.h"

namespace views {
class InkDrop;
}  // namespace views

namespace adrbrowsiel_ads {

// PaddedImageButtons are ImageButtons whose image can be padded within the
// button. This allows the creation of buttons whose clickable areas extend
// beyond their image areas without the need to create and maintain
// corresponding resource images with alpha padding
class PaddedImageButton : public views::ImageButton {
 public:
  METADATA_HEADER(PaddedImageButton);

  explicit PaddedImageButton(PressedCallback callback);
  ~PaddedImageButton() override = default;

  void AdjustBorderInsetToFitHeight(const int height);

  // views::InkDropHostView:
  std::unique_ptr<views::InkDrop> CreateInkDrop() override;

  // views::Button:
  void OnThemeChanged() override;

 private:
  PaddedImageButton(const PaddedImageButton&) = delete;
  PaddedImageButton& operator=(const PaddedImageButton&) = delete;
};

}  // namespace adrbrowsiel_ads

#endif  // adrbrowsiel_BROWSER_UI_adrbrowsiel_ADS_PADDED_IMAGE_BUTTON_H_
