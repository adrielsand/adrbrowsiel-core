/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_adrbrowsiel_WALLET_adrbrowsiel_WALLET_CONTEXT_UTILS_H_
#define adrbrowsiel_BROWSER_adrbrowsiel_WALLET_adrbrowsiel_WALLET_CONTEXT_UTILS_H_

namespace content {
class BrowserContext;
}

namespace adrbrowsiel_wallet {

bool IsAllowedForContext(content::BrowserContext* context);

}  // namespace adrbrowsiel_wallet

#endif  // adrbrowsiel_BROWSER_adrbrowsiel_WALLET_adrbrowsiel_WALLET_CONTEXT_UTILS_H_
