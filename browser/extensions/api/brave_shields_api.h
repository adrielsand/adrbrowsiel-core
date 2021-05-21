/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_EXTENSIONS_API_adrbrowsiel_SHIELDS_API_H_
#define adrbrowsiel_BROWSER_EXTENSIONS_API_adrbrowsiel_SHIELDS_API_H_

#include <memory>
#include <string>
#include <vector>

#include "extensions/browser/extension_function.h"

namespace extensions {
namespace api {

class adrbrowsielShieldsUrlCosmeticResourcesFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.urlCosmeticResources", UNKNOWN)

 protected:
  ~adrbrowsielShieldsUrlCosmeticResourcesFunction() override {}

  ResponseAction Run() override;

 private:
  std::unique_ptr<base::ListValue> GetUrlCosmeticResourcesOnTaskRunner(
      const std::string& url);
  void GetUrlCosmeticResourcesOnUI(std::unique_ptr<base::ListValue> resources);
};

class adrbrowsielShieldsHiddenClassIdSelectorsFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.hiddenClassIdSelectors", UNKNOWN)

 protected:
  ~adrbrowsielShieldsHiddenClassIdSelectorsFunction() override {}

  ResponseAction Run() override;

 private:
  std::unique_ptr<base::ListValue> GetHiddenClassIdSelectorsOnTaskRunner(
      const std::vector<std::string>& classes,
      const std::vector<std::string>& ids,
      const std::vector<std::string>& exceptions);
  void GetHiddenClassIdSelectorsOnUI(
      std::unique_ptr<base::ListValue> selectors);
};

class adrbrowsielShieldsMigrateLegacyCosmeticFiltersFunction
    : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.migrateLegacyCosmeticFilters",
                             UNKNOWN)

 protected:
  ~adrbrowsielShieldsMigrateLegacyCosmeticFiltersFunction() override {}

  ResponseAction Run() override;
};

class adrbrowsielShieldsAddSiteCosmeticFilterFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.addSiteCosmeticFilter", UNKNOWN)

 protected:
  ~adrbrowsielShieldsAddSiteCosmeticFilterFunction() override {}

  ResponseAction Run() override;
};

class adrbrowsielShieldsOpenFilterManagementPageFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.openFilterManagementPage", UNKNOWN)

 protected:
  ~adrbrowsielShieldsOpenFilterManagementPageFunction() override {}

  ResponseAction Run() override;
};

class adrbrowsielShieldsAllowScriptsOnceFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.allowScriptsOnce", UNKNOWN)

 protected:
  ~adrbrowsielShieldsAllowScriptsOnceFunction() override {}

  ResponseAction Run() override;
};

class adrbrowsielShieldsOpenBrowserActionUIFunction :
    public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.openBrowserActionUI", UNKNOWN)

 protected:
  ~adrbrowsielShieldsOpenBrowserActionUIFunction() override;

  ResponseAction Run() override;
};

class adrbrowsielShieldsSetadrbrowsielShieldsEnabledFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.setadrbrowsielShieldsEnabled", UNKNOWN)

 protected:
  ~adrbrowsielShieldsSetadrbrowsielShieldsEnabledFunction() override {}

  ResponseAction Run() override;
};

class adrbrowsielShieldsGetadrbrowsielShieldsEnabledFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.getadrbrowsielShieldsEnabled", UNKNOWN)

 protected:
  ~adrbrowsielShieldsGetadrbrowsielShieldsEnabledFunction() override {}

  ResponseAction Run() override;
};

class adrbrowsielShieldsShouldDoCosmeticFilteringFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.shouldDoCosmeticFiltering", UNKNOWN)

 protected:
  ~adrbrowsielShieldsShouldDoCosmeticFilteringFunction() override {}

  ResponseAction Run() override;
};

