/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_INFOBARS_adrbrowsiel_WAYBACK_MACHINE_DELEGATE_IMPL_H_
#define adrbrowsiel_BROWSER_INFOBARS_adrbrowsiel_WAYBACK_MACHINE_DELEGATE_IMPL_H_

#include <memory>

#include "adrbrowsiel/components/adrbrowsiel_wayback_machine/adrbrowsiel_wayback_machine_delegate.h"

namespace content {
class WebContents;
}  // namespace content

namespace infobars {
class InfoBar;
}  // namespace infobars

class adrbrowsielWaybackMachineDelegateImpl : public adrbrowsielWaybackMachineDelegate {
 public:
  static void AttachTabHelperIfNeeded(content::WebContents* web_contents);

  adrbrowsielWaybackMachineDelegateImpl();
  ~adrbrowsielWaybackMachineDelegateImpl() override;

  adrbrowsielWaybackMachineDelegateImpl(
      const adrbrowsielWaybackMachineDelegateImpl&) = delete;
  adrbrowsielWaybackMachineDelegateImpl& operator=(
      const adrbrowsielWaybackMachineDelegateImpl&) = delete;

 private:
  // adrbrowsielWaybackMachineDelegate overrides:
  void CreateInfoBar(content::WebContents* web_contents) override;

  std::unique_ptr<infobars::InfoBar> CreateInfoBarView(
      std::unique_ptr<adrbrowsielWaybackMachineInfoBarDelegate> delegate,
      content::WebContents* contents);
};

#endif  // adrbrowsiel_BROWSER_INFOBARS_adrbrowsiel_WAYBACK_MACHINE_DELEGATE_IMPL_H_
