/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/app/adrbrowsiel_main_delegate.h"

#define ChromeMainDelegate adrbrowsielMainDelegate
#include "../../../../chrome/app/chrome_main.cc"
#undef ChromeMainDelegate
