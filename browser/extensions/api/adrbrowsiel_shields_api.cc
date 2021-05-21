/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/extensions/api/adrbrowsiel_shields_api.h"

#include <utility>

#include "base/feature_list.h"
#include "base/strings/string_number_conversions.h"
#include "adrbrowsiel/browser/adrbrowsiel_browser_process.h"
#include "adrbrowsiel/browser/adrbrowsiel_shields/adrbrowsiel_shields_web_contents_observer.h"
#include "adrbrowsiel/browser/extensions/api/adrbrowsiel_action_api.h"
#include "adrbrowsiel/browser/ui/adrbrowsiel_pages.h"
#include "adrbrowsiel/browser/webcompat_reporter/webcompat_reporter_dialog.h"
#include "adrbrowsiel/common/extensions/api/adrbrowsiel_shields.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/ad_block_custom_filters_service.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/ad_block_service.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/adrbrowsiel_shields_p3a.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/adrbrowsiel_shields_util.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/common/adrbrowsiel_shield_constants.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/common/features.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/extensions/api/tabs/tabs_constants.h"
#include "chrome/browser/extensions/chrome_extension_function_details.h"
#include "chrome/browser/extensions/extension_tab_util.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser_finder.h"
#include "content/public/browser/web_contents.h"
#include "extensions/browser/extension_util.h"
#include "extensions/common/constants.h"

using adrbrowsiel_shields::adrbrowsielShieldsWebContentsObserver;
using adrbrowsiel_shields::ControlType;
using adrbrowsiel_shields::ControlTypeFromString;
using adrbrowsiel_shields::ControlTypeToString;

