/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_WAYBACK_MACHINE_adrbrowsiel_WAYBACK_MACHINE_UTILS_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_WAYBACK_MACHINE_adrbrowsiel_WAYBACK_MACHINE_UTILS_H_

class GURL;

bool IsWaybackMachineDisabledFor(const GURL& url);
GURL FixupWaybackQueryURL(const GURL& url);

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_WAYBACK_MACHINE_adrbrowsiel_WAYBACK_MACHINE_UTILS_H_
