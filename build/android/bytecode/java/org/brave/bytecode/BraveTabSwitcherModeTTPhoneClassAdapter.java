/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.adrbrowsiel.bytecode;

import org.objectweb.asm.ClassVisitor;

public class adrbrowsielTabSwitcherModeTTPhoneClassAdapter extends adrbrowsielClassVisitor {
    static String sTabSwitcherModeTTPhoneClassName =
            "org/chromium/chrome/browser/toolbar/top/TabSwitcherModeTTPhone";
    static String sadrbrowsielTabSwitcherModeTTPhoneClassName =
            "org/chromium/chrome/browser/toolbar/top/adrbrowsielTabSwitcherModeTTPhone";

    public adrbrowsielTabSwitcherModeTTPhoneClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sadrbrowsielTabSwitcherModeTTPhoneClassName, "mNewTabViewButton");
        makeProtectedField(sTabSwitcherModeTTPhoneClassName, "mNewTabViewButton");

        deleteField(sadrbrowsielTabSwitcherModeTTPhoneClassName, "mNewTabImageButton");
        makeProtectedField(sTabSwitcherModeTTPhoneClassName, "mNewTabImageButton");

        deleteField(sadrbrowsielTabSwitcherModeTTPhoneClassName, "mToggleTabStackButton");
        makeProtectedField(sTabSwitcherModeTTPhoneClassName, "mToggleTabStackButton");

        deleteField(sadrbrowsielTabSwitcherModeTTPhoneClassName, "mShouldShowNewTabVariation");
        makeProtectedField(sTabSwitcherModeTTPhoneClassName, "mShouldShowNewTabVariation");

        makePublicMethod(sTabSwitcherModeTTPhoneClassName, "updateNewTabButtonVisibility");
        addMethodAnnotation(sadrbrowsielTabSwitcherModeTTPhoneClassName, "updateNewTabButtonVisibility",
                "Ljava/lang/Override;");

        makePublicMethod(sTabSwitcherModeTTPhoneClassName, "shouldShowIncognitoToggle");
        deleteMethod(sadrbrowsielTabSwitcherModeTTPhoneClassName, "shouldShowIncognitoToggle");
    }
}
