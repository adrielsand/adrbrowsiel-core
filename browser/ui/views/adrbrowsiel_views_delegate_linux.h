/* This Source Code Form is subject to the terms of the Mozilla Public
+ * License, v. 2.0. If a copy of the MPL was not distributed with this file,
+ * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_VIEWS_adrbrowsiel_VIEWS_DELEGATE_LINUX_H_
#define adrbrowsiel_BROWSER_UI_VIEWS_adrbrowsiel_VIEWS_DELEGATE_LINUX_H_

#include "chrome/browser/ui/views/chrome_views_delegate.h"

class adrbrowsielViewsDelegateLinux : public ChromeViewsDelegate {
 public:
  adrbrowsielViewsDelegateLinux() = default;
  ~adrbrowsielViewsDelegateLinux() override = default;
 private:
  // ChromeViewsDelegate overrides:
  gfx::ImageSkia* GetDefaultWindowIcon() const override;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielViewsDelegateLinux);
};

#endif  // adrbrowsiel_BROWSER_UI_VIEWS_adrbrowsiel_VIEWS_DELEGATE_LINUX_H_
