/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved. 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/version_info/version_info.h"

#define GetVersionNumber GetadrbrowsielVersionNumberForDisplay
#include "../../../../../../chrome/browser/ui/webui/crashes_ui.cc"
#undef GetVersionNumber
