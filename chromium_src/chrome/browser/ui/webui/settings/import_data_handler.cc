/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/importer/adrbrowsiel_external_process_importer_host.h"
#include "adrbrowsiel/common/pref_names.h"

#define ExternalProcessImporterHost adrbrowsielExternalProcessImporterHost

// NOTE: When we add new import feature, also consider to add it for importing
// in welcome page.
// See IMPORT_BROWSER_DATA_REQUESTED action in welcome_reducer.ts.
#define adrbrowsiel_IMPORT_DATA                               \
  if (*types->FindBoolKey(kImportDialogExtensions))     \
    selected_items |= importer::EXTENSIONS;             \
  if (*types->FindBoolKey(kImportDialogPayments))       \
    selected_items |= importer::PAYMENTS;

#define adrbrowsiel_SEND_BROWSER_PROFILE_DATA                                        \
  browser_profile->SetBoolean("extensions",                                    \
                              (browser_services & importer::EXTENSIONS) != 0); \
  browser_profile->SetBoolean("payments",                                      \
                              (browser_services & importer::PAYMENTS) != 0);

#include "../../../../../../../chrome/browser/ui/webui/settings/import_data_handler.cc"
#undef ExternalProcessImporterHost
#undef adrbrowsiel_IMPORT_DATA
#undef adrbrowsiel_SEND_BROWSER_PROFILE_DATA