namespace extensions {
namespace api {

namespace {

const char kInvalidUrlError[] = "Invalid URL.";
const char kInvalidControlTypeError[] = "Invalid ControlType.";

}  // namespace

ExtensionFunction::ResponseAction
adrbrowsielShieldsUrlCosmeticResourcesFunction::Run() {
  std::unique_ptr<adrbrowsiel_shields::UrlCosmeticResources::Params> params(
      adrbrowsiel_shields::UrlCosmeticResources::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());
  g_adrbrowsiel_browser_process->ad_block_service()
      ->GetTaskRunner()
      ->PostTaskAndReplyWithResult(
          FROM_HERE,
          base::BindOnce(&adrbrowsielShieldsUrlCosmeticResourcesFunction::
                             GetUrlCosmeticResourcesOnTaskRunner,
                         this, params->url),
          base::BindOnce(&adrbrowsielShieldsUrlCosmeticResourcesFunction::
                             GetUrlCosmeticResourcesOnUI,
                         this));
  return RespondLater();
}

std::unique_ptr<base::ListValue>
adrbrowsielShieldsUrlCosmeticResourcesFunction::GetUrlCosmeticResourcesOnTaskRunner(
    const std::string& url) {
  base::Optional<base::Value> resources =
      g_adrbrowsiel_browser_process->ad_block_service()->UrlCosmeticResources(url);

  if (!resources || !resources->is_dict()) {
    return std::unique_ptr<base::ListValue>();
  }

  auto result_list = std::make_unique<base::ListValue>();
  result_list->Append(std::move(*resources));
  return result_list;
}

void adrbrowsielShieldsUrlCosmeticResourcesFunction::GetUrlCosmeticResourcesOnUI(
    std::unique_ptr<base::ListValue> resources) {
  if (!resources) {
    Respond(Error("Url-specific cosmetic resources could not be returned"));
    return;
  }
  Respond(ArgumentList(std::move(resources)));
}

ExtensionFunction::ResponseAction
adrbrowsielShieldsHiddenClassIdSelectorsFunction::Run() {
  std::unique_ptr<adrbrowsiel_shields::HiddenClassIdSelectors::Params> params(
      adrbrowsiel_shields::HiddenClassIdSelectors::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());
  g_adrbrowsiel_browser_process->ad_block_service()
      ->GetTaskRunner()
      ->PostTaskAndReplyWithResult(
          FROM_HERE,
          base::BindOnce(&adrbrowsielShieldsHiddenClassIdSelectorsFunction::
                             GetHiddenClassIdSelectorsOnTaskRunner,
                         this, params->classes, params->ids,
                         params->exceptions),
          base::BindOnce(&adrbrowsielShieldsHiddenClassIdSelectorsFunction::
                             GetHiddenClassIdSelectorsOnUI,
                         this));
  return RespondLater();
}

std::unique_ptr<base::ListValue> adrbrowsielShieldsHiddenClassIdSelectorsFunction::
    GetHiddenClassIdSelectorsOnTaskRunner(
        const std::vector<std::string>& classes,
        const std::vector<std::string>& ids,
        const std::vector<std::string>& exceptions) {
  base::Optional<base::Value> hide_selectors =
      g_adrbrowsiel_browser_process->ad_block_service()->HiddenClassIdSelectors(
          classes, ids, exceptions);

  if (!hide_selectors || !hide_selectors->is_list())
    return std::make_unique<base::ListValue>();

  auto result_list =
      std::make_unique<base::ListValue>(hide_selectors->GetList());

  return result_list;
}

void adrbrowsielShieldsHiddenClassIdSelectorsFunction::GetHiddenClassIdSelectorsOnUI(
    std::unique_ptr<base::ListValue> selectors) {
  Respond(ArgumentList(std::move(selectors)));
}

ExtensionFunction::ResponseAction
adrbrowsielShieldsMigrateLegacyCosmeticFiltersFunction::Run() {
  std::unique_ptr<adrbrowsiel_shields::MigrateLegacyCosmeticFilters::Params> params(
      adrbrowsiel_shields::MigrateLegacyCosmeticFilters::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const bool success =
      g_adrbrowsiel_browser_process->ad_block_custom_filters_service()
          ->MigrateLegacyCosmeticFilters(
              params->legacy_filters.additional_properties);

  auto callback_args = std::make_unique<base::ListValue>();
  callback_args->Append(base::Value(success));
  return RespondNow(ArgumentList(std::move(callback_args)));
}

ExtensionFunction::ResponseAction
adrbrowsielShieldsAddSiteCosmeticFilterFunction::Run() {
  std::unique_ptr<adrbrowsiel_shields::AddSiteCosmeticFilter::Params> params(
      adrbrowsiel_shields::AddSiteCosmeticFilter::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  auto* custom_filters_service =
      g_adrbrowsiel_browser_process->ad_block_custom_filters_service();
  std::string custom_filters = custom_filters_service->GetCustomFilters();
  custom_filters_service->UpdateCustomFilters(custom_filters + '\n' +
                                              params->host + "##" +
                                              params->css_selector + '\n');

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
adrbrowsielShieldsOpenFilterManagementPageFunction::Run() {
  Browser* browser = chrome::FindLastActive();
  if (browser) {
    adrbrowsiel::ShowadrbrowsielAdblock(browser);
  }

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction adrbrowsielShieldsAllowScriptsOnceFunction::Run() {
  std::unique_ptr<adrbrowsiel_shields::AllowScriptsOnce::Params> params(
      adrbrowsiel_shields::AllowScriptsOnce::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  // Get web contents for this tab
  content::WebContents* contents = nullptr;
  if (!ExtensionTabUtil::GetTabById(
          params->tab_id, Profile::FromBrowserContext(browser_context()),
          include_incognito_information(), nullptr, nullptr, &contents,
          nullptr)) {
    return RespondNow(Error(tabs_constants::kTabNotFoundError,
                            base::NumberToString(params->tab_id)));
  }

  adrbrowsielShieldsWebContentsObserver::FromWebContents(contents)->AllowScriptsOnce(
      params->origins, contents);
  return RespondNow(NoArguments());
}

adrbrowsielShieldsOpenBrowserActionUIFunction::
~adrbrowsielShieldsOpenBrowserActionUIFunction() {
}

ExtensionFunction::ResponseAction
adrbrowsielShieldsOpenBrowserActionUIFunction::Run() {
  std::unique_ptr<adrbrowsiel_shields::OpenBrowserActionUI::Params> params(
      adrbrowsiel_shields::OpenBrowserActionUI::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());
  std::string error;
  if (!adrbrowsielActionAPI::ShowActionUI(this,
      adrbrowsiel_extension_id,
      std::move(params->window_id),
      std::move(params->relative_path), &error)) {
    return RespondNow(Error(error));
  }
  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
adrbrowsielShieldsSetadrbrowsielShieldsEnabledFunction::Run() {
  std::unique_ptr<adrbrowsiel_shields::SetadrbrowsielShieldsEnabled::Params> params(
      adrbrowsiel_shields::SetadrbrowsielShieldsEnabled::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow setting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  ::adrbrowsiel_shields::SetadrbrowsielShieldsEnabled(
      HostContentSettingsMapFactory::GetForProfile(profile),
      params->enabled,
      url,
      g_browser_process->local_state());

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
adrbrowsielShieldsGetadrbrowsielShieldsEnabledFunction::Run() {
  std::unique_ptr<adrbrowsiel_shields::GetadrbrowsielShieldsEnabled::Params> params(
      adrbrowsiel_shields::GetadrbrowsielShieldsEnabled::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow getting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto enabled = ::adrbrowsiel_shields::GetadrbrowsielShieldsEnabled(
      HostContentSettingsMapFactory::GetForProfile(profile),
      url);

  return RespondNow(OneArgument(base::Value(enabled)));
}

ExtensionFunction::ResponseAction
adrbrowsielShieldsShouldDoCosmeticFilteringFunction::Run() {
#if !defined(OS_ANDROID) && !defined(CHROME_OS)
  if (base::FeatureList::IsEnabled(
          ::adrbrowsiel_shields::features::kadrbrowsielAdblockCosmeticFilteringNative))
    return RespondNow(OneArgument(base::Value(false)));
#endif
  std::unique_ptr<adrbrowsiel_shields::ShouldDoCosmeticFiltering::Params>
    params(
      adrbrowsiel_shields::ShouldDoCosmeticFiltering::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow getting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  const bool enabled = ::adrbrowsiel_shields::ShouldDoCosmeticFiltering(
      HostContentSettingsMapFactory::GetForProfile(profile),
      url);

  return RespondNow(OneArgument(base::Value(enabled)));
}

ExtensionFunction::ResponseAction
adrbrowsielShieldsSetCosmeticFilteringControlTypeFunction::Run() {
  std::unique_ptr<adrbrowsiel_shields::SetCosmeticFilteringControlType::Params>
    params(
      adrbrowsiel_shields::SetCosmeticFilteringControlType::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow setting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  auto control_type = ControlTypeFromString(params->control_type);
  if (control_type == ControlType::INVALID) {
    return RespondNow(Error(kInvalidControlTypeError, params->control_type));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  ::adrbrowsiel_shields::SetCosmeticFilteringControlType(
      HostContentSettingsMapFactory::GetForProfile(profile),
      control_type,
      url,
      g_browser_process->local_state());

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
adrbrowsielShieldsIsFirstPartyCosmeticFilteringEnabledFunction::Run() {
  std::unique_ptr<adrbrowsiel_shields::IsFirstPartyCosmeticFilteringEnabled::Params>
      params(
          adrbrowsiel_shields::IsFirstPartyCosmeticFilteringEnabled::Params::Create(
          *args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow getting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  const bool enabled = ::adrbrowsiel_shields::IsFirstPartyCosmeticFilteringEnabled(
      HostContentSettingsMapFactory::GetForProfile(profile), url);

  return RespondNow(OneArgument(base::Value(enabled)));
}

ExtensionFunction::ResponseAction adrbrowsielShieldsSetAdControlTypeFunction::Run() {
  std::unique_ptr<adrbrowsiel_shields::SetAdControlType::Params> params(
      adrbrowsiel_shields::SetAdControlType::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow setting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  auto control_type = ControlTypeFromString(params->control_type);
  if (control_type == ControlType::INVALID) {
    return RespondNow(Error(kInvalidControlTypeError, params->control_type));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  ::adrbrowsiel_shields::SetAdControlType(
      HostContentSettingsMapFactory::GetForProfile(profile),
      control_type,
      url,
      g_browser_process->local_state());

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction adrbrowsielShieldsGetAdControlTypeFunction::Run() {
  std::unique_ptr<adrbrowsiel_shields::GetAdControlType::Params> params(
      adrbrowsiel_shields::GetAdControlType::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow getting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto type = ::adrbrowsiel_shields::GetAdControlType(
      HostContentSettingsMapFactory::GetForProfile(profile),
      url);

  return RespondNow(OneArgument(base::Value(ControlTypeToString(type))));
}

ExtensionFunction::ResponseAction
adrbrowsielShieldsSetCookieControlTypeFunction::Run() {
  std::unique_ptr<adrbrowsiel_shields::SetCookieControlType::Params> params(
      adrbrowsiel_shields::SetCookieControlType::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow setting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  auto control_type = ControlTypeFromString(params->control_type);
  if (control_type == ControlType::INVALID) {
    return RespondNow(Error(kInvalidControlTypeError, params->control_type));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  ::adrbrowsiel_shields::SetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(profile),
      control_type,
      url,
      g_browser_process->local_state());

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
adrbrowsielShieldsGetCookieControlTypeFunction::Run() {
  std::unique_ptr<adrbrowsiel_shields::GetCookieControlType::Params> params(
      adrbrowsiel_shields::GetCookieControlType::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow getting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto type = ::adrbrowsiel_shields::GetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(profile),
      url);

  return RespondNow(OneArgument(base::Value(ControlTypeToString(type))));
}

ExtensionFunction::ResponseAction
adrbrowsielShieldsSetFingerprintingControlTypeFunction::Run() {
  std::unique_ptr<adrbrowsiel_shields::SetFingerprintingControlType::Params> params(
      adrbrowsiel_shields::SetFingerprintingControlType::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow setting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  auto control_type = ControlTypeFromString(params->control_type);
  if (control_type == ControlType::INVALID) {
    return RespondNow(Error(kInvalidControlTypeError, params->control_type));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  ::adrbrowsiel_shields::SetFingerprintingControlType(
      HostContentSettingsMapFactory::GetForProfile(profile),
      control_type,
      url,
      g_browser_process->local_state());

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
adrbrowsielShieldsGetFingerprintingControlTypeFunction::Run() {
  std::unique_ptr<adrbrowsiel_shields::GetFingerprintingControlType::Params> params(
      adrbrowsiel_shields::GetFingerprintingControlType::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow getting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto type = ::adrbrowsiel_shields::GetFingerprintingControlType(
      HostContentSettingsMapFactory::GetForProfile(profile),
      url);

  return RespondNow(OneArgument(base::Value(ControlTypeToString(type))));
}

ExtensionFunction::ResponseAction
adrbrowsielShieldsSetHTTPSEverywhereEnabledFunction::Run() {
  std::unique_ptr<adrbrowsiel_shields::SetHTTPSEverywhereEnabled::Params> params(
      adrbrowsiel_shields::SetHTTPSEverywhereEnabled::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow setting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  ::adrbrowsiel_shields::SetHTTPSEverywhereEnabled(
      HostContentSettingsMapFactory::GetForProfile(profile),
      params->enabled,
      url,
      g_browser_process->local_state());

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
adrbrowsielShieldsGetHTTPSEverywhereEnabledFunction::Run() {
  std::unique_ptr<adrbrowsiel_shields::GetHTTPSEverywhereEnabled::Params> params(
      adrbrowsiel_shields::GetHTTPSEverywhereEnabled::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow getting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto type = ::adrbrowsiel_shields::GetHTTPSEverywhereEnabled(
      HostContentSettingsMapFactory::GetForProfile(profile),
      url);

  return RespondNow(OneArgument(base::Value(type)));
}

ExtensionFunction::ResponseAction
adrbrowsielShieldsSetNoScriptControlTypeFunction::Run() {
  std::unique_ptr<adrbrowsiel_shields::SetNoScriptControlType::Params> params(
      adrbrowsiel_shields::SetNoScriptControlType::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow setting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  auto control_type = ControlTypeFromString(params->control_type);
  if (control_type == ControlType::INVALID) {
    return RespondNow(Error(kInvalidControlTypeError, params->control_type));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  ::adrbrowsiel_shields::SetNoScriptControlType(
      HostContentSettingsMapFactory::GetForProfile(profile),
      control_type,
      url,
      g_browser_process->local_state());

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
adrbrowsielShieldsGetNoScriptControlTypeFunction::Run() {
  std::unique_ptr<adrbrowsiel_shields::GetNoScriptControlType::Params> params(
      adrbrowsiel_shields::GetNoScriptControlType::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  const GURL url(params->url);
  // we don't allow getting defaults from the extension
  if (url.is_empty() || !url.is_valid()) {
    return RespondNow(Error(kInvalidUrlError, params->url));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto type = ::adrbrowsiel_shields::GetNoScriptControlType(
      HostContentSettingsMapFactory::GetForProfile(profile),
      url);

  return RespondNow(OneArgument(base::Value(ControlTypeToString(type))));
}

ExtensionFunction::ResponseAction
adrbrowsielShieldsOnShieldsPanelShownFunction::Run() {
  ::adrbrowsiel_shields::MaybeRecordShieldsUsageP3A(::adrbrowsiel_shields::kClicked,
                                              g_browser_process->local_state());
  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction adrbrowsielShieldsReportBrokenSiteFunction::Run() {
  std::unique_ptr<adrbrowsiel_shields::ReportBrokenSite::Params> params(
      adrbrowsiel_shields::ReportBrokenSite::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  // Get web contents for this tab
  content::WebContents* contents = nullptr;
  if (!ExtensionTabUtil::GetTabById(
        params->tab_id,
        Profile::FromBrowserContext(browser_context()),
        false,
        nullptr,
        nullptr,
        &contents,
        nullptr)) {
    return RespondNow(Error(tabs_constants::kTabNotFoundError,
                            base::NumberToString(params->tab_id)));
  }

  OpenWebcompatReporterDialog(contents);

  return RespondNow(NoArguments());
}

}  // namespace api
}  // namespace extensions
