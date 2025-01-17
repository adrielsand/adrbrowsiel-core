/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/profiles/profile_util.h"

#define GetBrowserContextRedirectedInIncognito \
  GetBrowserContextRedirectedInIncognitoOverride
#include "../../../../../chrome/browser/bookmarks/managed_bookmark_service_factory.cc"
