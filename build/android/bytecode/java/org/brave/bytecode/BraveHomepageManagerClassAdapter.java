/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.adrbrowsiel.bytecode;

import org.objectweb.asm.ClassVisitor;

public class adrbrowsielHomepageManagerClassAdapter extends adrbrowsielClassVisitor {
    static String sHomepageManagerClassName = "org/chromium/chrome/browser/homepage/HomepageManager";
    static String sadrbrowsielHomepageManagerClassName = "org/chromium/chrome/browser/homepage/adrbrowsielHomepageManager";

    public adrbrowsielHomepageManagerClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeMethodOwner(sHomepageManagerClassName, "shouldCloseAppWithZeroTabs", sadrbrowsielHomepageManagerClassName);
    }
}
