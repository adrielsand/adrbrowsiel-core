/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_EXTENSIONS_adrbrowsiel_EXTENSIONS_BROWSER_CLIENT_IMPL_H_
#define adrbrowsiel_BROWSER_EXTENSIONS_adrbrowsiel_EXTENSIONS_BROWSER_CLIENT_IMPL_H_

#include "chrome/browser/extensions/chrome_extensions_browser_client.h"

namespace extensions {

class adrbrowsielExtensionsBrowserClientImpl : public ChromeExtensionsBrowserClient {
 public:
  adrbrowsielExtensionsBrowserClientImpl();
  ~adrbrowsielExtensionsBrowserClientImpl() override = default;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielExtensionsBrowserClientImpl);
};

}  // namespace extensions

#endif  // adrbrowsiel_BROWSER_EXTENSIONS_adrbrowsiel_EXTENSIONS_BROWSER_CLIENT_IMPL_H_
