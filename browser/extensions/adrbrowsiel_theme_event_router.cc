/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/extensions/adrbrowsiel_theme_event_router.h"

#include <memory>
#include <string>
#include <utility>

#include "adrbrowsiel/browser/themes/adrbrowsiel_dark_mode_utils.h"
#include "adrbrowsiel/common/extensions/api/adrbrowsiel_theme.h"
#include "chrome/browser/profiles/profile.h"
#include "extensions/browser/event_router.h"
#include "ui/native_theme/native_theme.h"

namespace extensions {

adrbrowsielThemeEventRouter::adrbrowsielThemeEventRouter(Profile* profile)
    : profile_(profile),
      observer_(this) {
  observer_.Add(ui::NativeTheme::GetInstanceForNativeUi());
}

adrbrowsielThemeEventRouter::~adrbrowsielThemeEventRouter() {}

void adrbrowsielThemeEventRouter::OnNativeThemeUpdated(
    ui::NativeTheme* observed_theme) {
  DCHECK(observer_.IsObserving(observed_theme));
  Notify();
}

void adrbrowsielThemeEventRouter::Notify() {
  const std::string theme_type =
      dark_mode::GetStringFromadrbrowsielDarkModeType(
          dark_mode::GetActiveadrbrowsielDarkModeType());

  auto event = std::make_unique<extensions::Event>(
      extensions::events::adrbrowsiel_ON_adrbrowsiel_THEME_TYPE_CHANGED,
      api::adrbrowsiel_theme::OnadrbrowsielThemeTypeChanged::kEventName,
      api::adrbrowsiel_theme::OnadrbrowsielThemeTypeChanged::Create(theme_type),
      profile_);

  if (EventRouter* event_router = EventRouter::Get(profile_))
    event_router->BroadcastEvent(std::move(event));
}

}  // namespace extensions
