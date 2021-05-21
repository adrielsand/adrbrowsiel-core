/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_IMPORTER_adrbrowsiel_IMPORTER_P3A_H_
#define adrbrowsiel_BROWSER_IMPORTER_adrbrowsiel_IMPORTER_P3A_H_

#include "chrome/common/importer/importer_type.h"

// This mostly duplicates code in |importer_uma.cc| but we want better
// naming, better buckets and slightly different logic.
void RecordImporterP3A(importer::ImporterType type);

#endif  // adrbrowsiel_BROWSER_IMPORTER_adrbrowsiel_IMPORTER_P3A_H_
