/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * you can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_HOVER_CARD_BUBBLE_VIEW_H_
#define adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_HOVER_CARD_BUBBLE_VIEW_H_

// Inject a protected method that will have access to the private members of the
// base class. Then, we can call this method from the subclass' override.
#define adrbrowsiel_TAB_HOVER_CARD_BUBBLE_VIEW_H_ \
 protected:                                 \
  void adrbrowsielUpdateCardContent(const Tab* tab);

#define TabHoverCardBubbleView TabHoverCardBubbleView_ChromiumImpl
#define UpdateCardContent virtual UpdateCardContent
#include "../../../../../../../chrome/browser/ui/views/tabs/tab_hover_card_bubble_view.h"
#undef UpdateCardContent
#undef TabHoverCardBubbleView
#undef adrbrowsiel_TAB_HOVER_CARD_BUBBLE_VIEW_H_

class TabHoverCardBubbleView : public TabHoverCardBubbleView_ChromiumImpl {
 public:
  using TabHoverCardBubbleView_ChromiumImpl::
      TabHoverCardBubbleView_ChromiumImpl;

  void UpdateCardContent(const Tab* tab) override;

  DISALLOW_COPY_AND_ASSIGN(TabHoverCardBubbleView);
};

#endif  // adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_HOVER_CARD_BUBBLE_VIEW_H_
