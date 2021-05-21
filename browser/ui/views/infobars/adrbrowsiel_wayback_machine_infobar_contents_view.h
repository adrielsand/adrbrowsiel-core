/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_VIEWS_INFOBARS_adrbrowsiel_WAYBACK_MACHINE_INFOBAR_CONTENTS_VIEW_H_
#define adrbrowsiel_BROWSER_UI_VIEWS_INFOBARS_adrbrowsiel_WAYBACK_MACHINE_INFOBAR_CONTENTS_VIEW_H_

#include <vector>

#include "adrbrowsiel/components/adrbrowsiel_wayback_machine/wayback_machine_url_fetcher.h"
#include "ui/views/view.h"

namespace content {
class WebContents;
}  // namespace content

namespace views {
class ImageView;
class Label;
}  // namespace views

class adrbrowsielWaybackMachineInfoBarButtonContainer;
class GURL;

// Includes all view controls except close button that managed by InfoBarView.
class adrbrowsielWaybackMachineInfoBarContentsView
    : public views::View,
      public WaybackMachineURLFetcher::Client {
 public:
  explicit adrbrowsielWaybackMachineInfoBarContentsView(
      content::WebContents* contents);
  ~adrbrowsielWaybackMachineInfoBarContentsView() override;

  adrbrowsielWaybackMachineInfoBarContentsView(
      const adrbrowsielWaybackMachineInfoBarContentsView&) = delete;
  adrbrowsielWaybackMachineInfoBarContentsView& operator=(
      const adrbrowsielWaybackMachineInfoBarContentsView&) = delete;

 private:
  using Labels = std::vector<views::Label*>;
  using Views = std::vector<views::View*>;

  // views::View overrides:
  void OnThemeChanged() override;

  // WaybackMachineURLFetcher::Client overrides:
  void OnWaybackURLFetched(const GURL& latest_wayback_url) override;

  void InitializeChildren();
  views::Label* CreateLabel(const std::u16string& text);
  SkColor GetColor(int id) const;
  void UpdateChildrenVisibility(bool show_before_checking_views);
  void FetchWaybackURL();
  void LoadURL(const GURL& url);
  void HideInfobar();

  void ButtonPressed();

  // Used for labels theme changing all together.
  Labels labels_;
  Views views_visible_before_checking_;
  Views views_visible_after_checking_;
  content::WebContents* contents_;
  WaybackMachineURLFetcher wayback_machine_url_fetcher_;

  adrbrowsielWaybackMachineInfoBarButtonContainer* button_ = nullptr;
  views::ImageView* wayback_spot_graphic_ = nullptr;
  bool wayback_url_fetch_requested_ = false;
};

#endif  // adrbrowsiel_BROWSER_UI_VIEWS_INFOBARS_adrbrowsiel_WAYBACK_MACHINE_INFOBAR_CONTENTS_VIEW_H_
