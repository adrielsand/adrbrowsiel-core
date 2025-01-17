/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_VIEWS_TRANSLATE_adrbrowsiel_TRANSLATE_ICON_VIEW_H_
#define adrbrowsiel_BROWSER_UI_VIEWS_TRANSLATE_adrbrowsiel_TRANSLATE_ICON_VIEW_H_

#include <string>

#include "chrome/browser/ui/views/translate/translate_icon_view.h"

#include "base/memory/weak_ptr.h"
#include "adrbrowsiel/browser/translate/buildflags/buildflags.h"
#include "chrome/common/extensions/webstore_install_result.h"
#include "extensions/buildflags/buildflags.h"

// The purpose of this subclass is to hide translate icon and translate bubble
// when Google Translate extension is installed. This subclass also takes care
// of installing Google Translate extension upon requests from translate
// bubble, and updates both UI based on the installation result. This subclass
// is only used when ENABLE_adrbrowsiel_TRANSLATE_EXTENSION is true.
class adrbrowsielTranslateIconView : public TranslateIconView {
 public:
  adrbrowsielTranslateIconView(
      CommandUpdater* command_updater,
      IconLabelBubbleView::Delegate* icon_label_bubble_delegate,
      PageActionIconView::Delegate* page_action_icon_delegate);
  ~adrbrowsielTranslateIconView() override;

  void UpdateImpl() override;

  void InstallGoogleTranslate();

 private:
  void OnInstallResult(bool success,
                       const std::string& error,
                       extensions::webstore_install::Result result);

  base::WeakPtrFactory<adrbrowsielTranslateIconView> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielTranslateIconView);
};

#endif  // adrbrowsiel_BROWSER_UI_VIEWS_TRANSLATE_adrbrowsiel_TRANSLATE_ICON_VIEW_H_
