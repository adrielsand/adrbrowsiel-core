/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_EXTENSIONS_adrbrowsiel_EXTENSION_MANAGEMENT_H_
#define adrbrowsiel_BROWSER_EXTENSIONS_adrbrowsiel_EXTENSION_MANAGEMENT_H_

#include "base/scoped_observer.h"
#include "chrome/browser/extensions/extension_management.h"
#include "components/prefs/pref_change_registrar.h"
#include "extensions/browser/extension_registry.h"
#include "extensions/browser/extension_registry_observer.h"

namespace extensions {

class adrbrowsielExtensionManagement : public ExtensionManagement,
                                 public ExtensionRegistryObserver {
 public:
  explicit adrbrowsielExtensionManagement(Profile* profile);
  ~adrbrowsielExtensionManagement() override;

 private:
  // ExtensionRegistryObserver implementation.
  void OnExtensionLoaded(
      content::BrowserContext* browser_context,
      const Extension* extension) override;
  void OnExtensionUnloaded(
      content::BrowserContext* browser_context,
      const Extension* extension,
      UnloadedExtensionReason reason) override;

  void OnTorDisabledChanged();
  void Cleanup(content::BrowserContext* browser_context);

  PrefChangeRegistrar local_state_pref_change_registrar_;

  ScopedObserver<ExtensionRegistry, ExtensionRegistryObserver>
    extension_registry_observer_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielExtensionManagement);
};

}  // namespace extensions

#endif  // adrbrowsiel_BROWSER_EXTENSIONS_adrbrowsiel_EXTENSION_MANAGEMENT_H_
