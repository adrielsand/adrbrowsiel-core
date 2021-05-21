/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/webui/settings/adrbrowsiel_default_extensions_handler.h"

#include <memory>
#include <string>

#include "base/bind.h"
#include "base/values.h"
#include "adrbrowsiel/browser/extensions/adrbrowsiel_component_loader.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_webtorrent/grit/adrbrowsiel_webtorrent_resources.h"
#include "adrbrowsiel/components/decentralized_dns/buildflags/buildflags.h"
#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"
#include "chrome/browser/about_flags.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/extensions/component_loader.h"
#include "chrome/browser/extensions/extension_service.h"
#include "chrome/browser/extensions/webstore_install_with_prompt.h"
#include "chrome/browser/lifetime/application_lifetime.h"
#include "chrome/browser/media/router/media_router_feature.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/chrome_select_file_policy.h"
#include "chrome/common/chrome_switches.h"
#include "chrome/common/pref_names.h"
#include "components/flags_ui/flags_ui_constants.h"
#include "components/flags_ui/pref_service_flags_storage.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_ui.h"
#include "extensions/browser/extension_registry.h"
#include "extensions/browser/extension_system.h"
#include "extensions/common/constants.h"
#include "extensions/common/feature_switch.h"

#if BUILDFLAG(ENABLE_TOR)
#include "adrbrowsiel/browser/tor/tor_profile_service_factory.h"
#include "adrbrowsiel/components/tor/pref_names.h"
#endif

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/adrbrowsiel_wallet_constants.h"
#endif

#if BUILDFLAG(ENABLE_WIDEVINE)
#include "adrbrowsiel/browser/widevine/widevine_utils.h"
#endif

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
#include "adrbrowsiel/components/decentralized_dns/constants.h"
#include "adrbrowsiel/components/decentralized_dns/utils.h"
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "adrbrowsiel/browser/ipfs/ipfs_service_factory.h"
#include "adrbrowsiel/components/ipfs/ipfs_service.h"
#include "adrbrowsiel/components/ipfs/keys/ipns_keys_manager.h"
#include "adrbrowsiel/components/ipfs/pref_names.h"
#endif

adrbrowsielDefaultExtensionsHandler::adrbrowsielDefaultExtensionsHandler()
    : weak_ptr_factory_(this) {
#if BUILDFLAG(ENABLE_WIDEVINE)
  was_widevine_enabled_ = IsWidevineOptedIn();
#endif
}

adrbrowsielDefaultExtensionsHandler::~adrbrowsielDefaultExtensionsHandler() {}

