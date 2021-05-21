/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/extensions/adrbrowsiel_extension_management.h"

#include <memory>

#include "adrbrowsiel/browser/adrbrowsiel_browser_process.h"
#include "adrbrowsiel/browser/extensions/adrbrowsiel_extension_provider.h"
#include "adrbrowsiel/browser/tor/tor_profile_service_factory.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/buildflags/buildflags.h"
#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"
#include "adrbrowsiel/components/tor/buildflags/buildflags.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/extensions/extension_management_internal.h"
#include "chrome/browser/extensions/external_policy_loader.h"
#include "chrome/browser/profiles/profile.h"
#include "components/prefs/pref_service.h"
#include "extensions/browser/extension_registry.h"
#include "extensions/common/constants.h"
#include "extensions/common/extension.h"
#include "extensions/common/extension_urls.h"

#if BUILDFLAG(ENABLE_TOR)
#include "adrbrowsiel/components/tor/adrbrowsiel_tor_client_updater.h"
#include "adrbrowsiel/components/tor/pref_names.h"
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "adrbrowsiel/components/ipfs/adrbrowsiel_ipfs_client_updater.h"
#include "adrbrowsiel/components/ipfs/ipfs_utils.h"
#endif

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/adrbrowsiel_wallet_constants.h"
#endif

namespace extensions {

adrbrowsielExtensionManagement::adrbrowsielExtensionManagement(Profile* profile)
    : ExtensionManagement(profile), extension_registry_observer_(this) {
  extension_registry_observer_.Add(
      ExtensionRegistry::Get(static_cast<content::BrowserContext*>(profile)));
  providers_.push_back(std::make_unique<adrbrowsielExtensionProvider>());
  local_state_pref_change_registrar_.Init(g_browser_process->local_state());
#if BUILDFLAG(ENABLE_TOR)
  local_state_pref_change_registrar_.Add(
      tor::prefs::kTorDisabled,
      base::BindRepeating(&adrbrowsielExtensionManagement::OnTorDisabledChanged,
                          base::Unretained(this)));
#endif
  // Make IsInstallationExplicitlyAllowed to be true
#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
  AccessById(ethereum_remote_client_extension_id)->installation_mode =
      INSTALLATION_RECOMMENDED;
#endif
  Cleanup(profile);
}

adrbrowsielExtensionManagement::~adrbrowsielExtensionManagement() {
  local_state_pref_change_registrar_.RemoveAll();
}

void adrbrowsielExtensionManagement::OnExtensionLoaded(
    content::BrowserContext* browser_context,
    const Extension* extension) {
  if (extension->id() == ipfs_companion_extension_id)
    pref_service_->SetBoolean(kIPFSCompanionEnabled, true);
}

void adrbrowsielExtensionManagement::OnExtensionUnloaded(
    content::BrowserContext* browser_context,
    const Extension* extension,
    UnloadedExtensionReason reason) {
  if (extension->id() == ipfs_companion_extension_id)
    pref_service_->SetBoolean(kIPFSCompanionEnabled, false);
}

void adrbrowsielExtensionManagement::OnTorDisabledChanged() {
#if BUILDFLAG(ENABLE_TOR)
  if (TorProfileServiceFactory::IsTorDisabled())
    g_adrbrowsiel_browser_process->tor_client_updater()->Cleanup();
#endif
}

void adrbrowsielExtensionManagement::Cleanup(content::BrowserContext* context) {
  // BrowserPolicyConnector enforce policy earlier than this constructor so we
  // have to manully cleanup tor executable when tor is disabled by gpo
  OnTorDisabledChanged();

#if BUILDFLAG(IPFS_ENABLED)
  // Remove ipfs executable if it is disabled by GPO.
  if (ipfs::IsIpfsDisabledByPolicy(context))
    g_adrbrowsiel_browser_process->ipfs_client_updater()->Cleanup();
#endif
}

}  // namespace extensions
