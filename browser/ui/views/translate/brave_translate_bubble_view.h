/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_VIEWS_TRANSLATE_adrbrowsiel_TRANSLATE_BUBBLE_VIEW_H_
#define adrbrowsiel_BROWSER_UI_VIEWS_TRANSLATE_adrbrowsiel_TRANSLATE_BUBBLE_VIEW_H_

#include <memory>
#include <utility>

#include "chrome/browser/ui/views/translate/translate_bubble_view.h"

#include "adrbrowsiel/browser/translate/buildflags/buildflags.h"
#include "extensions/buildflags/buildflags.h"

class adrbrowsielTranslateIconView;

// The purpose of this subclass is to repurpose the translate bubble to install
// google translate extension, this is only used when
// ENABLE_adrbrowsiel_TRANSLATE_EXTENSION is true.
class adrbrowsielTranslateBubbleView : public TranslateBubbleView {
 public:
  adrbrowsielTranslateBubbleView(views::View* anchor_view,
                           std::unique_ptr<TranslateBubbleModel> model,
                           translate::TranslateErrors::Type error_type,
                           content::WebContents* web_contents);
  ~adrbrowsielTranslateBubbleView() override;

  // views::BubbleDialogDelegateView methods.
  void Init() override;

  // views::View methods.
  bool AcceleratorPressed(const ui::Accelerator& accelerator) override;

  // LocationBarBubbleDelegateView methods.
  bool ShouldShowWindowTitle() const override;

 protected:
  virtual void InstallGoogleTranslate();

 private:
  friend class adrbrowsielTranslateBubbleViewTest;
  std::unique_ptr<views::View> adrbrowsielCreateViewBeforeTranslate();
  void DisableOfferTranslatePref();
  void ButtonPressed(ButtonID button_id);

  // Remove this. As we replace |translate_view_|, we should destroy after
  // replacing it. However, its child view(|tabbed_pane_|) is still referenced
  // from TranslateBubbleView. Keep to prevent leak.
  std::unique_ptr<views::View> removed_translate_view_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielTranslateBubbleView);
};

#endif  // adrbrowsiel_BROWSER_UI_VIEWS_TRANSLATE_adrbrowsiel_TRANSLATE_BUBBLE_VIEW_H_
