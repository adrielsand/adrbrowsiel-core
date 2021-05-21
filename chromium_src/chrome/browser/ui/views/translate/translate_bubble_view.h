/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TRANSLATE_TRANSLATE_BUBBLE_VIEW_H_
#define adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TRANSLATE_TRANSLATE_BUBBLE_VIEW_H_

#define adrbrowsiel_TRANSLATE_BUBBLE_VIEW_H_ \
  private: \
    friend class adrbrowsielTranslateBubbleView; \
    friend class adrbrowsielTranslateBubbleViewTest; \
    FRIEND_TEST_ALL_PREFIXES(adrbrowsielTranslateBubbleViewTest, \
                             adrbrowsielBeforeTranslateView); \
    FRIEND_TEST_ALL_PREFIXES(adrbrowsielTranslateBubbleViewTest, TranslateButton); \
    FRIEND_TEST_ALL_PREFIXES(adrbrowsielTranslateBubbleViewTest, CancelButton); \
  public:
// define adrbrowsiel_TRANSLATE_BUBBLE_VIEW_H_

class adrbrowsielTranslateBubbleView;
#include "../../../../../../../chrome/browser/ui/views/translate/translate_bubble_view.h"

#endif  // adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TRANSLATE_TRANSLATE_BUBBLE_VIEW_H_
