/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/adrbrowsiel_ads/padded_image_view.h"

#include "ui/gfx/color_palette.h"
#include "ui/gfx/geometry/insets.h"
#include "ui/views/background.h"
#include "ui/views/border.h"
#include "ui/views/metadata/metadata_impl_macros.h"

namespace adrbrowsiel_ads {

namespace {

constexpr SkColor kBackgroundColor = SK_ColorTRANSPARENT;

constexpr gfx::Insets kBorderInset(
    /* top */ 2,
    /* left */ 6,
    /* bottom */ 0,
    /* right */ 6);

}  // namespace

PaddedImageView::PaddedImageView() : views::ImageView() {
  SetBackground(views::CreateSolidBackground(kBackgroundColor));

  SetBorder(views::CreateEmptyBorder(kBorderInset));
}

BEGIN_METADATA(PaddedImageView, views::ImageView)
END_METADATA

}  // namespace adrbrowsiel_ads
