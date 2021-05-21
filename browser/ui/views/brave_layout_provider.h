/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_VIEWS_adrbrowsiel_LAYOUT_PROVIDER_H_
#define adrbrowsiel_BROWSER_UI_VIEWS_adrbrowsiel_LAYOUT_PROVIDER_H_

#include "base/macros.h"
#include "chrome/browser/ui/views/chrome_layout_provider.h"

class adrbrowsielLayoutProvider : public ChromeLayoutProvider {
 public:
  adrbrowsielLayoutProvider() = default;
  ~adrbrowsielLayoutProvider() override = default;

  int GetCornerRadiusMetric(views::Emphasis emphasis,
                            const gfx::Size& size = gfx::Size()) const override;

 private:
  DISALLOW_COPY_AND_ASSIGN(adrbrowsielLayoutProvider);
};

#endif  // adrbrowsiel_BROWSER_UI_VIEWS_adrbrowsiel_LAYOUT_PROVIDER_H_
