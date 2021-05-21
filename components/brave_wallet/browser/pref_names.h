/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_BROWSER_PREF_NAMES_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_BROWSER_PREF_NAMES_H_

extern const char kadrbrowsielWalletAES256GCMSivNonce[];
extern const char kadrbrowsielWalletEncryptedSeed[];
// Deprecated in favor of kadrbrowsielWalletWeb3Provider
extern const char kadrbrowsielWalletEnabledDeprecated[];
extern const char kadrbrowsielWalletPrefVersion[];
extern const char kadrbrowsielWalletWeb3Provider[];
extern const char kLoadCryptoWalletsOnStartup[];
extern const char kOptedIntoCryptoWallets[];
extern const char kadrbrowsielWalletPasswordEncryptorSalt[];
extern const char kadrbrowsielWalletPasswordEncryptorNonce[];
extern const char kadrbrowsielWalletEncryptedMnemonic[];
extern const char kadrbrowsielWalletDefaultKeyringAccountNum[];
extern const char kShowWalletIconOnToolbar[];

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_BROWSER_PREF_NAMES_H_
