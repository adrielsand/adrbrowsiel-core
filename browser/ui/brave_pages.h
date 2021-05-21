/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_adrbrowsiel_PAGES_H_
#define adrbrowsiel_BROWSER_UI_adrbrowsiel_PAGES_H_

#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"

class Browser;

namespace adrbrowsiel {

void ShowadrbrowsielAdblock(Browser* browser);
void ShowWebcompatReporter(Browser* browser);
void ShowadrbrowsielRewards(Browser* browser);
void ShowadrbrowsielWallet(Browser* browser);
void ShowExtensionSettings(Browser* browser);
void ShowWalletSettings(Browser* browser);
void ShowSync(Browser* browser);

}  // namespace adrbrowsiel

#endif  // adrbrowsiel_BROWSER_UI_adrbrowsiel_PAGES_H_
