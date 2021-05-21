/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_STARTUP_DEFAULT_adrbrowsiel_BROWSER_PROMPT_H_
#define adrbrowsiel_BROWSER_UI_STARTUP_DEFAULT_adrbrowsiel_BROWSER_PROMPT_H_

class PrefRegistrySimple;
class Profile;

void RegisterDefaultadrbrowsielBrowserPromptPrefs(PrefRegistrySimple* registry);

void ShowDefaultadrbrowsielBrowserPrompt(Profile* profile);

void ResetDefaultadrbrowsielBrowserPrompt(Profile* profile);

#endif  // adrbrowsiel_BROWSER_UI_STARTUP_DEFAULT_adrbrowsiel_BROWSER_PROMPT_H_
