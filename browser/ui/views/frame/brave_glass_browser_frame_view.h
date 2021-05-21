/* Copyright 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_VIEWS_FRAME_adrbrowsiel_GLASS_BROWSER_FRAME_VIEW_H_
#define adrbrowsiel_BROWSER_UI_VIEWS_FRAME_adrbrowsiel_GLASS_BROWSER_FRAME_VIEW_H_

#include <memory>

#include "chrome/browser/ui/views/frame/glass_browser_frame_view.h"

class adrbrowsielWindowFrameGraphic;

class adrbrowsielGlassBrowserFrameView : public GlassBrowserFrameView {
 public:
  adrbrowsielGlassBrowserFrameView(BrowserFrame* frame, BrowserView* browser_view);
  ~adrbrowsielGlassBrowserFrameView() override;

  adrbrowsielGlassBrowserFrameView(const adrbrowsielGlassBrowserFrameView&) = delete;
  adrbrowsielGlassBrowserFrameView& operator=(
      const adrbrowsielGlassBrowserFrameView&) = delete;

 private:
  // GlassBrowserFrameView overrides:
  void OnPaint(gfx::Canvas* canvas) override;

  std::unique_ptr<adrbrowsielWindowFrameGraphic> frame_graphic_;
};

#endif  // adrbrowsiel_BROWSER_UI_VIEWS_FRAME_adrbrowsiel_GLASS_BROWSER_FRAME_VIEW_H_