void adrbrowsielDefaultExtensionsHandler::RegisterMessages() {
  profile_ = Profile::FromWebUI(web_ui());
#if BUILDFLAG(IPFS_ENABLED)
  ipfs::IpfsService* service =
      ipfs::IpfsServiceFactory::GetForContext(profile_);
  if (service) {
    ipfs_service_observer_.Observe(service);
  }
  web_ui()->RegisterMessageCallback(
      "notifyIpfsNodeStatus",
      base::BindRepeating(&adrbrowsielDefaultExtensionsHandler::CheckIpfsNodeStatus,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setIPFSStorageMax",
      base::BindRepeating(&adrbrowsielDefaultExtensionsHandler::SetIPFSStorageMax,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "importIpnsKey",
      base::BindRepeating(&adrbrowsielDefaultExtensionsHandler::ImportIpnsKey,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "launchIPFSService",
      base::BindRepeating(&adrbrowsielDefaultExtensionsHandler::LaunchIPFSService,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "shutdownIPFSService",
      base::BindRepeating(&adrbrowsielDefaultExtensionsHandler::ShutdownIPFSService,
                          base::Unretained(this)));
#endif

  web_ui()->RegisterMessageCallback(
      "setWebTorrentEnabled",
      base::BindRepeating(&adrbrowsielDefaultExtensionsHandler::SetWebTorrentEnabled,
                          base::Unretained(this)));
#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
  web_ui()->RegisterMessageCallback(
      "setadrbrowsielWalletEnabled",
      base::BindRepeating(&adrbrowsielDefaultExtensionsHandler::SetadrbrowsielWalletEnabled,
                          base::Unretained(this)));
#endif
  web_ui()->RegisterMessageCallback(
      "setHangoutsEnabled",
      base::BindRepeating(&adrbrowsielDefaultExtensionsHandler::SetHangoutsEnabled,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setIPFSCompanionEnabled",
      base::BindRepeating(
          &adrbrowsielDefaultExtensionsHandler::SetIPFSCompanionEnabled,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setMediaRouterEnabled",
      base::BindRepeating(&adrbrowsielDefaultExtensionsHandler::SetMediaRouterEnabled,
                          base::Unretained(this)));
  // TODO(petemill): If anything outside this handler is responsible for causing
  // restart-neccessary actions, then this should be moved to a generic handler
  // and the flag should be moved to somewhere more static / singleton-like.
  web_ui()->RegisterMessageCallback(
      "getRestartNeeded",
      base::BindRepeating(&adrbrowsielDefaultExtensionsHandler::GetRestartNeeded,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setTorEnabled",
      base::BindRepeating(&adrbrowsielDefaultExtensionsHandler::SetTorEnabled,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "isTorEnabled",
      base::BindRepeating(&adrbrowsielDefaultExtensionsHandler::IsTorEnabled,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "isTorManaged",
      base::BindRepeating(&adrbrowsielDefaultExtensionsHandler::IsTorManaged,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setWidevineEnabled",
      base::BindRepeating(&adrbrowsielDefaultExtensionsHandler::SetWidevineEnabled,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "isWidevineEnabled",
      base::BindRepeating(&adrbrowsielDefaultExtensionsHandler::IsWidevineEnabled,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "isDecentralizedDnsEnabled",
      base::BindRepeating(
          &adrbrowsielDefaultExtensionsHandler::IsDecentralizedDnsEnabled,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getDecentralizedDnsResolveMethodList",
      base::BindRepeating(
          &adrbrowsielDefaultExtensionsHandler::GetDecentralizedDnsResolveMethodList,
          base::Unretained(this)));

  // Can't call this in ctor because it needs to access web_ui().
  InitializePrefCallbacks();
}

void adrbrowsielDefaultExtensionsHandler::InitializePrefCallbacks() {
  PrefService* prefs = Profile::FromWebUI(web_ui())->GetPrefs();
  pref_change_registrar_.Init(prefs);
  pref_change_registrar_.Add(
      kadrbrowsielEnabledMediaRouter,
      base::Bind(&adrbrowsielDefaultExtensionsHandler::OnMediaRouterEnabledChanged,
                 base::Unretained(this)));
  pref_change_registrar_.Add(
      prefs::kEnableMediaRouter,
      base::Bind(&adrbrowsielDefaultExtensionsHandler::OnMediaRouterEnabledChanged,
                 base::Unretained(this)));
  local_state_change_registrar_.Init(g_browser_process->local_state());
#if BUILDFLAG(ENABLE_TOR)
  local_state_change_registrar_.Add(
      tor::prefs::kTorDisabled,
      base::Bind(&adrbrowsielDefaultExtensionsHandler::OnTorEnabledChanged,
                 base::Unretained(this)));
#endif

#if BUILDFLAG(ENABLE_WIDEVINE)
  local_state_change_registrar_.Add(
      kWidevineOptedIn,
      base::Bind(&adrbrowsielDefaultExtensionsHandler::OnWidevineEnabledChanged,
                 base::Unretained(this)));
#endif
}

void adrbrowsielDefaultExtensionsHandler::OnMediaRouterEnabledChanged() {
  OnRestartNeededChanged();
}

bool adrbrowsielDefaultExtensionsHandler::IsRestartNeeded() {
  bool media_router_current_pref =
      profile_->GetPrefs()->GetBoolean(prefs::kEnableMediaRouter);
  bool media_router_new_pref =
      profile_->GetPrefs()->GetBoolean(kadrbrowsielEnabledMediaRouter);

  if (media_router_current_pref != media_router_new_pref)
    return true;

#if BUILDFLAG(ENABLE_WIDEVINE)
  if (was_widevine_enabled_ != IsWidevineOptedIn())
    return true;
#endif

  return false;
}

void adrbrowsielDefaultExtensionsHandler::GetRestartNeeded(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);

  AllowJavascript();
  ResolveJavascriptCallback(args->GetList()[0], base::Value(IsRestartNeeded()));
}

void adrbrowsielDefaultExtensionsHandler::SetWebTorrentEnabled(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);
  bool enabled;
  args->GetBoolean(0, &enabled);

  extensions::ExtensionService* service =
      extensions::ExtensionSystem::Get(profile_)->extension_service();
  extensions::ComponentLoader* loader = service->component_loader();

  if (enabled) {
    if (!loader->Exists(adrbrowsiel_webtorrent_extension_id)) {
      base::FilePath adrbrowsiel_webtorrent_path(FILE_PATH_LITERAL(""));
      adrbrowsiel_webtorrent_path =
          adrbrowsiel_webtorrent_path.Append(FILE_PATH_LITERAL("adrbrowsiel_webtorrent"));
      loader->Add(IDR_adrbrowsiel_WEBTORRENT, adrbrowsiel_webtorrent_path);
    }
    service->EnableExtension(adrbrowsiel_webtorrent_extension_id);
  } else {
    service->DisableExtension(
        adrbrowsiel_webtorrent_extension_id,
        extensions::disable_reason::DisableReason::DISABLE_BLOCKED_BY_POLICY);
  }
}

void adrbrowsielDefaultExtensionsHandler::SetHangoutsEnabled(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);
  bool enabled;
  args->GetBoolean(0, &enabled);

  extensions::ExtensionService* service =
      extensions::ExtensionSystem::Get(profile_)->extension_service();

  if (enabled) {
    extensions::ComponentLoader* loader = service->component_loader();
    if (!loader->Exists(hangouts_extension_id)) {
      static_cast<extensions::adrbrowsielComponentLoader*>(loader)
          ->ForceAddHangoutServicesExtension();
    }
    service->EnableExtension(hangouts_extension_id);
  } else {
    service->DisableExtension(
        hangouts_extension_id,
        extensions::disable_reason::DisableReason::DISABLE_BLOCKED_BY_POLICY);
  }
}

bool adrbrowsielDefaultExtensionsHandler::IsExtensionInstalled(
    const std::string& extension_id) const {
  extensions::ExtensionRegistry* registry = extensions::ExtensionRegistry::Get(
      static_cast<content::BrowserContext*>(profile_));
  return registry && registry->GetInstalledExtension(extension_id);
}

void adrbrowsielDefaultExtensionsHandler::OnInstallResult(
    const std::string& pref_name,
    bool success,
    const std::string& error,
    extensions::webstore_install::Result result) {
  if (result != extensions::webstore_install::Result::SUCCESS &&
      result != extensions::webstore_install::Result::LAUNCH_IN_PROGRESS) {
    profile_->GetPrefs()->SetBoolean(pref_name, false);
  }
}

void adrbrowsielDefaultExtensionsHandler::OnRestartNeededChanged() {
  if (IsJavascriptAllowed()) {
    FireWebUIListener("adrbrowsiel-needs-restart-changed",
                      base::Value(IsRestartNeeded()));
  }
}

void adrbrowsielDefaultExtensionsHandler::SetMediaRouterEnabled(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);
  bool enabled;
  args->GetBoolean(0, &enabled);

  std::string feature_name(switches::kLoadMediaRouterComponentExtension);
  enabled ? feature_name += "@1" : feature_name += "@2";
  flags_ui::PrefServiceFlagsStorage flags_storage(
      g_browser_process->local_state());
  about_flags::SetFeatureEntryEnabled(&flags_storage, feature_name, true);
}

void adrbrowsielDefaultExtensionsHandler::SetTorEnabled(const base::ListValue* args) {
#if BUILDFLAG(ENABLE_TOR)
  CHECK_EQ(args->GetSize(), 1U);
  bool enabled;
  args->GetBoolean(0, &enabled);
  AllowJavascript();
  TorProfileServiceFactory::SetTorDisabled(!enabled);
#endif
}

void adrbrowsielDefaultExtensionsHandler::IsTorEnabled(const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(
      args->GetList()[0],
#if BUILDFLAG(ENABLE_TOR)
      base::Value(!TorProfileServiceFactory::IsTorDisabled()));
#else
      base::Value(false));
#endif
}

void adrbrowsielDefaultExtensionsHandler::OnTorEnabledChanged() {
  if (IsJavascriptAllowed()) {
    FireWebUIListener("tor-enabled-changed",
#if BUILDFLAG(ENABLE_TOR)
                      base::Value(!TorProfileServiceFactory::IsTorDisabled()));
#else
                      base::Value(false));
#endif
  }
}

void adrbrowsielDefaultExtensionsHandler::IsTorManaged(const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);

#if BUILDFLAG(ENABLE_TOR)
  const bool is_managed = g_browser_process->local_state()
                              ->FindPreference(tor::prefs::kTorDisabled)
                              ->IsManaged();
#else
  const bool is_managed = false;
#endif

  AllowJavascript();
  ResolveJavascriptCallback(args->GetList()[0], base::Value(is_managed));
}

void adrbrowsielDefaultExtensionsHandler::SetWidevineEnabled(
    const base::ListValue* args) {
#if BUILDFLAG(ENABLE_WIDEVINE)
  CHECK_EQ(args->GetSize(), 1U);
  bool enabled;
  args->GetBoolean(0, &enabled);
  enabled ? EnableWidevineCdmComponent() : DisableWidevineCdmComponent();
  AllowJavascript();
#endif
}

void adrbrowsielDefaultExtensionsHandler::IsWidevineEnabled(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(args->GetList()[0],
#if BUILDFLAG(ENABLE_WIDEVINE)
                            base::Value(IsWidevineOptedIn()));
#else
                            base::Value(false));
#endif
}

void adrbrowsielDefaultExtensionsHandler::OnWidevineEnabledChanged() {
  if (IsJavascriptAllowed()) {
    FireWebUIListener("widevine-enabled-changed",
#if BUILDFLAG(ENABLE_WIDEVINE)
                      base::Value(IsWidevineOptedIn()));
#else
                      base::Value(false));
#endif
    OnRestartNeededChanged();
  }
}

void adrbrowsielDefaultExtensionsHandler::ShutdownIPFSService(
    const base::ListValue* args) {
  ipfs::IpfsService* service =
      ipfs::IpfsServiceFactory::GetForContext(profile_);
  if (!service) {
    return;
  }
  if (service->IsDaemonLaunched())
    service->ShutdownDaemon(base::NullCallback());
}

void adrbrowsielDefaultExtensionsHandler::LaunchIPFSService(
    const base::ListValue* args) {
  ipfs::IpfsService* service =
      ipfs::IpfsServiceFactory::GetForContext(profile_);
  if (!service) {
    return;
  }
  if (!service->IsDaemonLaunched())
    service->LaunchDaemon(base::NullCallback());
}

void adrbrowsielDefaultExtensionsHandler::SetIPFSStorageMax(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);
  int storage_max_gb = 0;
  args->GetInteger(0, &storage_max_gb);
  PrefService* prefs = Profile::FromWebUI(web_ui())->GetPrefs();
  prefs->SetInteger(kIpfsStorageMax, storage_max_gb);
  ipfs::IpfsService* service =
      ipfs::IpfsServiceFactory::GetForContext(profile_);
  if (!service) {
    return;
  }
  if (service->IsDaemonLaunched()) {
    service->RestartDaemon();
  }
}

void adrbrowsielDefaultExtensionsHandler::SetIPFSCompanionEnabled(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);
  bool enabled;
  args->GetBoolean(0, &enabled);

  extensions::ExtensionService* service =
      extensions::ExtensionSystem::Get(profile_)->extension_service();
  if (enabled) {
    if (!IsExtensionInstalled(ipfs_companion_extension_id)) {
      // Using FindLastActiveWithProfile() here will be fine. Of course, it can
      // return NULL but only return NULL when there was no activated window
      // with |profile_| so far. But, it's impossible at here because user can't
      // request ipfs install request w/o activating browser.
      scoped_refptr<extensions::WebstoreInstallWithPrompt> installer =
          new extensions::WebstoreInstallWithPrompt(
              ipfs_companion_extension_id, profile_,
              chrome::FindLastActiveWithProfile(profile_)
                  ->window()
                  ->GetNativeWindow(),
              base::BindOnce(&adrbrowsielDefaultExtensionsHandler::OnInstallResult,
                             weak_ptr_factory_.GetWeakPtr(),
                             kIPFSCompanionEnabled));
      installer->BeginInstall();
    }
    service->EnableExtension(ipfs_companion_extension_id);
  } else {
    service->DisableExtension(
        ipfs_companion_extension_id,
        extensions::disable_reason::DisableReason::DISABLE_USER_ACTION);
  }
}

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
void adrbrowsielDefaultExtensionsHandler::SetadrbrowsielWalletEnabled(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);
  bool enabled;
  args->GetBoolean(0, &enabled);

  extensions::ExtensionService* service =
      extensions::ExtensionSystem::Get(profile_)->extension_service();
  if (enabled) {
    service->EnableExtension(ethereum_remote_client_extension_id);
  } else {
    service->DisableExtension(
        ethereum_remote_client_extension_id,
        extensions::disable_reason::DisableReason::DISABLE_USER_ACTION);
  }
}
#endif

void adrbrowsielDefaultExtensionsHandler::IsDecentralizedDnsEnabled(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(
      args->GetList()[0],
#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
      base::Value(decentralized_dns::IsDecentralizedDnsEnabled()));
#else
      base::Value(false));
#endif
}

