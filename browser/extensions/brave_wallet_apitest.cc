/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/path_service.h"
#include "adrbrowsiel/common/adrbrowsiel_paths.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/adrbrowsiel_wallet_constants.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/pref_names.h"
#include "chrome/browser/extensions/extension_apitest.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_test.h"
#include "extensions/common/constants.h"
#include "extensions/test/result_catcher.h"

namespace extensions {
namespace {

class adrbrowsielWalletExtensionApiTest : public ExtensionApiTest {
 public:
  void SetUp() override {
    adrbrowsiel::RegisterPathProvider();
    base::PathService::Get(adrbrowsiel::DIR_TEST_DATA, &extension_dir_);
    extension_dir_ = extension_dir_.AppendASCII("extensions/api_test");
    ExtensionApiTest::SetUp();
  }
  void TearDown() override {
    ExtensionApiTest::TearDown();
  }
  PrefService* GetPrefs() { return browser()->profile()->GetPrefs(); }
  base::FilePath extension_dir_;
};

IN_PROC_BROWSER_TEST_F(adrbrowsielWalletExtensionApiTest,
    adrbrowsielExtensionWithWalletHasAccess) {
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("adrbrowsielShieldsWithWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), adrbrowsiel_extension_id,
      "testBasics()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(adrbrowsielWalletExtensionApiTest,
    adrbrowsielWalletAPIAvailable) {
  std::unique_ptr<base::Environment> env(base::Environment::Create());
  env->SetVar("adrbrowsiel_INFURA_PROJECT_ID", "test-project-id");
  env->SetVar("adrbrowsiel_SERVICES_KEY", "test-adrbrowsiel-key");
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("adrbrowsielWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testBasics()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(adrbrowsielWalletExtensionApiTest,
    adrbrowsielWalletAPIKnownValuesTest) {
  GetPrefs()->SetString(kadrbrowsielWalletAES256GCMSivNonce, "yJngKDr5nCGYz7EM");
  GetPrefs()->SetString(kadrbrowsielWalletEncryptedSeed,
      "IQu5fUMbXG6E7v8ITwcIKL3TI3rst0LU1US7ZxCKpgAGgLNAN6DbCN7nMF2Eg7Kx");
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("adrbrowsielWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testKnownSeedValuesEndToEnd()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(adrbrowsielWalletExtensionApiTest,
    adrbrowsielWalletAPIBitGoKnownValuesTest) {
  GetPrefs()->SetString(kadrbrowsielWalletAES256GCMSivNonce, "yJngKDr5nCGYz7EM");
  GetPrefs()->SetString(kadrbrowsielWalletEncryptedSeed,
      "IQu5fUMbXG6E7v8ITwcIKL3TI3rst0LU1US7ZxCKpgAGgLNAN6DbCN7nMF2Eg7Kx");
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("adrbrowsielWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testKnownBitGoSeedValuesEndToEnd()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(adrbrowsielWalletExtensionApiTest,
    adrbrowsielWalletWeb3ProviderCryptoWallets) {
  GetPrefs()->SetInteger(kadrbrowsielWalletWeb3Provider,
      static_cast<int>(adrbrowsielWalletWeb3ProviderTypes::CRYPTO_WALLETS));
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("adrbrowsielWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testProviderIsCryptoWallets()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(adrbrowsielWalletExtensionApiTest,
    adrbrowsielWalletWeb3ProviderMetaMask) {
  GetPrefs()->SetInteger(kadrbrowsielWalletWeb3Provider,
      static_cast<int>(adrbrowsielWalletWeb3ProviderTypes::METAMASK));
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("adrbrowsielWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testProviderIsMetaMask()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(adrbrowsielWalletExtensionApiTest,
    adrbrowsielWalletWeb3ProviderAsk) {
  GetPrefs()->SetInteger(kadrbrowsielWalletWeb3Provider,
      static_cast<int>(adrbrowsielWalletWeb3ProviderTypes::ASK));
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("adrbrowsielWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testProviderIsAsk()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(adrbrowsielWalletExtensionApiTest,
    adrbrowsielWalletWeb3ProviderNone) {
  GetPrefs()->SetInteger(kadrbrowsielWalletWeb3Provider,
      static_cast<int>(adrbrowsielWalletWeb3ProviderTypes::NONE));
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("adrbrowsielWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testProviderIsNone()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(adrbrowsielWalletExtensionApiTest,
                       adrbrowsielWalletWeb3ProvideradrbrowsielWallet) {
  GetPrefs()->SetInteger(
      kadrbrowsielWalletWeb3Provider,
      static_cast<int>(adrbrowsielWalletWeb3ProviderTypes::adrbrowsiel_WALLET));
  ResultCatcher catcher;
  const Extension* extension =
      LoadExtension(extension_dir_.AppendASCII("adrbrowsielWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testProviderIsadrbrowsielWallet()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(adrbrowsielWalletExtensionApiTest,
    adrbrowsielWalletAPINotAvailable) {
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("notadrbrowsielWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(adrbrowsielWalletExtensionApiTest,
    adrbrowsielShieldsDappDetectionWhenDefault) {
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("adrbrowsielShieldsWithWallet"));
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), adrbrowsiel_extension_id, "testDappCheck()"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(adrbrowsielWalletExtensionApiTest,
    adrbrowsielShieldsDappDetectionWhenAsk) {
  GetPrefs()->SetInteger(kadrbrowsielWalletWeb3Provider,
      static_cast<int>(adrbrowsielWalletWeb3ProviderTypes::ASK));
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("adrbrowsielShieldsWithWallet"));
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), adrbrowsiel_extension_id, "testDappCheck()"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(adrbrowsielWalletExtensionApiTest,
    adrbrowsielShieldsNoDappDetectionWhenNone) {
  GetPrefs()->SetInteger(kadrbrowsielWalletWeb3Provider,
      static_cast<int>(adrbrowsielWalletWeb3ProviderTypes::NONE));
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("adrbrowsielShieldsWithWallet"));
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), adrbrowsiel_extension_id, "testNoDappCheck()"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(adrbrowsielWalletExtensionApiTest,
    adrbrowsielShieldsNoDappDetectionWhenMetaMask) {
  GetPrefs()->SetInteger(kadrbrowsielWalletWeb3Provider,
      static_cast<int>(adrbrowsielWalletWeb3ProviderTypes::METAMASK));
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("adrbrowsielShieldsWithWallet"));
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), adrbrowsiel_extension_id, "testNoDappCheck()"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(adrbrowsielWalletExtensionApiTest,
    adrbrowsielShieldsNoDappDetectionWhenCryptoWallets) {
  GetPrefs()->SetInteger(kadrbrowsielWalletWeb3Provider,
      static_cast<int>(adrbrowsielWalletWeb3ProviderTypes::CRYPTO_WALLETS));
  ResultCatcher catcher;
  const Extension* extension =
      LoadExtension(extension_dir_.AppendASCII("adrbrowsielShieldsWithWallet"));
  LoadExtension(extension_dir_.AppendASCII("adrbrowsielWallet"));
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), adrbrowsiel_extension_id, "testNoDappCheck()"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(adrbrowsielWalletExtensionApiTest,
                       adrbrowsielShieldsNoDappDetectionWhenadrbrowsielWallet) {
  GetPrefs()->SetInteger(
      kadrbrowsielWalletWeb3Provider,
      static_cast<int>(adrbrowsielWalletWeb3ProviderTypes::adrbrowsiel_WALLET));
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("adrbrowsielShieldsWithWallet"));
  LoadExtension(extension_dir_.AppendASCII("adrbrowsielWallet"));
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), adrbrowsiel_extension_id, "testNoDappCheck()"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(adrbrowsielWalletExtensionApiTest,
    adrbrowsielShieldsDappDetectionWhenCryptoWalletsNotReady) {
  GetPrefs()->SetInteger(kadrbrowsielWalletWeb3Provider,
      static_cast<int>(adrbrowsielWalletWeb3ProviderTypes::CRYPTO_WALLETS));
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("adrbrowsielShieldsWithWallet"));
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), adrbrowsiel_extension_id, "testDappCheck()"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

}  // namespace
}  // namespace extensions
