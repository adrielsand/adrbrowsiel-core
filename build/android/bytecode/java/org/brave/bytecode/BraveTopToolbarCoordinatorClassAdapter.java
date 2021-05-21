/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.adrbrowsiel.bytecode;

import org.objectweb.asm.ClassVisitor;

public class adrbrowsielTopToolbarCoordinatorClassAdapter extends adrbrowsielClassVisitor {
    static String sTopToolbarCoordinatorClassName =
            "org/chromium/chrome/browser/toolbar/top/TopToolbarCoordinator";
    static String sadrbrowsielTopToolbarCoordinatorClassName =
            "org/chromium/chrome/browser/toolbar/top/adrbrowsielTopToolbarCoordinator";

    public adrbrowsielTopToolbarCoordinatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sadrbrowsielTopToolbarCoordinatorClassName, "mTabSwitcherModeCoordinatorPhone");
        makeProtectedField(sTopToolbarCoordinatorClassName, "mTabSwitcherModeCoordinatorPhone");

        deleteField(sadrbrowsielTopToolbarCoordinatorClassName, "mOptionalButtonController");
        makeProtectedField(sTopToolbarCoordinatorClassName, "mOptionalButtonController");
    }
}
