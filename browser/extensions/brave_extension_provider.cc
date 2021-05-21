/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/extensions/adrbrowsiel_extension_provider.h"

#include <algorithm>
#include <string>
#include <vector>

#include "base/strings/utf_string_conversions.h"
#include "adrbrowsiel/browser/adrbrowsiel_browser_process.h"
#include "adrbrowsiel/components/adrbrowsiel_component_updater/browser/extension_whitelist_service.h"
#include "adrbrowsiel/components/adrbrowsiel_component_updater/browser/local_data_files_service.h"
#include "adrbrowsiel/grit/adrbrowsiel_generated_resources.h"
#include "extensions/common/constants.h"
#include "ui/base/l10n/l10n_util.h"

namespace {

bool IsBlacklisted(const extensions::Extension* extension) {
  // This is a hardcoded list of extensions to block.
  // Don't add new extensions to this list. Add them to
  // the files managed by the extension whitelist service.
  static std::vector<std::string> blacklisted_extensions(
      {// Used for tests, corresponds to
       // adrbrowsiel/test/data/should-be-blocked-extension.
       "mlklomjnahgiddgfdgjhibinlfibfffc",
     });

  if (std::find(blacklisted_extensions.begin(), blacklisted_extensions.end(),
                extension->id()) != blacklisted_extensions.end())
    return true;

  return g_adrbrowsiel_browser_process->extension_whitelist_service()->IsBlacklisted(
      extension->id());
}

}  // namespace

namespace extensions {

adrbrowsielExtensionProvider::adrbrowsielExtensionProvider() {}

adrbrowsielExtensionProvider::~adrbrowsielExtensionProvider() {}

std::string adrbrowsielExtensionProvider::GetDebugPolicyProviderName() const {
#if defined(NDEBUG)
  NOTREACHED();
  return std::string();
#else
  return "adrbrowsiel Extension Provider";
#endif
}

bool adrbrowsielExtensionProvider::UserMayLoad(const Extension* extension,
                                         std::u16string* error) const {
  if (IsBlacklisted(extension)) {
    if (error) {
      *error = l10n_util::GetStringFUTF16(IDS_EXTENSION_CANT_INSTALL_ON_adrbrowsiel,
                                          base::UTF8ToUTF16(extension->name()),
                                          base::UTF8ToUTF16(extension->id()));
    }
    DVLOG(1) << "Extension will not install "
             << " ID: " << base::UTF8ToUTF16(extension->id()) << ", "
             << " Name: " << base::UTF8ToUTF16(extension->name());
    return false;
  }
  return true;
}

bool adrbrowsielExtensionProvider::MustRemainInstalled(const Extension* extension,
                                                 std::u16string* error) const {
  return extension->id() == adrbrowsiel_extension_id ||
         extension->id() == adrbrowsiel_rewards_extension_id;
}

}  // namespace extensions
