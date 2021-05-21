/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.adrbrowsiel.bytecode;

import org.objectweb.asm.ClassVisitor;

public class adrbrowsielAndroidSyncSettingsClassAdapter extends adrbrowsielClassVisitor {
	static String sAndroidSyncSettingsClassName =
            "org/chromium/chrome/browser/sync/AndroidSyncSettings";
    static String sadrbrowsielAndroidSyncSettingsClassName =
            "org/chromium/chrome/browser/sync/adrbrowsielAndroidSyncSettings";

    public adrbrowsielAndroidSyncSettingsClassAdapter(ClassVisitor visitor) {
        super(visitor);

        addMethodAnnotation(sadrbrowsielAndroidSyncSettingsClassName,
            "disableChromeSync", "Ljava/lang/Override;");
    }
}
