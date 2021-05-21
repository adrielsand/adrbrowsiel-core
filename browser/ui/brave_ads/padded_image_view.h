/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_adrbrowsiel_ADS_PADDED_IMAGE_VIEW_H_
#define adrbrowsiel_BROWSER_UI_adrbrowsiel_ADS_PADDED_IMAGE_VIEW_H_

#include "ui/views/controls/image_view.h"
#include "ui/views/metadata/metadata_header_macros.h"

namespace adrbrowsiel_ads {

class PaddedImageView : public views::ImageView {
 public:
  METADATA_HEADER(PaddedImageView);

  PaddedImageView();
  ~PaddedImageView() override = default;

 private:
  PaddedImageView(const PaddedImageView&) = delete;
  PaddedImageView& operator=(const PaddedImageView&) = delete;
};

}  // namespace adrbrowsiel_ads

#endif  // adrbrowsiel_BROWSER_UI_adrbrowsiel_ADS_PADDED_IMAGE_VIEW_H_
