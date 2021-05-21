/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_BROWSER_COMMANDS_H_
#define adrbrowsiel_BROWSER_UI_BROWSER_COMMANDS_H_

class Browser;

namespace adrbrowsiel {

void NewOffTheRecordWindowTor(Browser*);
void NewTorConnectionForSite(Browser*);
void AddNewProfile();
void OpenGuestProfile();
void ToggleSpeedreader(Browser* browser);
void ShowWalletBubble(Browser* browser);
void CloseWalletBubble(Browser* browser);

}  // namespace adrbrowsiel


#endif  // adrbrowsiel_BROWSER_UI_BROWSER_COMMANDS_H_
