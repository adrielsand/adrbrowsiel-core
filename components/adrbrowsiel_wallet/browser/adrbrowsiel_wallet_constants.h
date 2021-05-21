/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_BROWSER_adrbrowsiel_WALLET_CONSTANTS_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_BROWSER_adrbrowsiel_WALLET_CONSTANTS_H_

enum class adrbrowsielWalletWeb3ProviderTypes {
  ASK,
  NONE,
  CRYPTO_WALLETS,
  METAMASK,
  adrbrowsiel_WALLET
};

extern const char ethereum_remote_client_extension_id[];
extern const char ethereum_remote_client_extension_name[];
extern const char ethereum_remote_client_extension_public_key[];
extern const char ethereum_remote_client_base_url[];
extern const char ethereum_remote_client_phishing_url[];
extern const char ethereum_remote_client_ens_redirect_url[];
extern const char ethereum_remote_client_host[];
extern const char metamask_extension_id[];

namespace adrbrowsiel_wallet {

enum class Network {
  kMainnet,
  kRinkeby,
  kRopsten,
  kGoerli,
  kKovan,
  kLocalhost,
  kCustom,
};

}  // namespace adrbrowsiel_wallet

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_BROWSER_adrbrowsiel_WALLET_CONSTANTS_H_
