/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/webui/settings/default_adrbrowsiel_shields_handler.h"

#include <string>

#include "base/bind.h"
#include "base/values.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/adrbrowsiel_shields_util.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/web_ui.h"
#include "url/gurl.h"

using adrbrowsiel_shields::ControlType;
using adrbrowsiel_shields::ControlTypeFromString;
using adrbrowsiel_shields::ControlTypeToString;

void DefaultadrbrowsielShieldsHandler::RegisterMessages() {
  profile_ = Profile::FromWebUI(web_ui());
  web_ui()->RegisterMessageCallback(
      "isAdControlEnabled",
      base::BindRepeating(&DefaultadrbrowsielShieldsHandler::IsAdControlEnabled,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setAdControlType",
      base::BindRepeating(&DefaultadrbrowsielShieldsHandler::SetAdControlType,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "isFirstPartyCosmeticFilteringEnabled",
      base::BindRepeating(
          &DefaultadrbrowsielShieldsHandler::IsFirstPartyCosmeticFilteringEnabled,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setCosmeticFilteringControlType",
      base::BindRepeating(
          &DefaultadrbrowsielShieldsHandler::SetCosmeticFilteringControlType,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getCookieControlType",
      base::BindRepeating(&DefaultadrbrowsielShieldsHandler::GetCookieControlType,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setCookieControlType",
      base::BindRepeating(&DefaultadrbrowsielShieldsHandler::SetCookieControlType,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getFingerprintingControlType",
      base::BindRepeating(
          &DefaultadrbrowsielShieldsHandler::GetFingerprintingControlType,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setFingerprintingControlType",
      base::BindRepeating(
          &DefaultadrbrowsielShieldsHandler::SetFingerprintingControlType,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setHTTPSEverywhereEnabled",
      base::BindRepeating(
          &DefaultadrbrowsielShieldsHandler::SetHTTPSEverywhereEnabled,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setNoScriptControlType",
      base::BindRepeating(&DefaultadrbrowsielShieldsHandler::SetNoScriptControlType,
                          base::Unretained(this)));
}

void DefaultadrbrowsielShieldsHandler::IsAdControlEnabled(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);

  ControlType setting = adrbrowsiel_shields::GetAdControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_), GURL());

  AllowJavascript();
  ResolveJavascriptCallback(
      args->GetList()[0].Clone(),
      base::Value(setting == ControlType::BLOCK));
}

void DefaultadrbrowsielShieldsHandler::SetAdControlType(const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);
  bool value;
  args->GetBoolean(0, &value);

  adrbrowsiel_shields::SetAdControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      value ? ControlType::BLOCK
            : ControlType::ALLOW,
      GURL(),
      g_browser_process->local_state());
}

void DefaultadrbrowsielShieldsHandler::IsFirstPartyCosmeticFilteringEnabled(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);

  bool enabled = adrbrowsiel_shields::IsFirstPartyCosmeticFilteringEnabled(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      GURL());

  AllowJavascript();
  ResolveJavascriptCallback(
      args->GetList()[0].Clone(),
      base::Value(enabled));
}

void DefaultadrbrowsielShieldsHandler::SetCosmeticFilteringControlType(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);
  std::string value;
  args->GetString(0, &value);

  adrbrowsiel_shields::SetCosmeticFilteringControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      ControlTypeFromString(value),
      GURL(),
      g_browser_process->local_state());
}

void DefaultadrbrowsielShieldsHandler::GetCookieControlType(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);

  ControlType setting = adrbrowsiel_shields::GetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      GURL());

  AllowJavascript();
  ResolveJavascriptCallback(
      args->GetList()[0].Clone(),
      base::Value(ControlTypeToString(setting)));
}

void DefaultadrbrowsielShieldsHandler::SetCookieControlType(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);
  std::string value;
  args->GetString(0, &value);

  adrbrowsiel_shields::SetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      ControlTypeFromString(value),
      GURL(),
      g_browser_process->local_state());
}

void DefaultadrbrowsielShieldsHandler::GetFingerprintingControlType(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);

  ControlType setting = adrbrowsiel_shields::GetFingerprintingControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      GURL());

  AllowJavascript();
  ResolveJavascriptCallback(
      args->GetList()[0].Clone(),
      base::Value(ControlTypeToString(setting)));
}

void DefaultadrbrowsielShieldsHandler::SetFingerprintingControlType(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);
  std::string value;
  args->GetString(0, &value);

  adrbrowsiel_shields::SetFingerprintingControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      ControlTypeFromString(value),
      GURL(),
      g_browser_process->local_state());
}

void DefaultadrbrowsielShieldsHandler::SetHTTPSEverywhereEnabled(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);
  bool value;
  args->GetBoolean(0, &value);

  adrbrowsiel_shields::SetHTTPSEverywhereEnabled(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      value,
      GURL(),
      g_browser_process->local_state());
}

void DefaultadrbrowsielShieldsHandler::SetNoScriptControlType(
    const base::ListValue* args) {
  CHECK_EQ(args->GetSize(), 1U);
  CHECK(profile_);
  bool value;
  args->GetBoolean(0, &value);

  adrbrowsiel_shields::SetNoScriptControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      value ? ControlType::BLOCK
            : ControlType::ALLOW,
      GURL(),
      g_browser_process->local_state());
}
