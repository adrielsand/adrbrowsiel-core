/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/extensions/api/adrbrowsiel_theme_api.h"
#include "adrbrowsiel/browser/themes/adrbrowsiel_dark_mode_utils.h"
#include "adrbrowsiel/common/pref_names.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/browser/extensions/extension_function_test_utils.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_test.h"
#include "extensions/common/extension_builder.h"

using extensions::api::adrbrowsielThemeGetadrbrowsielThemeTypeFunction;
using extension_function_test_utils::RunFunctionAndReturnSingleResult;

class adrbrowsielThemeAPIBrowserTest : public InProcessBrowserTest {
 public:
  void SetUpOnMainThread() override {
    InProcessBrowserTest::SetUpOnMainThread();
    extension_ = extensions::ExtensionBuilder("Test").Build();
  }

  scoped_refptr<const extensions::Extension> extension() {
    return extension_;
  }

 private:
  scoped_refptr<const extensions::Extension> extension_;
};

IN_PROC_BROWSER_TEST_F(adrbrowsielThemeAPIBrowserTest,
                       adrbrowsielThemeGetadrbrowsielThemeTypeTest) {
  // Change to Light type and check it from api.
  dark_mode::SetadrbrowsielDarkModeType(
      dark_mode::adrbrowsielDarkModeType::adrbrowsiel_DARK_MODE_TYPE_LIGHT);
  EXPECT_EQ(dark_mode::adrbrowsielDarkModeType::adrbrowsiel_DARK_MODE_TYPE_LIGHT,
            dark_mode::GetActiveadrbrowsielDarkModeType());

  scoped_refptr<adrbrowsielThemeGetadrbrowsielThemeTypeFunction> get_function(
      new adrbrowsielThemeGetadrbrowsielThemeTypeFunction());
  get_function->set_extension(extension().get());
  std::unique_ptr<base::Value> value;
  value.reset(RunFunctionAndReturnSingleResult(get_function.get(),
                                               std::string("[]"),
                                               browser()));
  EXPECT_EQ(value->GetString(), "Light");
}
