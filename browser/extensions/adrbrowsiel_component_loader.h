/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_EXTENSIONS_adrbrowsiel_COMPONENT_LOADER_H_
#define adrbrowsiel_BROWSER_EXTENSIONS_adrbrowsiel_COMPONENT_LOADER_H_

#include <string>

#include "base/files/file_path.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/buildflags/buildflags.h"
#include "chrome/browser/extensions/component_loader.h"
#include "components/prefs/pref_change_registrar.h"

class PrefService;
class Profile;

namespace adrbrowsiel_rewards {
class RewardsService;
}


namespace extensions {

// For registering, loading, and unloading component extensions.
class adrbrowsielComponentLoader : public ComponentLoader {
 public:
  adrbrowsielComponentLoader(ExtensionSystem* extension_system,
                       Profile* browser_context);
  ~adrbrowsielComponentLoader() override;

  // Adds the default component extensions. If |skip_session_components|
  // the loader will skip loading component extensions that weren't supposed to
  // be loaded unless we are in signed user session (ChromeOS). For all other
  // platforms this |skip_session_components| is expected to be unset.
  void AddDefaultComponentExtensions(bool skip_session_components) override;
  void OnComponentRegistered(std::string extension_id);

#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
  void AddRewardsExtension();
#endif
#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
  void AddEthereumRemoteClientExtension();
  void AddEthereumRemoteClientExtensionOnStartup();
#endif
  void AddWebTorrentExtension();
  void OnComponentReady(std::string extension_id,
    bool allow_file_access,
    const base::FilePath& install_dir,
    const std::string& manifest);
  void AddExtension(const std::string& id,
      const std::string& name, const std::string& public_key);
  // ForceAddHangoutServicesExtension ignores whether or not a preference for
  // hangouts is set.  If the buildflag is not set, it won't add though.
  void ForceAddHangoutServicesExtension();

 private:
#if BUILDFLAG(ENABLE_HANGOUT_SERVICES_EXTENSION)
  void AddHangoutServicesExtension() override;
#endif  // BUILDFLAG(ENABLE_HANGOUT_SERVICES_EXTENSION)

#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
  void CheckRewardsStatus();
#endif

  void ReinstallAsNonComponent(std::string extension_id);

  Profile* profile_;
  PrefService* profile_prefs_;
  PrefChangeRegistrar pref_change_registrar_;
  std::string ethereum_remote_client_manifest_;
  base::FilePath ethereum_remote_client_install_dir_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielComponentLoader);
};

}  // namespace extensions

#endif  // adrbrowsiel_BROWSER_EXTENSIONS_adrbrowsiel_COMPONENT_LOADER_H_
