/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_BROWSER_adrbrowsiel_WALLET_SERVICE_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_BROWSER_adrbrowsiel_WALLET_SERVICE_H_

#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/containers/queue.h"
#include "base/files/file_path.h"
#include "base/macros.h"
#include "base/memory/scoped_refptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/scoped_observer.h"
#include "base/values.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/eth_json_rpc_controller.h"
#include "components/keyed_service/core/keyed_service.h"
#include "extensions/buildflags/buildflags.h"
#include "url/gurl.h"

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "extensions/browser/extension_registry.h"
#include "extensions/browser/extension_registry_observer.h"
#endif

class adrbrowsielWalletDelegate;
class PrefChangeRegistrar;
class PrefService;

namespace base {
class FilePath;
class SequencedTaskRunner;
}  // namespace base

namespace content {
class BrowserContext;
}  // namespace content

class adrbrowsielWalletService : public KeyedService,
#if BUILDFLAG(ENABLE_EXTENSIONS)
                           public extensions::ExtensionRegistryObserver,
#endif
                           public base::SupportsWeakPtr<adrbrowsielWalletService> {
 public:
  explicit adrbrowsielWalletService(
      content::BrowserContext* context,
      std::unique_ptr<adrbrowsielWalletDelegate> adrbrowsiel_wallet_delegate);
  ~adrbrowsielWalletService() override;
  using LoadUICallback = base::OnceCallback<void()>;

  adrbrowsiel_wallet::EthJsonRpcController* controller() const;

  void ResetCryptoWallets();
  std::string GetWalletSeed(std::vector<uint8_t> key);
  std::string GetBitGoSeed(std::vector<uint8_t> key);
  bool IsCryptoWalletsSetup() const;
  bool IsCryptoWalletsReady() const;
  bool ShouldShowLazyLoadInfobar() const;
  void MaybeLoadCryptoWalletsExtension(LoadUICallback callback);
  void CryptoWalletsExtensionReady();

  static std::string GetEthereumRemoteClientSeedFromRootSeed(
      const std::string& seed);
  static std::string GetBitGoSeedFromRootSeed(const std::string& seed);
  static bool SealSeed(const std::string& seed,
                       const std::string& key,
                       const std::string& nonce,
                       std::string* cipher_seed);
  static bool OpenSeed(const std::string& cipher_seed,
                       const std::string& key,
                       const std::string& nonce,
                       std::string* seed);
  static void SaveToPrefs(PrefService* prefs,
                          const std::string& cipher_seed,
                          const std::string& nonce);
  static bool LoadFromPrefs(PrefService* prefs,
                            std::string* cipher_seed,
                            std::string* nonce);
  static std::string GetRandomNonce();
  static std::string GetRandomSeed();
  static const size_t kNonceByteLength;
  static const size_t kSeedByteLength;

 private:
  bool LoadRootSeedInfo(std::vector<uint8_t> key, std::string* seed);
  void RemoveUnusedWeb3ProviderContentScripts();
  void OnPreferenceChanged();

#if BUILDFLAG(ENABLE_EXTENSIONS)
  void OnExtensionInstalled(content::BrowserContext* browser_context,
                            const extensions::Extension* extension,
                            bool is_update) override;
  void OnExtensionLoaded(content::BrowserContext* browser_context,
                         const extensions::Extension* extension) override;
  void OnExtensionUnloaded(content::BrowserContext* browser_context,
                           const extensions::Extension* extension,
                           extensions::UnloadedExtensionReason reason) override;
  void OnExtensionUninstalled(content::BrowserContext* browser_context,
                              const extensions::Extension* extension,
                              extensions::UninstallReason reason) override;
#endif

  content::BrowserContext* context_;
  std::unique_ptr<PrefChangeRegistrar> pref_change_registrar_;
  std::unique_ptr<adrbrowsielWalletDelegate> adrbrowsiel_wallet_delegate_;
  std::unique_ptr<adrbrowsiel_wallet::EthJsonRpcController> controller_;
#if BUILDFLAG(ENABLE_EXTENSIONS)
  ScopedObserver<extensions::ExtensionRegistry,
                 extensions::ExtensionRegistryObserver>
      extension_registry_observer_{this};
#endif
  scoped_refptr<base::SequencedTaskRunner> file_task_runner_;
  LoadUICallback load_ui_callback_;
  DISALLOW_COPY_AND_ASSIGN(adrbrowsielWalletService);
};

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_WALLET_BROWSER_adrbrowsiel_WALLET_SERVICE_H_
