/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.adrbrowsiel.bytecode;

import org.objectweb.asm.ClassVisitor;

public class adrbrowsielToolbarLayoutClassAdapter extends adrbrowsielClassVisitor {
    static String sCustomTabToolbarClassName =
            "org/chromium/chrome/browser/customtabs/features/toolbar/CustomTabToolbar";
    static String sToolbarPhoneClassName = "org/chromium/chrome/browser/toolbar/top/ToolbarPhone";
    static String sadrbrowsielToolbarLayoutClassName =
            "org/chromium/chrome/browser/toolbar/top/adrbrowsielToolbarLayout";

    public adrbrowsielToolbarLayoutClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeSuperName(sCustomTabToolbarClassName, sadrbrowsielToolbarLayoutClassName);

        changeSuperName(sToolbarPhoneClassName, sadrbrowsielToolbarLayoutClassName);
    }
}
