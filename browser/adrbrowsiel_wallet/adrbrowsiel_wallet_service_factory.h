/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_adrbrowsiel_WALLET_adrbrowsiel_WALLET_SERVICE_FACTORY_H_
#define adrbrowsiel_BROWSER_adrbrowsiel_WALLET_adrbrowsiel_WALLET_SERVICE_FACTORY_H_

#include "base/memory/singleton.h"
#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

class adrbrowsielWalletService;

class adrbrowsielWalletServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  static adrbrowsielWalletService* GetForContext(content::BrowserContext* context);
  static adrbrowsielWalletServiceFactory* GetInstance();

 private:
  friend struct base::DefaultSingletonTraits<adrbrowsielWalletServiceFactory>;

  adrbrowsielWalletServiceFactory();
  ~adrbrowsielWalletServiceFactory() override;

  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const override;
  content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const override;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielWalletServiceFactory);
};

#endif  // adrbrowsiel_BROWSER_adrbrowsiel_WALLET_adrbrowsiel_WALLET_SERVICE_FACTORY_H_
