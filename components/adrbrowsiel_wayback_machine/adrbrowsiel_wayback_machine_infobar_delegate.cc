/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/adrbrowsiel_wayback_machine/adrbrowsiel_wayback_machine_infobar_delegate.h"

using InfoBarIdentifier = infobars::InfoBarDelegate::InfoBarIdentifier;

adrbrowsielWaybackMachineInfoBarDelegate::
adrbrowsielWaybackMachineInfoBarDelegate() = default;

adrbrowsielWaybackMachineInfoBarDelegate::
~adrbrowsielWaybackMachineInfoBarDelegate() = default;

InfoBarIdentifier adrbrowsielWaybackMachineInfoBarDelegate::GetIdentifier() const {
  return WAYBACK_MACHINE_INFOBAR_DELEGATE;
}

bool adrbrowsielWaybackMachineInfoBarDelegate::EqualsDelegate(
    infobars::InfoBarDelegate* delegate) const {
  return delegate->GetIdentifier() == GetIdentifier();
}
