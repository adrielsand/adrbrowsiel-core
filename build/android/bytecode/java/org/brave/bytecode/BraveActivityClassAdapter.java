/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.adrbrowsiel.bytecode;

import org.objectweb.asm.ClassVisitor;

public class adrbrowsielActivityClassAdapter extends adrbrowsielClassVisitor {
    static String sadrbrowsielActivityClassName = "org/chromium/chrome/browser/app/adrbrowsielActivity";

    public adrbrowsielActivityClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sadrbrowsielActivityClassName, "mTabModelProfileSupplier");
    }
}
