/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/adrbrowsiel_wallet/adrbrowsiel_wallet_context_utils.h"
#include "content/public/browser/browser_context.h"

namespace adrbrowsiel_wallet {

bool IsAllowedForContext(content::BrowserContext* context) {
  if (context && context->IsTor())
    return false;

  return true;
}

}  // namespace adrbrowsiel_wallet
