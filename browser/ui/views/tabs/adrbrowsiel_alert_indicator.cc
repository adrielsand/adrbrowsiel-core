/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/views/tabs/adrbrowsiel_alert_indicator.h"

#include <memory>
#include <string>

#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/ui/views/tabs/browser_tab_strip_controller.h"
#include "chrome/browser/ui/views/tabs/tab_controller.h"
#include "chrome/browser/ui/views/tabs/tab_strip.h"
#include "chrome/browser/ui/views/tabs/tab_strip_controller.h"
#include "chrome/browser/ui/views/tabs/tab_style_views.h"
#include "content/public/browser/web_contents.h"
#include "third_party/skia/include/core/SkPathTypes.h"
#include "ui/gfx/color_utils.h"
#include "ui/views/background.h"

namespace {

bool IsAudioState(const base::Optional<TabAlertState>& state) {
  return (state.has_value() && (state.value() == TabAlertState::AUDIO_PLAYING ||
                                state.value() == TabAlertState::AUDIO_MUTING));
}

}  // namespace

class adrbrowsielAlertIndicator::adrbrowsielAlertBackground : public views::Background {
 public:
  explicit adrbrowsielAlertBackground(adrbrowsielAlertIndicator* host_view)
    : host_view_(host_view) {
  }

  // views::Background overrides:
  void Paint(gfx::Canvas* canvas, views::View* view) const override {
    if (!host_view_->IsTabAudioToggleable())
      return;

    gfx::Point center = host_view_->GetContentsBounds().CenterPoint();
    SkPath path;
    path.setFillType(SkPathFillType::kEvenOdd);
    path.addCircle(center.x(), center.y(), host_view_->width() / 2);
    cc::PaintFlags flags;
    flags.setAntiAlias(true);
    flags.setColor(host_view_->GetBackgroundColor());
    canvas->DrawPath(path, flags);
  }

 private:
  adrbrowsielAlertIndicator* host_view_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielAlertBackground);
};

adrbrowsielAlertIndicator::adrbrowsielAlertIndicator(Tab* parent_tab)
    : AlertIndicator(parent_tab) {
  SetBackground(std::make_unique<adrbrowsielAlertBackground>(this));
}

SkColor adrbrowsielAlertIndicator::GetBackgroundColor() const {
  TabStyle::TabColors colors = parent_tab_->tab_style()->CalculateColors();
  if (!IsTabAudioToggleable() || !IsMouseHovered())
    return colors.background_color;

  // Approximating the InkDrop behavior of the close button.
  return color_utils::BlendTowardMaxContrast(colors.background_color,
                                             mouse_pressed_ ? 72 : 36);
}

bool adrbrowsielAlertIndicator::OnMousePressed(const ui::MouseEvent& event) {
  mouse_pressed_ = true;
  SchedulePaint();

  if (!IsTabAudioToggleable())
    return AlertIndicator::OnMousePressed(event);

  return true;
}

void adrbrowsielAlertIndicator::OnMouseReleased(const ui::MouseEvent& event) {
  mouse_pressed_ = false;
  SchedulePaint();

  if (!IsTabAudioToggleable() || !IsMouseHovered())
    return AlertIndicator::OnMouseReleased(event);

  auto* tab_strip = static_cast<TabStrip*>(parent_tab_->controller());
  const int tab_index = tab_strip->GetModelIndexOf(parent_tab_);
  if (tab_index == -1)
    return;
  auto* tab_strip_model = static_cast<BrowserTabStripController*>(
      tab_strip->controller())->model();
  auto* web_contents = tab_strip_model->GetWebContentsAt(tab_index);
  if (web_contents == nullptr)
    return;
  chrome::SetTabAudioMuted(web_contents,
                           !web_contents->IsAudioMuted(),
                           TabMutedReason::CONTEXT_MENU,
                           std::string());
}

void adrbrowsielAlertIndicator::OnMouseEntered(const ui::MouseEvent& event) {
  if (IsTabAudioToggleable())
    SchedulePaint();
  AlertIndicator::OnMouseExited(event);
}

void adrbrowsielAlertIndicator::OnMouseExited(const ui::MouseEvent& event) {
  if (IsTabAudioToggleable())
    SchedulePaint();
  AlertIndicator::OnMouseExited(event);
}

bool adrbrowsielAlertIndicator::OnMouseDragged(const ui::MouseEvent& event) {
  if (IsTabAudioToggleable())
    SchedulePaint();
  return AlertIndicator::OnMouseDragged(event);
}

bool adrbrowsielAlertIndicator::IsTabAudioToggleable() const {
  if (parent_tab_->controller()->IsTabPinned(parent_tab_))
    return false;

  return IsAudioState(alert_state_);
}
