// Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef adrbrowsiel_BROWSER_EXTENSIONS_UPDATER_adrbrowsiel_UPDATE_CLIENT_CONFIG_H_
#define adrbrowsiel_BROWSER_EXTENSIONS_UPDATER_adrbrowsiel_UPDATE_CLIENT_CONFIG_H_

#include <string>

#include "base/macros.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_refptr.h"
#include "base/optional.h"
#include "chrome/browser/extensions/updater/chrome_update_client_config.h"

class GURL;

namespace content {
class BrowserContext;
}

namespace update_client {
class NetworkFetcherFactory;
}

namespace extensions {

class ExtensionUpdateClientBaseTest;

class adrbrowsielUpdateClientConfig : public ChromeUpdateClientConfig {
 public:
  static scoped_refptr<ChromeUpdateClientConfig> Create(
      content::BrowserContext* context,
      base::Optional<GURL> url_override);

  using ChromeUpdateClientConfig::ChromeUpdateClientConfig;

  std::string GetChannel() const override;
  scoped_refptr<update_client::NetworkFetcherFactory> GetNetworkFetcherFactory()
      override;
  bool EnabledCupSigning() const override;

 protected:
  friend class base::RefCountedThreadSafe<adrbrowsielUpdateClientConfig>;
  friend class ExtensionUpdateClientBaseTest;

  ~adrbrowsielUpdateClientConfig() override;

 private:
  DISALLOW_COPY_AND_ASSIGN(adrbrowsielUpdateClientConfig);
};

}  // namespace extensions

#endif  // adrbrowsiel_BROWSER_EXTENSIONS_UPDATER_adrbrowsiel_UPDATE_CLIENT_CONFIG_H_
