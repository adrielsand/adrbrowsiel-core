/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_WAYBACK_MACHINE_adrbrowsiel_WAYBACK_MACHINE_INFOBAR_DELEGATE_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_WAYBACK_MACHINE_adrbrowsiel_WAYBACK_MACHINE_INFOBAR_DELEGATE_H_

#include <memory>

#include "components/infobars/core/infobar_delegate.h"

class adrbrowsielWaybackMachineInfoBarDelegate : public infobars::InfoBarDelegate {
 public:
  adrbrowsielWaybackMachineInfoBarDelegate();
  ~adrbrowsielWaybackMachineInfoBarDelegate() override;

  adrbrowsielWaybackMachineInfoBarDelegate(
      const adrbrowsielWaybackMachineInfoBarDelegate&) = delete;
  adrbrowsielWaybackMachineInfoBarDelegate& operator=(
      const adrbrowsielWaybackMachineInfoBarDelegate&) = delete;

 private:
  // infobars::InfoBarDelegate overrides:
  InfoBarIdentifier GetIdentifier() const override;
  bool EqualsDelegate(
      infobars::InfoBarDelegate* delegate) const override;
};

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_WAYBACK_MACHINE_adrbrowsiel_WAYBACK_MACHINE_INFOBAR_DELEGATE_H_
