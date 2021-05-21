/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/app/adrbrowsiel_command_ids.h"
#include "adrbrowsiel/grit/adrbrowsiel_generated_resources.h"

#define adrbrowsiel_BUILD_FILE_MENU                       \
  Item(IDS_NEW_OFFTHERECORD_WINDOW_TOR)             \
      .command_id(IDC_NEW_OFFTHERECORD_WINDOW_TOR),

#define adrbrowsiel_BUILD_HELP_MENU                         \
  Item(IDS_REPORT_BROKEN_SITE_MAC)                    \
      .command_id(IDC_SHOW_adrbrowsiel_WEBCOMPAT_REPORTER),

#include "../../../../../../chrome/browser/ui/cocoa/main_menu_builder.mm"   //NOLINT
#undef adrbrowsiel_WEBCOMPAT_REPORTER_MENU_ENTRY
