/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.search_engines.settings;

public class adrbrowsielPrivateSearchEnginePreference extends adrbrowsielSearchEnginePreference {
    public adrbrowsielPrivateSearchEnginePreference() {
        super(true /* private */);
    }
}
