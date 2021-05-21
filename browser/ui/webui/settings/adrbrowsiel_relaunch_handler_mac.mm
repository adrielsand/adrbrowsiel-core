/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/webui/settings/adrbrowsiel_relaunch_handler_mac.h"

#include "base/bind.h"
#import "adrbrowsiel/browser/mac/sparkle_glue.h"

void adrbrowsielRelaunchHandler::RegisterMessages() {
  web_ui()->RegisterMessageCallback(
      "relaunchOnMac",
      base::BindRepeating(&adrbrowsielRelaunchHandler::Relaunch,
                          base::Unretained(this)));
}

void adrbrowsielRelaunchHandler::Relaunch(const base::ListValue* args) {
  [[SparkleGlue sharedSparkleGlue] relaunch];
}
