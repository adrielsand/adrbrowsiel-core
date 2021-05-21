/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/update_util.h"

#include "base/command_line.h"
#include "adrbrowsiel/common/adrbrowsiel_switches.h"

namespace adrbrowsiel {

bool UpdateEnabled() {
#if defined(OFFICIAL_BUILD)
  return !base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableadrbrowsielUpdate);
#else
  return false;
#endif
}

}  //namespace adrbrowsiel
