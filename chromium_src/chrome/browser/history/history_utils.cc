/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define CanAddURLToHistory CanAddURLToHistory_ChromiumImpl
#include "../../../../../chrome/browser/history/history_utils.cc"  // NOLINT
#undef CanAddURLToHistory

#include "adrbrowsiel/common/url_constants.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/buildflags/buildflags.h"
#include "extensions/buildflags/buildflags.h"

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/adrbrowsiel_wallet_constants.h"
#endif

bool CanAddURLToHistory(const GURL& url) {
  if (!CanAddURLToHistory_ChromiumImpl(url))
    return false;

  bool is_adrbrowsiel_scheme = url.SchemeIs(content::kadrbrowsielUIScheme);
#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
  bool is_wallet_host =
      url.SchemeIs(kChromeExtensionScheme) &&
      url.host() == ethereum_remote_client_extension_id;
  return !is_adrbrowsiel_scheme && !is_wallet_host;
#else
  return !is_adrbrowsiel_scheme;
#endif
}
