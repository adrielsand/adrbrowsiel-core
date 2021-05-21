/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.adrbrowsiel.bytecode;

import org.objectweb.asm.ClassVisitor;

public class adrbrowsielSettingsLauncherImplClassAdapter extends adrbrowsielClassVisitor {
    static String sSettingsLauncherImplClassName =
            "org/chromium/chrome/browser/settings/SettingsLauncherImpl";
    static String sadrbrowsielSettingsLauncherImplClassName =
            "org/chromium/chrome/browser/settings/adrbrowsielSettingsLauncherImpl";

    public adrbrowsielSettingsLauncherImplClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sSettingsLauncherImplClassName, sadrbrowsielSettingsLauncherImplClassName);
    }
}
