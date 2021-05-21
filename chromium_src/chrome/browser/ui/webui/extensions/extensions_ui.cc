/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/resources/extensions/grit/adrbrowsiel_extensions_resources.h"
#include "adrbrowsiel/browser/resources/extensions/grit/adrbrowsiel_extensions_resources_map.h"
#include "adrbrowsiel/browser/ui/webui/navigation_bar_data_provider.h"
#include "adrbrowsiel/grit/adrbrowsiel_generated_resources.h"
#include "chrome/common/buildflags.h"
#include "chrome/grit/generated_resources.h"
#include "content/public/browser/web_ui_data_source.h"

namespace extensions {

namespace {

// Called from the original extension_ui.cc's CreateMdExtensionsSource via a
// patch.
void adrbrowsielAddExtensionsResources(content::WebUIDataSource* source) {
#if !BUILDFLAG(OPTIMIZE_WEBUI)
  for (size_t i = 0; i < kadrbrowsielExtensionsResourcesSize; ++i) {
    source->AddResourcePath(kadrbrowsielExtensionsResources[i].path,
                            kadrbrowsielExtensionsResources[i].id);
  }
#endif
  NavigationBarDataProvider::Initialize(source);
  source->AddLocalizedString("privateInfoWarning",
                             IDS_EXTENSIONS_adrbrowsiel_PRIVATE_WARNING);
  source->AddLocalizedString("spanningInfoWarning",
                             IDS_EXTENSIONS_adrbrowsiel_SPANNING_WARNING);
  source->AddLocalizedString("privateAndTorInfoWarning",
                             IDS_EXTENSIONS_adrbrowsiel_PRIVATE_AND_TOR_WARNING);
}

}  // namespace

}  // namespace extensions

// These are defined in generated_resources.h, but since we are including it
// here the original extensions_ui.cc shouldn't include it again and the
// redefined values will be used.
#undef IDS_EXTENSIONS_ITEM_CHROME_WEB_STORE
#define IDS_EXTENSIONS_ITEM_CHROME_WEB_STORE \
  IDS_EXTENSIONS_adrbrowsiel_ITEM_CHROME_WEB_STORE
#undef IDS_EXTENSIONS_ITEM_SOURCE_WEBSTORE
#define IDS_EXTENSIONS_ITEM_SOURCE_WEBSTORE \
  IDS_EXTENSIONS_adrbrowsiel_ITEM_SOURCE_WEBSTORE

#include "../../../../../../../chrome/browser/ui/webui/extensions/extensions_ui.cc"