void adrbrowsielDefaultExtensionsHandler::GetDecentralizedDnsResolveMethodList(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 2U);
  AllowJavascript();

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
  decentralized_dns::Provider provider =
      static_cast<decentralized_dns::Provider>(args->GetList()[1].GetInt());
  ResolveJavascriptCallback(args->GetList()[0],
                            decentralized_dns::GetResolveMethodList(provider));
#else
  ResolveJavascriptCallback(args->GetList()[0],
                            base::Value(base::Value::Type::LIST));
#endif
}

#if BUILDFLAG(IPFS_ENABLED)
void adrbrowsielDefaultExtensionsHandler::FileSelected(const base::FilePath& path,
                                                 int index,
                                                 void* params) {
  ipfs::IpfsService* service =
      ipfs::IpfsServiceFactory::GetForContext(profile_);
  if (!service)
    return;
  service->GetIpnsKeysManager()->ImportKey(
      path, dialog_key_,
      base::BindOnce(&adrbrowsielDefaultExtensionsHandler::OnKeyImported,
                     weak_ptr_factory_.GetWeakPtr()));
  select_file_dialog_.reset();
  dialog_key_.clear();
}

void adrbrowsielDefaultExtensionsHandler::OnKeyImported(const std::string& key,
                                                  const std::string& value,
                                                  bool success) {
  FireWebUIListener("adrbrowsiel-ipfs-key-imported", base::Value(key),
                    base::Value(value), base::Value(success));
}

