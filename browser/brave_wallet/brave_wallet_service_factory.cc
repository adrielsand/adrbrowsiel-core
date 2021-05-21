/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/adrbrowsiel_wallet/adrbrowsiel_wallet_service_factory.h"

#include <memory>

#include "adrbrowsiel/browser/adrbrowsiel_wallet/adrbrowsiel_wallet_delegate_impl.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/adrbrowsiel_wallet_service.h"
#include "chrome/browser/profiles/incognito_helpers.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"
#include "extensions/buildflags/buildflags.h"

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "extensions/browser/extension_registry_factory.h"
#include "extensions/browser/extension_system.h"
#include "extensions/browser/extension_system_provider.h"
#include "extensions/browser/extensions_browser_client.h"
#endif  // #if BUILDFLAG(ENABLE_EXTENSIONS)

// static
adrbrowsielWalletServiceFactory* adrbrowsielWalletServiceFactory::GetInstance() {
  return base::Singleton<adrbrowsielWalletServiceFactory>::get();
}

// static
adrbrowsielWalletService* adrbrowsielWalletServiceFactory::GetForContext(
    content::BrowserContext* context) {
  return static_cast<adrbrowsielWalletService*>(
      GetInstance()->GetServiceForBrowserContext(context, true));
}

adrbrowsielWalletServiceFactory::adrbrowsielWalletServiceFactory()
    : BrowserContextKeyedServiceFactory(
          "adrbrowsielWalletService",
          BrowserContextDependencyManager::GetInstance()) {
#if BUILDFLAG(ENABLE_EXTENSIONS)
  DependsOn(extensions::ExtensionRegistryFactory::GetInstance());
  DependsOn(
      extensions::ExtensionsBrowserClient::Get()->GetExtensionSystemFactory());
#endif  // #if BUILDFLAG(ENABLE_EXTENSIONS)
}

adrbrowsielWalletServiceFactory::~adrbrowsielWalletServiceFactory() {}

KeyedService* adrbrowsielWalletServiceFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  return new adrbrowsielWalletService(context,
                                std::make_unique<adrbrowsielWalletDelegateImpl>());
}

content::BrowserContext* adrbrowsielWalletServiceFactory::GetBrowserContextToUse(
    content::BrowserContext* context) const {
  return chrome::GetBrowserContextRedirectedInIncognito(context);
}
