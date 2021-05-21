/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_VIEWS_INFOBARS_adrbrowsiel_WAYBACK_MACHINE_INFOBAR_BUTTON_CONTAINER_H_
#define adrbrowsiel_BROWSER_UI_VIEWS_INFOBARS_adrbrowsiel_WAYBACK_MACHINE_INFOBAR_BUTTON_CONTAINER_H_

#include "ui/views/view.h"
#include "ui/views/controls/button/button.h"

class adrbrowsielWaybackMachineInfoBarThrobber;

// This manages button and throbber controls.
// buttons occupies all this containers area and throbber runs over the button.
// When throbbing is requested, button extends its right inset and throbber runs
// on that area.
class adrbrowsielWaybackMachineInfoBarButtonContainer : public views::View {
 public:
  explicit adrbrowsielWaybackMachineInfoBarButtonContainer(
      views::Button::PressedCallback callback);
  ~adrbrowsielWaybackMachineInfoBarButtonContainer() override;

  adrbrowsielWaybackMachineInfoBarButtonContainer(
      const adrbrowsielWaybackMachineInfoBarButtonContainer&) = delete;
  adrbrowsielWaybackMachineInfoBarButtonContainer& operator=(
      const adrbrowsielWaybackMachineInfoBarButtonContainer&) = delete;

  void StartThrobber();
  void StopThrobber();

  // views::View overrides:
  void Layout() override;
  gfx::Size CalculatePreferredSize() const override;

 private:
  void AdjustButtonInsets(bool add_insets);

  adrbrowsielWaybackMachineInfoBarThrobber* throbber_ = nullptr;
  views::View* button_ = nullptr;
};

#endif  // adrbrowsiel_BROWSER_UI_VIEWS_INFOBARS_adrbrowsiel_WAYBACK_MACHINE_INFOBAR_BUTTON_CONTAINER_H_
