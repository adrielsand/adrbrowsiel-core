/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/views/profiles/adrbrowsiel_incognito_menu_view.h"
#include "adrbrowsiel/browser/ui/views/profiles/adrbrowsiel_profile_menu_view.h"

#include "chrome/browser/ui/views/profiles/incognito_menu_view.h"
#include "chrome/browser/ui/views/profiles/profile_menu_view.h"

#define IncognitoMenuView adrbrowsielIncognitoMenuView
#define ProfileMenuView adrbrowsielProfileMenuView
#include "../../../../../../../chrome/browser/ui/views/profiles/profile_menu_view_base.cc"  // NOLINT
#undef IncognitoMenuView
#undef ProfileMenuView
