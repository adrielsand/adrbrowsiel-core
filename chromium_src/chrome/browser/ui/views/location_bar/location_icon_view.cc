/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/ipfs/ipfs_constants.h"

#define adrbrowsiel_SHOULD_SHOW_URL_IPFS_CHECK \
  url.SchemeIs(ipfs::kIPFSScheme) || url.SchemeIs(ipfs::kIPNSScheme) ||

#include "../../../../../../../chrome/browser/ui/views/location_bar/location_icon_view.cc"
#undef adrbrowsiel_SHOULD_SHOW_URL_IPFS_CHECK
