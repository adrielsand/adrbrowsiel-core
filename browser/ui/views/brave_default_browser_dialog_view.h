/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_VIEWS_adrbrowsiel_DEFAULT_BROWSER_DIALOG_VIEW_H_
#define adrbrowsiel_BROWSER_UI_VIEWS_adrbrowsiel_DEFAULT_BROWSER_DIALOG_VIEW_H_

#include <memory>

#include "ui/views/window/dialog_delegate.h"

namespace views {
class Checkbox;
class Label;
}  // namespace views

class adrbrowsielDefaultBrowserDialogView : public views::DialogDelegateView {
 public:
  adrbrowsielDefaultBrowserDialogView();
  ~adrbrowsielDefaultBrowserDialogView() override;

  adrbrowsielDefaultBrowserDialogView(const adrbrowsielDefaultBrowserDialogView&) = delete;
  adrbrowsielDefaultBrowserDialogView& operator=(
      const adrbrowsielDefaultBrowserDialogView&) = delete;

  // views::DialogDelegateView overrides:
  ui::ModalType GetModalType() const override;
  bool ShouldShowCloseButton() const override;
  std::unique_ptr<views::NonClientFrameView> CreateNonClientFrameView(
      views::Widget* widget) override;
  void OnDialogInitialized() override;

 private:
  void OnCancelButtonClicked();
  void OnAcceptButtonClicked();
  void CreateChildViews();

  views::Label* header_label_ = nullptr;
  views::Label* contents_label_ = nullptr;
  views::Checkbox* dont_ask_again_checkbox_ = nullptr;
};

#endif  // adrbrowsiel_BROWSER_UI_VIEWS_adrbrowsiel_DEFAULT_BROWSER_DIALOG_VIEW_H_
