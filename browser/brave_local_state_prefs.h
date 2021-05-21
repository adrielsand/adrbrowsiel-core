/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_adrbrowsiel_LOCAL_STATE_PREFS_H_
#define adrbrowsiel_BROWSER_adrbrowsiel_LOCAL_STATE_PREFS_H_

class PrefRegistrySimple;

namespace adrbrowsiel {

void RegisterLocalStatePrefs(PrefRegistrySimple* registry);

}  // namespace adrbrowsiel

#endif  // adrbrowsiel_BROWSER_adrbrowsiel_LOCAL_STATE_PREFS_H_
