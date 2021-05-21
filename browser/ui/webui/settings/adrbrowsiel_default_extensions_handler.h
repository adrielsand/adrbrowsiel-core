/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_WEBUI_SETTINGS_adrbrowsiel_DEFAULT_EXTENSIONS_HANDLER_H_
#define adrbrowsiel_BROWSER_UI_WEBUI_SETTINGS_adrbrowsiel_DEFAULT_EXTENSIONS_HANDLER_H_

#include <string>

#include "base/memory/weak_ptr.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/buildflags/buildflags.h"
#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"
#include "adrbrowsiel/components/tor/buildflags/buildflags.h"
#include "chrome/browser/ui/webui/settings/settings_page_ui_handler.h"
#include "chrome/common/extensions/webstore_install_result.h"
#include "components/prefs/pref_change_registrar.h"
#include "third_party/widevine/cdm/buildflags.h"

#if BUILDFLAG(IPFS_ENABLED)
#include "adrbrowsiel/components/ipfs/ipfs_service.h"
#include "adrbrowsiel/components/ipfs/ipfs_service_observer.h"
#include "ui/shell_dialogs/select_file_dialog.h"
#endif

class Profile;

class adrbrowsielDefaultExtensionsHandler : public settings::SettingsPageUIHandler
#if BUILDFLAG(IPFS_ENABLED)
    ,
                                      ipfs::IpfsServiceObserver,
                                      public ui::SelectFileDialog::Listener
#endif
{
 public:
  adrbrowsielDefaultExtensionsHandler();
  ~adrbrowsielDefaultExtensionsHandler() override;

 private:
  // SettingsPageUIHandler overrides:
  void RegisterMessages() override;
  void OnJavascriptAllowed() override {}
  void OnJavascriptDisallowed() override {}

  void GetRestartNeeded(const base::ListValue* args);
  void SetWebTorrentEnabled(const base::ListValue* args);
  void SetHangoutsEnabled(const base::ListValue* args);
  void SetIPFSCompanionEnabled(const base::ListValue* args);
  void SetMediaRouterEnabled(const base::ListValue* args);
  void SetIPFSStorageMax(const base::ListValue* args);
  void ImportIpnsKey(const base::ListValue* args);
  void LaunchIPFSService(const base::ListValue* args);
  void ShutdownIPFSService(const base::ListValue* args);
#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
  void SetadrbrowsielWalletEnabled(const base::ListValue* args);
#endif
  void SetTorEnabled(const base::ListValue* args);
  void IsTorEnabled(const base::ListValue* args);
  void OnTorEnabledChanged();
  void IsTorManaged(const base::ListValue* args);
  void SetWidevineEnabled(const base::ListValue* args);
  void IsWidevineEnabled(const base::ListValue* args);
  void OnWidevineEnabledChanged();
  void IsDecentralizedDnsEnabled(const base::ListValue* args);
  void GetDecentralizedDnsResolveMethodList(const base::ListValue* args);

  void InitializePrefCallbacks();

  bool IsExtensionInstalled(const std::string& extension_id) const;
  void OnInstallResult(const std::string& pref_name,
                       bool success,
                       const std::string& error,
                       extensions::webstore_install::Result result);

  void OnRestartNeededChanged();
  void OnMediaRouterEnabledChanged();
  bool IsRestartNeeded();

#if BUILDFLAG(IPFS_ENABLED)
  // ui::SelectFileDialog::Listener
  void FileSelected(const base::FilePath& path,
                    int index,
                    void* params) override;
  void FileSelectionCanceled(void* params) override;

  void OnKeyImported(const std::string& key,
                     const std::string& value,
                     bool success);
  // ipfs::IpfsServiceObserver
  void OnIpfsLaunched(bool result, int64_t pid) override;
  void OnIpfsShutdown() override;
  void OnIpnsKeysLoaded(bool success) override;
  void CheckIpfsNodeStatus(const base::ListValue* args);
  void NotifyNodeStatus();

  std::string dialog_key_;
  scoped_refptr<ui::SelectFileDialog> select_file_dialog_;
#endif

#if BUILDFLAG(ENABLE_WIDEVINE)
  bool was_widevine_enabled_ = false;
#endif
  Profile* profile_ = nullptr;
  PrefChangeRegistrar pref_change_registrar_;
#if BUILDFLAG(ENABLE_TOR)
  PrefChangeRegistrar local_state_change_registrar_;
#endif
#if BUILDFLAG(IPFS_ENABLED)
  base::ScopedObservation<ipfs::IpfsService, ipfs::IpfsServiceObserver>
      ipfs_service_observer_{this};
#endif
  base::WeakPtrFactory<adrbrowsielDefaultExtensionsHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielDefaultExtensionsHandler);
};

#endif  // adrbrowsiel_BROWSER_UI_WEBUI_SETTINGS_adrbrowsiel_DEFAULT_EXTENSIONS_HANDLER_H_
