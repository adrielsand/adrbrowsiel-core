/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/translate/buildflags/buildflags.h"
#include "adrbrowsiel/browser/ui/views/location_bar/adrbrowsiel_star_view.h"
#include "adrbrowsiel/browser/ui/views/translate/adrbrowsiel_translate_icon_view.h"
#include "adrbrowsiel/browser/ui/views/reader_mode/adrbrowsiel_reader_mode_icon_view.h"
#include "chrome/browser/ui/page_action/page_action_icon_type.h"

namespace {
constexpr PageActionIconType kUndefinedPageActionIconType =
    static_cast<PageActionIconType>(-1);
}  // namespace

// Circumvent creation of CookieControlsIconView in
// PageActionIconController::Init's switch statement by injecting a case
// with a non-existent value created above.
#define kCookieControls \
  kCookieControls:      \
  break;                \
  case kUndefinedPageActionIconType

#if BUILDFLAG(ENABLE_adrbrowsiel_TRANSLATE_EXTENSION)
#define TranslateIconView adrbrowsielTranslateIconView
#endif
#define ReaderModeIconView adrbrowsielReaderModeIconView
#define StarView adrbrowsielStarView
#include "../../../../../../../chrome/browser/ui/views/page_action/page_action_icon_controller.cc"
#undef StarView
#if BUILDFLAG(ENABLE_adrbrowsiel_TRANSLATE_EXTENSION)
#undef TranslateIconView
#endif
#undef ReaderModeIconView
#undef kCookieControls
