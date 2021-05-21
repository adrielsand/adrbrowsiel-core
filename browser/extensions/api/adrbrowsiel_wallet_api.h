/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_EXTENSIONS_API_adrbrowsiel_WALLET_API_H_
#define adrbrowsiel_BROWSER_EXTENSIONS_API_adrbrowsiel_WALLET_API_H_

#include <string>

#include "extensions/browser/extension_function.h"

class Profile;

namespace extensions {
namespace api {

class adrbrowsielWalletPromptToEnableWalletFunction :
    public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielWallet.promptToEnableWallet", UNKNOWN)

 protected:
  ~adrbrowsielWalletPromptToEnableWalletFunction() override {}
  ResponseAction Run() override;
};

class adrbrowsielWalletReadyFunction :
    public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielWallet.ready", UNKNOWN)

 protected:
  ~adrbrowsielWalletReadyFunction() override {}
  ResponseAction Run() override;
};

class adrbrowsielWalletShouldCheckForDappsFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielWallet.shouldCheckForDapps", UNKNOWN)

 protected:
  ~adrbrowsielWalletShouldCheckForDappsFunction() override {}
  ResponseAction Run() override;
};

class adrbrowsielWalletLoadUIFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielWallet.loadUI", UNKNOWN)
  void OnLoaded();

 protected:
  ~adrbrowsielWalletLoadUIFunction() override {}
  ResponseAction Run() override;
};

class adrbrowsielWalletShouldPromptForSetupFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielWallet.shouldPromptForSetup", UNKNOWN)

 protected:
  ~adrbrowsielWalletShouldPromptForSetupFunction() override {}
  ResponseAction Run() override;
};

class adrbrowsielWalletGetWalletSeedFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielWallet.getWalletSeed", UNKNOWN)

 protected:
  ~adrbrowsielWalletGetWalletSeedFunction() override {}
  ResponseAction Run() override;
};

class adrbrowsielWalletGetBitGoSeedFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielWallet.getBitGoSeed", UNKNOWN)

 protected:
  ~adrbrowsielWalletGetBitGoSeedFunction() override {}
  ResponseAction Run() override;
};

class adrbrowsielWalletGetProjectIDFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielWallet.getProjectID", UNKNOWN)

 protected:
  ~adrbrowsielWalletGetProjectIDFunction() override {}
  ResponseAction Run() override;
};

class adrbrowsielWalletGetadrbrowsielKeyFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielWallet.getadrbrowsielKey", UNKNOWN)

 protected:
  ~adrbrowsielWalletGetadrbrowsielKeyFunction() override {}
  ResponseAction Run() override;
};

class adrbrowsielWalletResetWalletFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielWallet.resetWallet", UNKNOWN)

 protected:
  ~adrbrowsielWalletResetWalletFunction() override {}
  ResponseAction Run() override;
};

class adrbrowsielWalletGetWeb3ProviderFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielWallet.getWeb3Provider", UNKNOWN)

 protected:
  ~adrbrowsielWalletGetWeb3ProviderFunction() override {}
  ResponseAction Run() override;
};

class adrbrowsielWalletGetWeb3ProviderListFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielWallet.getWeb3ProviderList", UNKNOWN)

 protected:
  ~adrbrowsielWalletGetWeb3ProviderListFunction() override {}
  ResponseAction Run() override;
};

class adrbrowsielWalletIsNativeWalletEnabledFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielWallet.isNativeWalletEnabled", UNKNOWN)

 protected:
  ~adrbrowsielWalletIsNativeWalletEnabledFunction() override {}
  ResponseAction Run() override;
};

}  // namespace api
}  // namespace extensions

#endif  // adrbrowsiel_BROWSER_EXTENSIONS_API_adrbrowsiel_WALLET_API_H_
