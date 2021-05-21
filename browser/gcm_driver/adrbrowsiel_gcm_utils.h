/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_GCM_DRIVER_adrbrowsiel_GCM_UTILS_H_
#define adrbrowsiel_BROWSER_GCM_DRIVER_adrbrowsiel_GCM_UTILS_H_

class Profile;

namespace user_prefs {
class PrefRegistrySyncable;
}  // namespace user_prefs

namespace gcm {

void RegisterGCMProfilePrefs(user_prefs::PrefRegistrySyncable* registry);
void MigrateGCMPrefs(Profile* profile);

}  // namespace gcm

#endif  // adrbrowsiel_BROWSER_GCM_DRIVER_adrbrowsiel_GCM_UTILS_H_
