/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.adrbrowsiel.bytecode;

import org.objectweb.asm.ClassVisitor;

public class adrbrowsielBookmarkUtilsClassAdapter extends adrbrowsielClassVisitor {
    static String sBookmarkUtilsClassName = "org/chromium/chrome/browser/bookmarks/BookmarkUtils";
    static String sadrbrowsielBookmarkUtilsClassName =
            "org/chromium/chrome/browser/bookmarks/adrbrowsielBookmarkUtils";

    public adrbrowsielBookmarkUtilsClassAdapter(ClassVisitor visitor) {
        super(visitor);
        makePublicMethod(sBookmarkUtilsClassName, "showBookmarkBottomSheet");
        changeMethodOwner(
                sadrbrowsielBookmarkUtilsClassName, "showBookmarkBottomSheet", sBookmarkUtilsClassName);
        makePublicMethod(sBookmarkUtilsClassName, "addBookmarkAndShowSnackbar");
        changeMethodOwner(sadrbrowsielBookmarkUtilsClassName, "addBookmarkAndShowSnackbar",
                sBookmarkUtilsClassName);
        changeMethodOwner(
                sBookmarkUtilsClassName, "addOrEditBookmark", sadrbrowsielBookmarkUtilsClassName);
    }
}
