/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.adrbrowsiel.bytecode;

import org.objectweb.asm.ClassVisitor;

public class adrbrowsielTabGroupUiCoordinatorClassAdapter extends adrbrowsielClassVisitor {
    static String sTabGroupUiCoordinatorClassName =
            "org/chromium/chrome/browser/tasks/tab_management/TabGroupUiCoordinator";
    static String sadrbrowsielTabGroupUiCoordinatorClassName =
            "org/chromium/chrome/browser/tasks/tab_management/adrbrowsielTabGroupUiCoordinator";

    public adrbrowsielTabGroupUiCoordinatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sTabGroupUiCoordinatorClassName, sadrbrowsielTabGroupUiCoordinatorClassName);

        deleteField(sadrbrowsielTabGroupUiCoordinatorClassName, "mToolbarView");
        makeProtectedField(sTabGroupUiCoordinatorClassName, "mToolbarView");
    }
}
