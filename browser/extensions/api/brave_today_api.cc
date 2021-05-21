// Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "adrbrowsiel/browser/extensions/api/adrbrowsiel_today_api.h"

#include <memory>
#include <string>

#include "base/json/json_writer.h"
#include "base/values.h"
#include "adrbrowsiel/common/extensions/api/adrbrowsiel_theme.h"
#include "adrbrowsiel/components/adrbrowsiel_today/browser/urls.h"

namespace extensions {
namespace api {

ExtensionFunction::ResponseAction adrbrowsielTodayGetHostnameFunction::Run() {
  return RespondNow(OneArgument(base::Value(adrbrowsiel_today::GetHostname())));
}

ExtensionFunction::ResponseAction adrbrowsielTodayGetRegionUrlPartFunction::Run() {
  return RespondNow(OneArgument(base::Value(adrbrowsiel_today::GetRegionUrlPart())));
}

}  // namespace api
}  // namespace extensions
