// Copyright (c) 2019 The adrbrowsiel Authors
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "adrbrowsiel/browser/ui/webui/navigation_bar_data_provider.h"
#include "adrbrowsiel/grit/adrbrowsiel_generated_resources.h"
#include "content/public/browser/web_ui_data_source.h"

#define adrbrowsiel_CREATE_BOOKMARKS_UI_HTML_SOURCE \
  NavigationBarDataProvider::Initialize(source); \
  source->AddLocalizedString( \
      "emptyList", IDS_adrbrowsiel_BOOKMARK_MANAGER_EMPTY_LIST);

#include "../../../../../../../chrome/browser/ui/webui/bookmarks/bookmarks_ui.cc"

#undef adrbrowsiel_CREATE_BOOKMARKS_UI_HTML_SOURCE
