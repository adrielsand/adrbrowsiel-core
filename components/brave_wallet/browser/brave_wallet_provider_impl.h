/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_BROWSER_adrbrowsiel_WALLET_PROVIDER_IMPL_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_BROWSER_adrbrowsiel_WALLET_PROVIDER_IMPL_H_

#include <map>
#include <memory>
#include <string>

#include "base/memory/weak_ptr.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/adrbrowsiel_wallet.mojom.h"

class adrbrowsielWalletService;

namespace adrbrowsiel_wallet {

class adrbrowsielWalletProviderImpl final
    : public adrbrowsiel_wallet::mojom::adrbrowsielWalletProvider {
 public:
  adrbrowsielWalletProviderImpl(const adrbrowsielWalletProviderImpl&) = delete;
  adrbrowsielWalletProviderImpl& operator=(const adrbrowsielWalletProviderImpl&) = delete;
  explicit adrbrowsielWalletProviderImpl(
      base::WeakPtr<adrbrowsielWalletService> wallet_service);
  ~adrbrowsielWalletProviderImpl() override;

  void Request(const std::string& json_payload,
               RequestCallback callback) override;
  void OnResponse(RequestCallback callback,
                  const int http_code,
                  const std::string& response,
                  const std::map<std::string, std::string>& headers);

 private:
  base::WeakPtr<adrbrowsielWalletService> wallet_service_;

  base::WeakPtrFactory<adrbrowsielWalletProviderImpl> weak_factory_;
};

}  // namespace adrbrowsiel_wallet

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_BROWSER_adrbrowsiel_WALLET_PROVIDER_IMPL_H_