class adrbrowsielShieldsSetCosmeticFilteringControlTypeFunction :
    public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.setCosmeticFilteringControlType",
                             UNKNOWN)

 protected:
  ~adrbrowsielShieldsSetCosmeticFilteringControlTypeFunction() override {}

  ResponseAction Run() override;
};

class adrbrowsielShieldsIsFirstPartyCosmeticFilteringEnabledFunction :
    public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION(
      "adrbrowsielShields.isFirstPartyCosmeticFilteringEnabled",
      UNKNOWN)

 protected:
  ~adrbrowsielShieldsIsFirstPartyCosmeticFilteringEnabledFunction() override {}

  ResponseAction Run() override;
};

class adrbrowsielShieldsSetAdControlTypeFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.setAdControlType", UNKNOWN)

 protected:
  ~adrbrowsielShieldsSetAdControlTypeFunction() override {}

  ResponseAction Run() override;
};

class adrbrowsielShieldsGetAdControlTypeFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.getAdControlType", UNKNOWN)

 protected:
  ~adrbrowsielShieldsGetAdControlTypeFunction() override {}

  ResponseAction Run() override;
};

class adrbrowsielShieldsSetCookieControlTypeFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.setCookieControlType", UNKNOWN)

 protected:
  ~adrbrowsielShieldsSetCookieControlTypeFunction() override {}

  ResponseAction Run() override;
};

class adrbrowsielShieldsGetCookieControlTypeFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.getCookieControlType", UNKNOWN)

 protected:
  ~adrbrowsielShieldsGetCookieControlTypeFunction() override {}

  ResponseAction Run() override;
};

class adrbrowsielShieldsSetFingerprintingControlTypeFunction
    : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.setFingerprintingControlType",
                             UNKNOWN)

 protected:
  ~adrbrowsielShieldsSetFingerprintingControlTypeFunction() override {}

  ResponseAction Run() override;
};

class adrbrowsielShieldsGetFingerprintingControlTypeFunction
    : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.getFingerprintingControlType",
                             UNKNOWN)

 protected:
  ~adrbrowsielShieldsGetFingerprintingControlTypeFunction() override {}

  ResponseAction Run() override;
};

class adrbrowsielShieldsSetHTTPSEverywhereEnabledFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.setHTTPSEverywhereEnabled",
                             UNKNOWN)

 protected:
  ~adrbrowsielShieldsSetHTTPSEverywhereEnabledFunction() override {}

  ResponseAction Run() override;
};

class adrbrowsielShieldsGetHTTPSEverywhereEnabledFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.getHTTPSEverywhereEnabled",
                             UNKNOWN)

 protected:
  ~adrbrowsielShieldsGetHTTPSEverywhereEnabledFunction() override {}

  ResponseAction Run() override;
};

class adrbrowsielShieldsSetNoScriptControlTypeFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.setNoScriptControlType", UNKNOWN)

 protected:
  ~adrbrowsielShieldsSetNoScriptControlTypeFunction() override {}

  ResponseAction Run() override;
};

class adrbrowsielShieldsGetNoScriptControlTypeFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.getNoScriptControlType", UNKNOWN)

 protected:
  ~adrbrowsielShieldsGetNoScriptControlTypeFunction() override {}

  ResponseAction Run() override;
};

// Notifies the browser that the shields panel was shown to the user.
class adrbrowsielShieldsOnShieldsPanelShownFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.onShieldsPanelShown", UNKNOWN)

 protected:
  ~adrbrowsielShieldsOnShieldsPanelShownFunction() override {}

  // ExtensionFunction:
  ResponseAction Run() override;
};

class adrbrowsielShieldsReportBrokenSiteFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("adrbrowsielShields.reportBrokenSite", UNKNOWN)

 protected:
  ~adrbrowsielShieldsReportBrokenSiteFunction() override {}

  ResponseAction Run() override;
};

}  // namespace api
}  // namespace extensions

#endif  // adrbrowsiel_BROWSER_EXTENSIONS_API_adrbrowsiel_SHIELDS_API_H_
