/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.preferences.website;

/**
 * Allows monitoring of blocked resources via adrbrowsiel shields.
 */
public interface adrbrowsielShieldsContentSettingsObserver {
    public void blockEvent(int tabId, String block_type, String subresource);
    public void savedBandwidth(long savings);
}

