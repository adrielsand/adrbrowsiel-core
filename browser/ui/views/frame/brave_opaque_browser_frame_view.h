/* Copyright 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_VIEWS_FRAME_adrbrowsiel_OPAQUE_BROWSER_FRAME_VIEW_H_
#define adrbrowsiel_BROWSER_UI_VIEWS_FRAME_adrbrowsiel_OPAQUE_BROWSER_FRAME_VIEW_H_

#include <memory>

#include "chrome/browser/ui/views/frame/opaque_browser_frame_view.h"

class adrbrowsielWindowFrameGraphic;

class adrbrowsielOpaqueBrowserFrameView : public OpaqueBrowserFrameView {
 public:
  adrbrowsielOpaqueBrowserFrameView(BrowserFrame* frame,
                              BrowserView* browser_view,
                              OpaqueBrowserFrameViewLayout* layout);
  ~adrbrowsielOpaqueBrowserFrameView() override;

  adrbrowsielOpaqueBrowserFrameView(const adrbrowsielOpaqueBrowserFrameView&) = delete;
  adrbrowsielOpaqueBrowserFrameView& operator=(
      const adrbrowsielOpaqueBrowserFrameView&) = delete;

  // adrbrowsielOpaqueBrowserFrameView overrides:
  void OnPaint(gfx::Canvas* canvas) override;

 private:
  std::unique_ptr<adrbrowsielWindowFrameGraphic> frame_graphic_;
};

#endif  // adrbrowsiel_BROWSER_UI_VIEWS_FRAME_adrbrowsiel_OPAQUE_BROWSER_FRAME_VIEW_H_
