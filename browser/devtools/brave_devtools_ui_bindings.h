/* Copyright (c) 2018 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_DEVTOOLS_adrbrowsiel_DEVTOOLS_UI_BINDINGS_H_
#define adrbrowsiel_BROWSER_DEVTOOLS_adrbrowsiel_DEVTOOLS_UI_BINDINGS_H_

#include "chrome/browser/devtools/devtools_ui_bindings.h"

class adrbrowsielDevToolsUIBindings : public DevToolsUIBindings {
 public:
  using DevToolsUIBindings::DevToolsUIBindings;

 private:
  FRIEND_TEST_ALL_PREFIXES(adrbrowsielDevToolsUIBindingsBrowserTest, ThemeTest);

  // DevToolsUIBindings overrides:
  void GetPreferences(DispatchCallback callback) override;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielDevToolsUIBindings);
};

#endif  // adrbrowsiel_BROWSER_DEVTOOLS_adrbrowsiel_DEVTOOLS_UI_BINDINGS_H_
