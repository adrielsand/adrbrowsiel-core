/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/adrbrowsiel_ads/padded_image_button.h"

#include <algorithm>
#include <utility>

#include "ui/gfx/color_palette.h"
#include "ui/gfx/color_utils.h"
#include "ui/gfx/geometry/insets.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/views/animation/ink_drop.h"
#include "ui/views/animation/ink_drop_impl.h"
#include "ui/views/background.h"
#include "ui/views/border.h"
#include "ui/views/metadata/metadata_impl_macros.h"

namespace adrbrowsiel_ads {

namespace {

constexpr SkColor kBackgroundColor = SK_ColorTRANSPARENT;

constexpr gfx::Insets kBorderInset(
    /* top */ 4,
    /* left */ 4,
    /* bottom */ 0,
    /* right */ 4);

const float kVisibleOpacity = 0.12f;

}  // namespace

PaddedImageButton::PaddedImageButton(PressedCallback callback)
    : views::ImageButton(callback) {
  SetBackground(views::CreateSolidBackground(kBackgroundColor));

  SetBorder(views::CreateEmptyBorder(kBorderInset));

  SetAnimateOnStateChange(false);

  SetInkDropMode(InkDropMode::ON);
  SetInkDropVisibleOpacity(kVisibleOpacity);
  SetHasInkDropActionOnClick(true);
}

void PaddedImageButton::AdjustBorderInsetToFitHeight(const int height) {
  views::Border* border = GetBorder();
  DCHECK(border);

  const gfx::ImageSkia& image = GetImage(views::Button::STATE_NORMAL);

  gfx::Insets insets = border->GetInsets();
  const int inset = std::max(0, height - insets.height() - image.height());

  insets += gfx::Insets(0, 0, /* bottom */ inset, 0);
  SetBorder(views::CreateEmptyBorder(insets));
}

std::unique_ptr<views::InkDrop> PaddedImageButton::CreateInkDrop() {
  std::unique_ptr<views::InkDropImpl> ink_drop = CreateDefaultInkDropImpl();
  ink_drop->SetShowHighlightOnHover(false);
  ink_drop->SetShowHighlightOnFocus(false);
  return std::move(ink_drop);
}

void PaddedImageButton::OnThemeChanged() {
  ImageButton::OnThemeChanged();

  const SkColor background_color = GetNativeTheme()->GetSystemColor(
      ui::NativeTheme::kColorId_WindowBackground);
  SetInkDropBaseColor(color_utils::GetColorWithMaxContrast(background_color));
}

BEGIN_METADATA(PaddedImageButton, views::ImageButton)
END_METADATA

}  // namespace adrbrowsiel_ads
