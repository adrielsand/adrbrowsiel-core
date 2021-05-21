/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_WEBUI_SETTINGS_IMPORT_DATA_HANDLER_H_
#define adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_WEBUI_SETTINGS_IMPORT_DATA_HANDLER_H_

#define adrbrowsiel_IMPORT_DATA_HANDLER_H \
 private: \
  friend class adrbrowsielImportDataHandler;

#define StartImport virtual StartImport

#include "../../../../../../../chrome/browser/ui/webui/settings/import_data_handler.h"

#undef adrbrowsiel_IMPORT_DATA_HANDLER_H
#undef StartImport

#endif  // adrbrowsiel_CHROMIUM_SRC_CHROME_BROWSER_UI_WEBUI_SETTINGS_IMPORT_DATA_HANDLER_H_
