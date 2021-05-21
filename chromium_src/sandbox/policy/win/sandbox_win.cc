/* Copyright 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/process/launch.h"
#include "sandbox/policy/switches.h"

namespace sandbox {
namespace policy {
namespace {

void adrbrowsielLaunchOption(base::CommandLine* cmd_line,
                       base::LaunchOptions* options) {
  std::string process_type =
      cmd_line->GetSwitchValueASCII(switches::kProcessType);
  if (process_type == switches::kUtilityProcess)
    options->start_hidden = true;
}

}  // namespace
}  // namespace policy
}  // namespace sandbox

#define adrbrowsiel_START_SANDBOXED_PROCESS adrbrowsielLaunchOption(cmd_line, &options);
#include "../../../../../sandbox/policy/win/sandbox_win.cc"
#undef adrbrowsiel_START_SANDBOXED_PROCESS
