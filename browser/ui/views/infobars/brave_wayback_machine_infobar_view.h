/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_VIEWS_INFOBARS_adrbrowsiel_WAYBACK_MACHINE_INFOBAR_VIEW_H_
#define adrbrowsiel_BROWSER_UI_VIEWS_INFOBARS_adrbrowsiel_WAYBACK_MACHINE_INFOBAR_VIEW_H_

#include <memory>

#include "chrome/browser/ui/views/infobars/infobar_view.h"

namespace content {
class WebContents;
}

class adrbrowsielWaybackMachineInfoBarDelegate;

class adrbrowsielWaybackMachineInfoBarView : public InfoBarView {
 public:
  adrbrowsielWaybackMachineInfoBarView(
      std::unique_ptr<adrbrowsielWaybackMachineInfoBarDelegate> delegate,
      content::WebContents* contents);
  ~adrbrowsielWaybackMachineInfoBarView() override;

  adrbrowsielWaybackMachineInfoBarView(
      const adrbrowsielWaybackMachineInfoBarView&) = delete;
  adrbrowsielWaybackMachineInfoBarView& operator=(
      const adrbrowsielWaybackMachineInfoBarView&) = delete;

 private:
  // InfoBarView overrides:
  void Layout() override;

  views::View* sub_views_ = nullptr;
};

#endif  // adrbrowsiel_BROWSER_UI_VIEWS_INFOBARS_adrbrowsiel_WAYBACK_MACHINE_INFOBAR_VIEW_H_
