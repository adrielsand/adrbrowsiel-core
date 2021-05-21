/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.adrbrowsiel.bytecode;

import org.objectweb.asm.ClassVisitor;

public class adrbrowsielManageSyncSettingsClassAdapter extends adrbrowsielClassVisitor {
    static String sManageSyncSettingsClassName =
            "org/chromium/chrome/browser/sync/settings/ManageSyncSettings";
    static String sadrbrowsielManageSyncSettingsClassName =
            "org/chromium/chrome/browser/sync/settings/adrbrowsielManageSyncSettings";

    adrbrowsielManageSyncSettingsClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sadrbrowsielManageSyncSettingsClassName, "mGoogleActivityControls");
        makeProtectedField(sManageSyncSettingsClassName, "mGoogleActivityControls");

        deleteField(sadrbrowsielManageSyncSettingsClassName, "mSyncEncryption");
        makeProtectedField(sManageSyncSettingsClassName, "mSyncEncryption");

        deleteField(sadrbrowsielManageSyncSettingsClassName, "mManageSyncData");
        makeProtectedField(sManageSyncSettingsClassName, "mManageSyncData");

        deleteField(sadrbrowsielManageSyncSettingsClassName, "mSyncPaymentsIntegration");
        makeProtectedField(sManageSyncSettingsClassName, "mSyncPaymentsIntegration");
    }
}
