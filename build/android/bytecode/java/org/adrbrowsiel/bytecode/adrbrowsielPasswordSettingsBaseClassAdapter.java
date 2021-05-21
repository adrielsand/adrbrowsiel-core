/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.adrbrowsiel.bytecode;

import org.objectweb.asm.ClassVisitor;

public class adrbrowsielPasswordSettingsBaseClassAdapter extends adrbrowsielClassVisitor {
    static String sPasswordSettingsClassName =
            "org/chromium/chrome/browser/password_manager/settings/PasswordSettings";
    static String sadrbrowsielPasswordSettingsBaseClassName =
            "org/chromium/chrome/browser/password_manager/settings/adrbrowsielPasswordSettingsBase";

    public adrbrowsielPasswordSettingsBaseClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sPasswordSettingsClassName, sadrbrowsielPasswordSettingsBaseClassName);

        changeMethodOwner(sPasswordSettingsClassName, "createCheckPasswords",
                sadrbrowsielPasswordSettingsBaseClassName);
        deleteMethod(sPasswordSettingsClassName, "createCheckPasswords");
    }
}
