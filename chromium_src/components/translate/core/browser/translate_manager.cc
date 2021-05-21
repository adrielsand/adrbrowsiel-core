/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "components/translate/core/browser/translate_manager.h"

#define HasAPIKeyConfigured adrbrowsielHasAPIKeyConfigured
#include "../../../../../../components/translate/core/browser/translate_manager.cc" // NOLINT
#undef HasAPIKeyConfigured

namespace google_apis {

bool adrbrowsielHasAPIKeyConfigured() {
  // Google API key is not used in adrbrowsiel for translation service, always return
  // true for the API key check so the flow won't be blocked because of missing
  // keys.
  return true;
}

}  // namespace google_apis
