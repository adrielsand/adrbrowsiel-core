/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/extensions/adrbrowsiel_component_loader.h"

#include <string>

#include "base/bind.h"
#include "base/command_line.h"
#include "bat/ads/pref_names.h"
#include "adrbrowsiel/browser/component_updater/adrbrowsiel_component_installer.h"
#include "adrbrowsiel/common/adrbrowsiel_switches.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_ads/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_component_updater/browser/adrbrowsiel_on_demand_updater.h"
#include "adrbrowsiel/components/adrbrowsiel_extension/grit/adrbrowsiel_extension.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/resources/extension/grit/adrbrowsiel_rewards_extension_resources.h"
#include "adrbrowsiel/components/adrbrowsiel_webtorrent/grit/adrbrowsiel_webtorrent_resources.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/extensions/extension_service.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/pref_names.h"
#include "components/grit/adrbrowsiel_components_resources.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/prefs/pref_service.h"
#include "extensions/browser/extension_prefs.h"
#include "extensions/browser/extension_registry.h"
#include "extensions/browser/extension_system.h"
#include "extensions/common/constants.h"
#include "extensions/common/mojom/manifest.mojom.h"

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
#include "adrbrowsiel/browser/extensions/adrbrowsiel_wallet_util.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/adrbrowsiel_wallet_constants.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/pref_names.h"
#endif

using extensions::mojom::ManifestLocation;

namespace extensions {

adrbrowsielComponentLoader::adrbrowsielComponentLoader(ExtensionSystem* extension_system,
                                           Profile* profile)
    : ComponentLoader(extension_system, profile),
      profile_(profile),
      profile_prefs_(profile->GetPrefs()) {
#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
  pref_change_registrar_.Init(profile_prefs_);
  pref_change_registrar_.Add(
      adrbrowsiel_rewards::prefs::kAutoContributeEnabled,
      base::Bind(&adrbrowsielComponentLoader::CheckRewardsStatus,
                 base::Unretained(this)));
#endif
}

adrbrowsielComponentLoader::~adrbrowsielComponentLoader() {}

void adrbrowsielComponentLoader::OnComponentRegistered(std::string extension_id) {
  adrbrowsiel_component_updater::adrbrowsielOnDemandUpdater::GetInstance()->OnDemandUpdate(
      extension_id);
}

void adrbrowsielComponentLoader::OnComponentReady(std::string extension_id,
                                            bool allow_file_access,
                                            const base::FilePath& install_dir,
                                            const std::string& manifest) {
  Add(manifest, install_dir);
  if (allow_file_access) {
    ExtensionPrefs::Get(profile_)->SetAllowFileAccess(extension_id, true);
  }
#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
  if (extension_id == ethereum_remote_client_extension_id) {
    ReinstallAsNonComponent(ethereum_remote_client_extension_id);
  }
#endif
}

void adrbrowsielComponentLoader::ReinstallAsNonComponent(
    const std::string extension_id) {
  extensions::ExtensionService* service =
      extensions::ExtensionSystem::Get(profile_)->extension_service();
  extensions::ExtensionRegistry* registry =
      extensions::ExtensionRegistry::Get(profile_);
  const Extension* extension = registry->GetInstalledExtension(extension_id);
  DCHECK(extension);
  if (extension->location() == ManifestLocation::kComponent) {
    service->RemoveComponentExtension(extension_id);
    std::string error;
    scoped_refptr<Extension> normal_extension = Extension::Create(
        extension->path(), ManifestLocation::kExternalPref,
        *extension->manifest()->value(), extension->creation_flags(), &error);
    service->AddExtension(normal_extension.get());
  }
}

void adrbrowsielComponentLoader::AddExtension(const std::string& extension_id,
                                        const std::string& name,
                                        const std::string& public_key) {
  adrbrowsiel::RegisterComponent(
      g_browser_process->component_updater(), name, public_key,
      base::Bind(&adrbrowsielComponentLoader::OnComponentRegistered,
                 base::Unretained(this), extension_id),
      base::Bind(&adrbrowsielComponentLoader::OnComponentReady,
                 base::Unretained(this), extension_id, true));
}

void adrbrowsielComponentLoader::AddHangoutServicesExtension() {
  if (!profile_prefs_->FindPreference(kHangoutsEnabled) ||
      profile_prefs_->GetBoolean(kHangoutsEnabled)) {
    ForceAddHangoutServicesExtension();
  }
}

void adrbrowsielComponentLoader::ForceAddHangoutServicesExtension() {
  ComponentLoader::AddHangoutServicesExtension();
}

void adrbrowsielComponentLoader::AddDefaultComponentExtensions(
    bool skip_session_components) {
  ComponentLoader::AddDefaultComponentExtensions(skip_session_components);

  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (!command_line.HasSwitch(switches::kDisableadrbrowsielExtension)) {
    base::FilePath adrbrowsiel_extension_path(FILE_PATH_LITERAL(""));
    adrbrowsiel_extension_path =
        adrbrowsiel_extension_path.Append(FILE_PATH_LITERAL("adrbrowsiel_extension"));
    Add(IDR_adrbrowsiel_EXTENSION, adrbrowsiel_extension_path);
  }

#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
  // Enable rewards extension if already opted-in
  CheckRewardsStatus();
#endif
}

#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
void adrbrowsielComponentLoader::AddRewardsExtension() {
  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (!command_line.HasSwitch(switches::kDisableadrbrowsielRewardsExtension) &&
      !Exists(adrbrowsiel_rewards_extension_id)) {
    base::FilePath adrbrowsiel_rewards_path(FILE_PATH_LITERAL(""));
    adrbrowsiel_rewards_path =
        adrbrowsiel_rewards_path.Append(FILE_PATH_LITERAL("adrbrowsiel_rewards"));
    Add(IDR_adrbrowsiel_REWARDS, adrbrowsiel_rewards_path);
  }
}

void adrbrowsielComponentLoader::CheckRewardsStatus() {
  const bool is_ac_enabled =
      profile_prefs_->GetBoolean(adrbrowsiel_rewards::prefs::kAutoContributeEnabled);

  if (is_ac_enabled) {
    AddRewardsExtension();
  }
}
#endif

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
void adrbrowsielComponentLoader::AddEthereumRemoteClientExtension() {
  AddExtension(ethereum_remote_client_extension_id,
               ethereum_remote_client_extension_name,
               ethereum_remote_client_extension_public_key);
}

void adrbrowsielComponentLoader::AddEthereumRemoteClientExtensionOnStartup() {
  // Only load if the eagerly load Crypto Wallets setting is on and there is a
  // project id configured in the build.
  if (HasInfuraProjectID() &&
      profile_prefs_->GetBoolean(kLoadCryptoWalletsOnStartup)) {
    AddEthereumRemoteClientExtension();
  }
}
#endif

void adrbrowsielComponentLoader::AddWebTorrentExtension() {
  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (!command_line.HasSwitch(switches::kDisableWebTorrentExtension) &&
      (!profile_prefs_->FindPreference(kWebTorrentEnabled) ||
       profile_prefs_->GetBoolean(kWebTorrentEnabled))) {
    base::FilePath adrbrowsiel_webtorrent_path(FILE_PATH_LITERAL(""));
    adrbrowsiel_webtorrent_path =
        adrbrowsiel_webtorrent_path.Append(FILE_PATH_LITERAL("adrbrowsiel_webtorrent"));
    Add(IDR_adrbrowsiel_WEBTORRENT, adrbrowsiel_webtorrent_path);
  }
}

}  // namespace extensions
