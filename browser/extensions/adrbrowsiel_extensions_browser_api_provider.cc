/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/extensions/adrbrowsiel_extensions_browser_api_provider.h"

#include "adrbrowsiel/common/extensions/api/generated_api_registration.h"
#include "extensions/browser/extension_function_registry.h"

namespace extensions {

adrbrowsielExtensionsBrowserAPIProvider::adrbrowsielExtensionsBrowserAPIProvider() =
    default;
adrbrowsielExtensionsBrowserAPIProvider::~adrbrowsielExtensionsBrowserAPIProvider() =
    default;

void adrbrowsielExtensionsBrowserAPIProvider::RegisterExtensionFunctions(
    ExtensionFunctionRegistry* registry) {
  // Generated APIs from adrbrowsiel.
  api::adrbrowsielGeneratedFunctionRegistry::RegisterAll(registry);
}

}  // namespace extensions
