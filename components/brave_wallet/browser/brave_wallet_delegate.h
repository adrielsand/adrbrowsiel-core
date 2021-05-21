/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
+ * This Source Code Form is subject to the terms of the Mozilla Public
+ * License, v. 2.0. If a copy of the MPL was not distributed with this file,
+ * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_BROWSER_adrbrowsiel_WALLET_DELEGATE_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_BROWSER_adrbrowsiel_WALLET_DELEGATE_H_

namespace content {
class BrowserContext;
}  // namespace content

class adrbrowsielWalletDelegate {
 public:
  virtual ~adrbrowsielWalletDelegate() = default;
  virtual void MaybeLoadCryptoWalletsExtension(
      content::BrowserContext* context) = 0;
};

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_BROWSER_adrbrowsiel_WALLET_DELEGATE_H_
