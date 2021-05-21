/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_adrbrowsiel_BROWSER_CONTENT_SETTING_BUBBLE_MODEL_DELEGATE_H_
#define adrbrowsiel_BROWSER_UI_adrbrowsiel_BROWSER_CONTENT_SETTING_BUBBLE_MODEL_DELEGATE_H_

#include "chrome/browser/ui/browser_content_setting_bubble_model_delegate.h"

class adrbrowsielBrowserContentSettingBubbleModelDelegate
    : public BrowserContentSettingBubbleModelDelegate {
 public:
  explicit adrbrowsielBrowserContentSettingBubbleModelDelegate(Browser* browser);
  ~adrbrowsielBrowserContentSettingBubbleModelDelegate() override;

  void ShowWidevineLearnMorePage();
  void ShowLearnMorePage(ContentSettingsType type) override;

 private:
  Browser* const browser_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielBrowserContentSettingBubbleModelDelegate);
};

#endif  // adrbrowsiel_BROWSER_UI_adrbrowsiel_BROWSER_CONTENT_SETTING_BUBBLE_MODEL_DELEGATE_H_
