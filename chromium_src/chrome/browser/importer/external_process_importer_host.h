/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_IMPORTER_EXTERNAL_PROCESS_IMPORTER_HOST_H_
#define adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_IMPORTER_EXTERNAL_PROCESS_IMPORTER_HOST_H_

#define adrbrowsiel_EXTERNAL_PROCESS_IMPORTER_HOST_H \
  friend class adrbrowsielExternalProcessImporterHost;

#define NotifyImportEnded virtual NotifyImportEnded
#include "../../../../../chrome/browser/importer/external_process_importer_host.h"
#undef adrbrowsiel_EXTERNAL_PROCESS_IMPORTER_HOST_H
#undef NotifyImportEnded

#endif  // adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_IMPORTER_EXTERNAL_PROCESS_IMPORTER_HOST_H_
