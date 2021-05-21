/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_adrbrowsiel_WALLET_adrbrowsiel_WALLET_DELEGATE_IMPL_H_
#define adrbrowsiel_BROWSER_adrbrowsiel_WALLET_adrbrowsiel_WALLET_DELEGATE_IMPL_H_

#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/adrbrowsiel_wallet_delegate.h"

class adrbrowsielWalletDelegateImpl : public adrbrowsielWalletDelegate {
 public:
  ~adrbrowsielWalletDelegateImpl() override;
  void MaybeLoadCryptoWalletsExtension(
      content::BrowserContext* context) override;
};

#endif  // adrbrowsiel_BROWSER_adrbrowsiel_WALLET_adrbrowsiel_WALLET_DELEGATE_IMPL_H_
