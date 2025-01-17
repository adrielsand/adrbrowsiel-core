/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_EXTENSIONS_adrbrowsiel_EXTENSION_PROVIDER_H_
#define adrbrowsiel_BROWSER_EXTENSIONS_adrbrowsiel_EXTENSION_PROVIDER_H_

#include <string>

#include "extensions/browser/management_policy.h"

namespace extensions {

class adrbrowsielExtensionProvider : public ManagementPolicy::Provider {
 public:
  adrbrowsielExtensionProvider();
  ~adrbrowsielExtensionProvider() override;
  std::string GetDebugPolicyProviderName() const override;
  bool UserMayLoad(const Extension* extension,
                   std::u16string* error) const override;
  bool MustRemainInstalled(const Extension* extension,
                           std::u16string* error) const override;

 private:
  DISALLOW_COPY_AND_ASSIGN(adrbrowsielExtensionProvider);
};

}  // namespace extensions

#endif  // adrbrowsiel_BROWSER_EXTENSIONS_adrbrowsiel_EXTENSION_PROVIDER_H_
