/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/themes/adrbrowsiel_theme_service.h"

#include "base/no_destructor.h"
#include "adrbrowsiel/browser/themes/adrbrowsiel_theme_helper.h"
#include "adrbrowsiel/browser/extensions/adrbrowsiel_theme_event_router.h"
#include "chrome/browser/profiles/profile.h"
#include "adrbrowsiel/browser/profiles/profile_util.h"

#if defined(OS_WIN)
#include "adrbrowsiel/browser/themes/adrbrowsiel_theme_helper_win.h"
#endif

namespace {

const ThemeHelper& GetadrbrowsielThemeHelper(Profile* profile) {
#if defined(OS_WIN)
  using adrbrowsielThemeHelper = adrbrowsielThemeHelperWin;
#endif
  // Because the helper is created as a NoDestructor static, we need separate
  // instances for regular, tor and guest profiles.
  if (profile->IsTor()) {
    static base::NoDestructor<std::unique_ptr<ThemeHelper>> dark_theme_helper(
        std::make_unique<adrbrowsielThemeHelper>());
    (static_cast<adrbrowsielThemeHelper*>(dark_theme_helper.get()->get()))
        ->set_is_tor();
    return **dark_theme_helper;
  } else if (adrbrowsiel::IsGuestProfile(profile)) {
    static base::NoDestructor<std::unique_ptr<ThemeHelper>> dark_theme_helper(
        std::make_unique<adrbrowsielThemeHelper>());
    (static_cast<adrbrowsielThemeHelper*>(dark_theme_helper.get()->get()))
        ->set_is_guest();
    return **dark_theme_helper;
  } else {
    static base::NoDestructor<std::unique_ptr<ThemeHelper>> theme_helper(
        std::make_unique<adrbrowsielThemeHelper>());
    return **theme_helper;
  }
}

}  // namespace

// Replace Chromium's ThemeHelper with adrbrowsielThemeHelper that is appropriate for
// the given profile. There should only be 3 static ThemeHelpers at most: the
// original Chromium one, and 2 adrbrowsiel ones.
adrbrowsielThemeService::adrbrowsielThemeService(Profile* profile,
                                     const ThemeHelper& theme_helper)
    : ThemeService(profile, GetadrbrowsielThemeHelper(profile)) {
  adrbrowsiel_theme_event_router_.reset(
      new extensions::adrbrowsielThemeEventRouter(profile));
}

adrbrowsielThemeService::~adrbrowsielThemeService() = default;

void adrbrowsielThemeService::SetadrbrowsielThemeEventRouterForTesting(
    extensions::adrbrowsielThemeEventRouter* mock_router) {
  adrbrowsiel_theme_event_router_.reset(mock_router);
}
