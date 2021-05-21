/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_COMPONENTS_INFOBARS_CORE_INFOBAR_DELEGATE_H_
#define adrbrowsiel_CHROMIUM_SRC_COMPONENTS_INFOBARS_CORE_INFOBAR_DELEGATE_H_

#define adrbrowsiel_INFOBAR_DELEGATE_IDENTIFIERS  \
  adrbrowsiel_CONFIRM_P3A_INFOBAR_DELEGATE = 500, \
  CRYPTO_WALLETS_INFOBAR_DELEGATE = 501,    \
  WAYBACK_MACHINE_INFOBAR_DELEGATE = 502,   \
  SYNC_V2_MIGRATE_INFOBAR_DELEGATE = 503,

#include "../../../../../components/infobars/core/infobar_delegate.h"

#undef adrbrowsiel_INFOBAR_DELEGATE_IDENTIFIERS

#endif  // adrbrowsiel_CHROMIUM_SRC_COMPONENTS_INFOBARS_CORE_INFOBAR_DELEGATE_H_