void adrbrowsielDefaultExtensionsHandler::FileSelectionCanceled(void* params) {
  select_file_dialog_.reset();
  dialog_key_.clear();
}

void adrbrowsielDefaultExtensionsHandler::ImportIpnsKey(const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);
  std::string key_name;
  args->GetString(0, &key_name);
  auto* web_contents = web_ui()->GetWebContents();
  select_file_dialog_ = ui::SelectFileDialog::Create(
      this, std::make_unique<ChromeSelectFilePolicy>(web_contents));
  if (!select_file_dialog_) {
    VLOG(1) << "Export already in progress";
    return;
  }
  Profile* profile =
      Profile::FromBrowserContext(web_contents->GetBrowserContext());
  const base::FilePath directory = profile->last_selected_directory();
  gfx::NativeWindow parent_window = web_contents->GetTopLevelNativeWindow();
  ui::SelectFileDialog::FileTypeInfo file_types;
  file_types.allowed_paths = ui::SelectFileDialog::FileTypeInfo::NATIVE_PATH;
  dialog_key_ = key_name;
  select_file_dialog_->SelectFile(
      ui::SelectFileDialog::SELECT_OPEN_FILE, std::u16string(), directory,
      &file_types, 0, FILE_PATH_LITERAL("key"), parent_window, nullptr);
}

void adrbrowsielDefaultExtensionsHandler::CheckIpfsNodeStatus(
    const base::ListValue* args) {
  NotifyNodeStatus();
}

void adrbrowsielDefaultExtensionsHandler::NotifyNodeStatus() {
  ipfs::IpfsService* service =
      ipfs::IpfsServiceFactory::GetForContext(profile_);
  bool launched = service && service->IsDaemonLaunched();
  FireWebUIListener("adrbrowsiel-ipfs-node-status-changed", base::Value(launched));
}

void adrbrowsielDefaultExtensionsHandler::OnIpfsLaunched(bool result, int64_t pid) {
  if (!IsJavascriptAllowed())
    return;
  NotifyNodeStatus();
}

void adrbrowsielDefaultExtensionsHandler::OnIpfsShutdown() {
  if (!IsJavascriptAllowed())
    return;
  NotifyNodeStatus();
}
void adrbrowsielDefaultExtensionsHandler::OnIpnsKeysLoaded(bool success) {
  if (!IsJavascriptAllowed())
    return;
  FireWebUIListener("adrbrowsiel-ipfs-keys-loaded", base::Value(success));
}
#endif
