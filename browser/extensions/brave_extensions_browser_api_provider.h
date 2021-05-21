/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_EXTENSIONS_adrbrowsiel_EXTENSIONS_BROWSER_API_PROVIDER_H_
#define adrbrowsiel_BROWSER_EXTENSIONS_adrbrowsiel_EXTENSIONS_BROWSER_API_PROVIDER_H_

#include "base/macros.h"
#include "extensions/browser/extensions_browser_api_provider.h"

namespace extensions {

class adrbrowsielExtensionsBrowserAPIProvider : public ExtensionsBrowserAPIProvider {
 public:
  adrbrowsielExtensionsBrowserAPIProvider();
  ~adrbrowsielExtensionsBrowserAPIProvider() override;

  void RegisterExtensionFunctions(ExtensionFunctionRegistry* registry) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(adrbrowsielExtensionsBrowserAPIProvider);
};

}  // namespace extensions

#endif  // adrbrowsiel_BROWSER_EXTENSIONS_adrbrowsiel_EXTENSIONS_BROWSER_API_PROVIDER_H_
