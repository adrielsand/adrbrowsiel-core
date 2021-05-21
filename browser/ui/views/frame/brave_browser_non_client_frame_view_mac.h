/* Copyright 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_VIEWS_FRAME_adrbrowsiel_BROWSER_NON_CLIENT_FRAME_VIEW_MAC_H_
#define adrbrowsiel_BROWSER_UI_VIEWS_FRAME_adrbrowsiel_BROWSER_NON_CLIENT_FRAME_VIEW_MAC_H_

#include <memory>

#include "chrome/browser/ui/views/frame/browser_non_client_frame_view_mac.h"

class adrbrowsielWindowFrameGraphic;

class adrbrowsielBrowserNonClientFrameViewMac : public BrowserNonClientFrameViewMac {
 public:
  adrbrowsielBrowserNonClientFrameViewMac(BrowserFrame* frame,
                                    BrowserView* browser_view);
  ~adrbrowsielBrowserNonClientFrameViewMac() override;

  adrbrowsielBrowserNonClientFrameViewMac(
      const adrbrowsielBrowserNonClientFrameViewMac&) = delete;
  adrbrowsielBrowserNonClientFrameViewMac& operator=(
      const adrbrowsielBrowserNonClientFrameViewMac&) = delete;

 private:
  // BrowserNonClientFrameViewMac overrides:
  void OnPaint(gfx::Canvas* canvas) override;

  std::unique_ptr<adrbrowsielWindowFrameGraphic> frame_graphic_;
};

#endif  // adrbrowsiel_BROWSER_UI_VIEWS_FRAME_adrbrowsiel_BROWSER_NON_CLIENT_FRAME_VIEW_MAC_H_
