/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ipfs/ipfs_service_factory.h"

#include "base/path_service.h"
#include "adrbrowsiel/browser/adrbrowsiel_browser_process.h"
#include "adrbrowsiel/browser/profiles/profile_util.h"
#include "adrbrowsiel/components/ipfs/ipfs_service.h"
#include "adrbrowsiel/components/ipfs/ipfs_utils.h"
#include "chrome/common/channel_info.h"
#include "chrome/common/chrome_paths.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"
#include "extensions/buildflags/buildflags.h"

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "extensions/browser/extension_registry_factory.h"
#include "extensions/browser/extension_system.h"
#include "extensions/browser/extension_system_provider.h"
#include "extensions/browser/extensions_browser_client.h"
#endif

namespace ipfs {

// static
IpfsServiceFactory* IpfsServiceFactory::GetInstance() {
  return base::Singleton<IpfsServiceFactory>::get();
}

// static
IpfsService* IpfsServiceFactory::GetForContext(
    content::BrowserContext* context) {
  if (!adrbrowsiel::IsRegularProfile(context) || !IsIpfsEnabled(context))
    return nullptr;

  return static_cast<IpfsService*>(
      GetInstance()->GetServiceForBrowserContext(context, true));
}

IpfsServiceFactory::IpfsServiceFactory()
    : BrowserContextKeyedServiceFactory(
          "IpfsService",
          BrowserContextDependencyManager::GetInstance()) {
#if BUILDFLAG(ENABLE_EXTENSIONS)
  DependsOn(extensions::ExtensionRegistryFactory::GetInstance());
  DependsOn(
      extensions::ExtensionsBrowserClient::Get()->GetExtensionSystemFactory());
#endif
}

IpfsServiceFactory::~IpfsServiceFactory() {}

KeyedService* IpfsServiceFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  base::FilePath user_data_dir;
  base::PathService::Get(chrome::DIR_USER_DATA, &user_data_dir);
  return new IpfsService(context,
                         g_adrbrowsiel_browser_process
                             ? g_adrbrowsiel_browser_process->ipfs_client_updater()
                             : nullptr,
                         user_data_dir, chrome::GetChannel());
}

}  // namespace ipfs
