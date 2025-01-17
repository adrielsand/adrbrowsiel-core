/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_VIEWS_SIDEBAR_SIDEBAR_SHOW_OPTIONS_EVENT_DETECT_WIDGET_H_
#define adrbrowsiel_BROWSER_UI_VIEWS_SIDEBAR_SIDEBAR_SHOW_OPTIONS_EVENT_DETECT_WIDGET_H_

#include <memory>

#include "base/scoped_observation.h"
#include "ui/views/widget/widget_delegate.h"

namespace views {
class Widget;
}  // namespace views

class adrbrowsielBrowserView;

class SidebarShowOptionsEventDetectWidget : public views::ViewObserver,
                                            public views::WidgetDelegate {
 public:
  class Delegate {
   public:
    virtual void ShowSidebar() = 0;

   protected:
    virtual ~Delegate() {}
  };

  explicit SidebarShowOptionsEventDetectWidget(adrbrowsielBrowserView* browser_view,
                                               Delegate* delegate);
  ~SidebarShowOptionsEventDetectWidget() override;

  SidebarShowOptionsEventDetectWidget(
      const SidebarShowOptionsEventDetectWidget&) = delete;
  SidebarShowOptionsEventDetectWidget& operator=(
      const SidebarShowOptionsEventDetectWidget&) = delete;

  void Show();
  void Hide();

  void SetShowOnHover(bool show_on_hover);

  // views::ViewObserver overrides:
  void OnViewBoundsChanged(views::View* observed_view) override;

 private:
  class ContentsView;

  std::unique_ptr<views::Widget> CreateWidget(Delegate* delegate);
  void AdjustWidgetBounds();

  adrbrowsielBrowserView* browser_view_ = nullptr;
  ContentsView* contents_view_ = nullptr;
  std::unique_ptr<views::Widget> widget_;
  base::ScopedObservation<views::View, views::ViewObserver> observation_{this};
};

#endif  // adrbrowsiel_BROWSER_UI_VIEWS_SIDEBAR_SIDEBAR_SHOW_OPTIONS_EVENT_DETECT_WIDGET_H_
