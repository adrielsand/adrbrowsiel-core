/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.adrbrowsiel.bytecode;

import org.objectweb.asm.ClassVisitor;

public class adrbrowsielIncognitoToggleTabLayoutClassAdapter extends adrbrowsielClassVisitor {
    static String sIncognitoToggleTabLayoutClassName =
            "org/chromium/chrome/browser/toolbar/IncognitoToggleTabLayout";
    static String sadrbrowsielIncognitoToggleTabLayoutClassName =
            "org/chromium/chrome/browser/toolbar/adrbrowsielIncognitoToggleTabLayout";

    public adrbrowsielIncognitoToggleTabLayoutClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sadrbrowsielIncognitoToggleTabLayoutClassName, "mIncognitoButtonIcon");
        makeProtectedField(sIncognitoToggleTabLayoutClassName, "mIncognitoButtonIcon");
    }
}
