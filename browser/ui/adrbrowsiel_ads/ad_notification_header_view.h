/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_adrbrowsiel_ADS_AD_NOTIFICATION_HEADER_VIEW_H_
#define adrbrowsiel_BROWSER_UI_adrbrowsiel_ADS_AD_NOTIFICATION_HEADER_VIEW_H_

#include "ui/gfx/text_constants.h"
#include "ui/views/metadata/metadata_header_macros.h"
#include "ui/views/view.h"

namespace views {
class Label;
}  // namespace views

namespace adrbrowsiel_ads {

class AdNotificationHeaderView : public views::View {
 public:
  METADATA_HEADER(AdNotificationHeaderView);

  explicit AdNotificationHeaderView(const int width);
  ~AdNotificationHeaderView() override;

  void SetTitle(const std::u16string& name);
  void SetTitleElideBehavior(gfx::ElideBehavior elide_behavior);

  void UpdateContent();

  // views::View:
  void GetAccessibleNodeData(ui::AXNodeData* node_data) override;
  void OnThemeChanged() override;

 private:
  void CreateView(const int width);

  views::Label* title_label_ = nullptr;
  views::Label* CreateTitleLabel();
  void UpdateTitleLabel();

  AdNotificationHeaderView(const AdNotificationHeaderView&) = delete;
  AdNotificationHeaderView& operator=(const AdNotificationHeaderView&) = delete;
};

}  // namespace adrbrowsiel_ads

#endif  // adrbrowsiel_BROWSER_UI_adrbrowsiel_ADS_AD_NOTIFICATION_HEADER_VIEW_H_
