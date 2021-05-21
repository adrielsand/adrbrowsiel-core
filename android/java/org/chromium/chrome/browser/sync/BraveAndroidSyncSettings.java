/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.sync;

import android.accounts.Account;

import org.chromium.chrome.browser.sync.ProfileSyncService;

// see org.adrbrowsiel.bytecode.adrbrowsielAndroidSyncSettingsAdapter
public class adrbrowsielAndroidSyncSettings extends AndroidSyncSettings {
    private boolean mMasterSyncEnabled;

    public adrbrowsielAndroidSyncSettings(Account account) {
        super(account);
    }

    @Override
    public void disableChromeSync() { }

    // We need to override this to make able
    // DevicePickerBottomSheetContent.createContentView send the link
    // For adrbrowsiel we don't have an account in Android system account,
    // so pretend sync for adrbrowsiel "account" is always on when sync is configured
    @Override
    public boolean isChromeSyncEnabled() {
        ProfileSyncService profileSyncService = ProfileSyncService.get();
        return profileSyncService != null && profileSyncService.isFirstSetupComplete();
    }
}
