/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/extensions/adrbrowsiel_component_loader.h"
#include "chrome/browser/extensions/component_loader.h"
#include "chrome/browser/extensions/extension_service.h"

#define ComponentLoader adrbrowsielComponentLoader
#include "../../../../../chrome/browser/extensions/extension_service.cc"
#undef ComponentLoader
