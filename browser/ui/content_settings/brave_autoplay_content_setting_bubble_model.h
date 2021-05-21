/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_CONTENT_SETTINGS_adrbrowsiel_AUTOPLAY_CONTENT_SETTING_BUBBLE_MODEL_H_
#define adrbrowsiel_BROWSER_UI_CONTENT_SETTINGS_adrbrowsiel_AUTOPLAY_CONTENT_SETTING_BUBBLE_MODEL_H_

#include "chrome/browser/ui/content_settings/content_setting_bubble_model.h"

class Profile;

using content::WebContents;

class adrbrowsielAutoplayContentSettingBubbleModel : public ContentSettingSimpleBubbleModel {
 public:
   adrbrowsielAutoplayContentSettingBubbleModel(Delegate* delegate,
                                          WebContents* web_contents);
   ~adrbrowsielAutoplayContentSettingBubbleModel() override;

  // ContentSettingSimpleBubbleModel:
  void CommitChanges() override;

 protected:
  bool settings_changed() const;

 private:
  void SetTitle();
  void SetRadioGroup();
  void SetNarrowestContentSetting(ContentSetting setting);

  ContentSetting block_setting_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielAutoplayContentSettingBubbleModel);
};

#endif // adrbrowsiel_BROWSER_UI_CONTENT_SETTINGS_adrbrowsiel_AUTOPLAY_CONTENT_SETTING_BUBBLE_MODEL_H_
