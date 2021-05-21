/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_VIEWS_SIDEBAR_SIDEBAR_ITEM_ADD_BUTTON_H_
#define adrbrowsiel_BROWSER_UI_VIEWS_SIDEBAR_SIDEBAR_ITEM_ADD_BUTTON_H_

#include "base/scoped_observation.h"
#include "base/timer/timer.h"
#include "adrbrowsiel/browser/ui/views/sidebar/sidebar_button_view.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_observer.h"

class adrbrowsielBrowser;

class SidebarItemAddButton : public SidebarButtonView,
                             public views::WidgetObserver {
 public:
  explicit SidebarItemAddButton(adrbrowsielBrowser* browser);
  ~SidebarItemAddButton() override;

  SidebarItemAddButton(const SidebarItemAddButton&) = delete;
  SidebarItemAddButton& operator=(const SidebarItemAddButton&) = delete;

  // SidebarButtonView overrides:
  void OnMouseEntered(const ui::MouseEvent& event) override;
  void OnMouseExited(const ui::MouseEvent& event) override;
  void OnGestureEvent(ui::GestureEvent* event) override;

  // views::WidgetObserver overrides:
  void OnWidgetDestroying(views::Widget* widget) override;

  bool IsBubbleVisible() const;

 private:
  void ShowBubbleWithDelay();
  void DoShowBubble();

  adrbrowsielBrowser* browser_;
  base::OneShotTimer timer_;
  base::ScopedObservation<views::Widget, views::WidgetObserver> observation_{
      this};
};

#endif  // adrbrowsiel_BROWSER_UI_VIEWS_SIDEBAR_SIDEBAR_ITEM_ADD_BUTTON_H_
