// Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef adrbrowsiel_BROWSER_EXTENSIONS_API_adrbrowsiel_TODAY_API_H_
#define adrbrowsiel_BROWSER_EXTENSIONS_API_adrbrowsiel_TODAY_API_H_

#include "extensions/browser/extension_function.h"

namespace extensions {
namespace api {

class adrbrowsielTodayGetHostnameFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielToday.getHostname", UNKNOWN)

 protected:
  ~adrbrowsielTodayGetHostnameFunction() override {}

  ResponseAction Run() override;
};

class adrbrowsielTodayGetRegionUrlPartFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielToday.getRegionUrlPart", UNKNOWN)

 protected:
  ~adrbrowsielTodayGetRegionUrlPartFunction() override {}

  ResponseAction Run() override;
};

}  // namespace api
}  // namespace extensions

#endif  // adrbrowsiel_BROWSER_EXTENSIONS_API_adrbrowsiel_TODAY_API_H_
