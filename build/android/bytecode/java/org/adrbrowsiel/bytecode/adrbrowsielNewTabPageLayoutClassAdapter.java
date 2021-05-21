/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.adrbrowsiel.bytecode;

import org.objectweb.asm.ClassVisitor;

public class adrbrowsielNewTabPageLayoutClassAdapter extends adrbrowsielClassVisitor {
    static String sNewTabPageLayoutClassName = "org/chromium/chrome/browser/ntp/NewTabPageLayout";
    static String sadrbrowsielNewTabPageLayoutClassName =
            "org/chromium/chrome/browser/ntp/adrbrowsielNewTabPageLayout";
    static String sNewTabPageLayoutSuperClassName = "android/widget/FrameLayout";

    public adrbrowsielNewTabPageLayoutClassAdapter(ClassVisitor visitor) {
        super(visitor);
        makePublicMethod(sNewTabPageLayoutClassName, "insertSiteSectionView");
        addMethodAnnotation(sadrbrowsielNewTabPageLayoutClassName, "insertSiteSectionView",
                "Ljava/lang/Override;");

        deleteField(sadrbrowsielNewTabPageLayoutClassName, "mSiteSectionView");
        makeProtectedField(sNewTabPageLayoutClassName, "mSiteSectionView");

        deleteField(sadrbrowsielNewTabPageLayoutClassName, "mTileGroup");
        makeProtectedField(sNewTabPageLayoutClassName, "mTileGroup");

        makePublicMethod(sNewTabPageLayoutClassName, "updateTileGridPlaceholderVisibility");
        addMethodAnnotation(sadrbrowsielNewTabPageLayoutClassName, "updateTileGridPlaceholderVisibility",
                "Ljava/lang/Override;");

        changeSuperName(sNewTabPageLayoutClassName, sNewTabPageLayoutSuperClassName);
    }
}
