/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_VIEWS_PROFILES_adrbrowsiel_INCOGNITO_MENU_VIEW_H_
#define adrbrowsiel_BROWSER_UI_VIEWS_PROFILES_adrbrowsiel_INCOGNITO_MENU_VIEW_H_

#include "chrome/browser/ui/views/profiles/incognito_menu_view.h"

class adrbrowsielIncognitoMenuView : public IncognitoMenuView {
 public:
  using IncognitoMenuView::IncognitoMenuView;
  ~adrbrowsielIncognitoMenuView() override = default;

  // ProfileMenuViewBase:
  void BuildMenu() override;

 private:
  friend class IncognitoMenuView;

  // views::BubbleDialogDelegateView:
  std::u16string GetAccessibleWindowTitle() const override;

  // Button actions.
  void OnExitButtonClicked() override;

  void AddTorButton();
  void OnTorProfileButtonClicked();

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielIncognitoMenuView);
};

#endif  // adrbrowsiel_BROWSER_UI_VIEWS_PROFILES_adrbrowsiel_INCOGNITO_MENU_VIEW_H_
