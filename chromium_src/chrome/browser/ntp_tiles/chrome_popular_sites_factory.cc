/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ntp_tiles/chrome_popular_sites_factory.h"

#include "adrbrowsiel/components/ntp_tiles/adrbrowsiel_popular_sites_impl.h"

#define PopularSitesImpl adrbrowsielPopularSitesImpl
#include "../../../../../chrome/browser/ntp_tiles/chrome_popular_sites_factory.cc"  // NOLINT
#undef PopularSitesImpl
