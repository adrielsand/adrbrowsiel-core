/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/views/tabs/adrbrowsiel_browser_tab_strip_controller.h"

#include <utility>

#include "adrbrowsiel/browser/ui/views/tabs/adrbrowsiel_tab_context_menu_contents.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/browser/ui/views/tabs/tab_strip.h"

adrbrowsielBrowserTabStripController::adrbrowsielBrowserTabStripController(
    TabStripModel* model,
    BrowserView* browser_view,
    std::unique_ptr<TabMenuModelFactory> menu_model_factory_override)
    : BrowserTabStripController(model,
                                browser_view,
                                std::move(menu_model_factory_override)) {}

adrbrowsielBrowserTabStripController::~adrbrowsielBrowserTabStripController() {
  if (context_menu_contents_)
    context_menu_contents_->Cancel();
}

void adrbrowsielBrowserTabStripController::ShowContextMenuForTab(
    Tab* tab,
    const gfx::Point& p,
    ui::MenuSourceType source_type) {
  BrowserView* browser_view =
      BrowserView::GetBrowserViewForBrowser(browser());
  context_menu_contents_ = std::make_unique<adrbrowsielTabContextMenuContents>(
      tab,
      this,
      browser_view->tabstrip()->GetModelIndexOf(tab));
  context_menu_contents_->RunMenuAt(p, source_type);
}
