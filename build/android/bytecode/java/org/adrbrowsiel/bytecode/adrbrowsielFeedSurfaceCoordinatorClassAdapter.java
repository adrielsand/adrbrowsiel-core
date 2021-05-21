/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.adrbrowsiel.bytecode;

import org.objectweb.asm.ClassVisitor;

public class adrbrowsielFeedSurfaceCoordinatorClassAdapter extends adrbrowsielClassVisitor {
    static String sFeedSurfaceCoordinatorClassName =
            "org/chromium/chrome/browser/feed/FeedSurfaceCoordinator";
    static String sadrbrowsielFeedSurfaceCoordinatorClassName =
            "org/chromium/chrome/browser/feed/adrbrowsielFeedSurfaceCoordinator";

    public adrbrowsielFeedSurfaceCoordinatorClassAdapter(ClassVisitor visitor) {
        super(visitor);
        makePublicMethod(sFeedSurfaceCoordinatorClassName, "isEnhancedProtectionPromoEnabled");
        addMethodAnnotation(sadrbrowsielFeedSurfaceCoordinatorClassName,
                "isEnhancedProtectionPromoEnabled", "Ljava/lang/Override;");

        deleteField(sadrbrowsielFeedSurfaceCoordinatorClassName, "mActivity");
        makeProtectedField(sFeedSurfaceCoordinatorClassName, "mActivity");

        deleteField(sadrbrowsielFeedSurfaceCoordinatorClassName, "mScrollViewForPolicy");
        makeProtectedField(sFeedSurfaceCoordinatorClassName, "mScrollViewForPolicy");

        deleteField(sadrbrowsielFeedSurfaceCoordinatorClassName, "mNtpHeader");
        makeProtectedField(sFeedSurfaceCoordinatorClassName, "mNtpHeader");

        deleteField(sadrbrowsielFeedSurfaceCoordinatorClassName, "mRootView");
        makeProtectedField(sFeedSurfaceCoordinatorClassName, "mRootView");
    }
}
