/* Copyright 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/importer/adrbrowsiel_external_process_importer_client.h"
#include "adrbrowsiel/browser/importer/adrbrowsiel_in_process_importer_bridge.h"

#define ExternalProcessImporterClient adrbrowsielExternalProcessImporterClient
#define InProcessImporterBridge adrbrowsielInProcessImporterBridge
#include "../../../../../chrome/browser/importer/external_process_importer_host.cc"
#undef ExternalProcessImporterClient
#undef InProcessImporterBridge
