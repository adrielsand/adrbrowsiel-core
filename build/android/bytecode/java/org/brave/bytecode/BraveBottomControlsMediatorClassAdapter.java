/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.adrbrowsiel.bytecode;

import org.objectweb.asm.ClassVisitor;

public class adrbrowsielBottomControlsMediatorClassAdapter extends adrbrowsielClassVisitor {
    static String sBottomControlsMediatorClassName =
            "org/chromium/chrome/browser/toolbar/bottom/BottomControlsMediator";
    static String sadrbrowsielBottomControlsMediatorClassName =
            "org/chromium/chrome/browser/toolbar/bottom/adrbrowsielBottomControlsMediator";

    public adrbrowsielBottomControlsMediatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sBottomControlsMediatorClassName, sadrbrowsielBottomControlsMediatorClassName);

        deleteField(sadrbrowsielBottomControlsMediatorClassName, "mBottomControlsHeight");
        makeProtectedField(sBottomControlsMediatorClassName, "mBottomControlsHeight");

        deleteField(sadrbrowsielBottomControlsMediatorClassName, "mModel");
        makeProtectedField(sBottomControlsMediatorClassName, "mModel");

        deleteField(sadrbrowsielBottomControlsMediatorClassName, "mBrowserControlsSizer");
        makeProtectedField(sBottomControlsMediatorClassName, "mBrowserControlsSizer");
    }
}
