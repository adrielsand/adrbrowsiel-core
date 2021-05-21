/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/extensions/api/adrbrowsiel_theme_api.h"

#include <memory>
#include <string>

#include "base/json/json_writer.h"
#include "base/values.h"
#include "adrbrowsiel/browser/themes/adrbrowsiel_dark_mode_utils.h"
#include "adrbrowsiel/common/extensions/api/adrbrowsiel_theme.h"

namespace extensions {
namespace api {

ExtensionFunction::ResponseAction adrbrowsielThemeGetadrbrowsielThemeListFunction::Run() {
  std::string json_string;
  base::JSONWriter::Write(dark_mode::GetadrbrowsielDarkModeTypeList(), &json_string);
  return RespondNow(OneArgument(base::Value(json_string)));
}

ExtensionFunction::ResponseAction adrbrowsielThemeGetadrbrowsielThemeTypeFunction::Run() {
  const std::string theme_type =
      dark_mode::GetStringFromadrbrowsielDarkModeType(
          dark_mode::GetActiveadrbrowsielDarkModeType());
  return RespondNow(OneArgument(base::Value(theme_type)));
}

ExtensionFunction::ResponseAction adrbrowsielThemeSetadrbrowsielThemeTypeFunction::Run() {
  std::unique_ptr<adrbrowsiel_theme::SetadrbrowsielThemeType::Params> params(
      adrbrowsiel_theme::SetadrbrowsielThemeType::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  dark_mode::SetadrbrowsielDarkModeType(params->type);

  return RespondNow(NoArguments());
}

}  // namespace api
}  // namespace extensions
