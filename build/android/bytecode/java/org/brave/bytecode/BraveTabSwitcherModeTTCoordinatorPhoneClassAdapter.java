/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.adrbrowsiel.bytecode;

import org.objectweb.asm.ClassVisitor;

public class adrbrowsielTabSwitcherModeTTCoordinatorPhoneClassAdapter extends adrbrowsielClassVisitor {
    static String sTabSwitcherModeTTCoordinatorPhoneClassName =
            "org/chromium/chrome/browser/toolbar/top/TabSwitcherModeTTCoordinatorPhone";
    static String sadrbrowsielTabSwitcherModeTTCoordinatorPhoneClassName =
            "org/chromium/chrome/browser/toolbar/top/adrbrowsielTabSwitcherModeTTCoordinatorPhone";

    public adrbrowsielTabSwitcherModeTTCoordinatorPhoneClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sadrbrowsielTabSwitcherModeTTCoordinatorPhoneClassName, "mTabSwitcherModeToolbar");
        makeProtectedField(sTabSwitcherModeTTCoordinatorPhoneClassName, "mTabSwitcherModeToolbar");
    }
}
