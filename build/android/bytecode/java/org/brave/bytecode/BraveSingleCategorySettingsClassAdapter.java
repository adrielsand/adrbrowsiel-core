/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.adrbrowsiel.bytecode;

import org.objectweb.asm.ClassVisitor;

public class adrbrowsielSingleCategorySettingsClassAdapter extends adrbrowsielClassVisitor {
    static String sSingleCategorySettingsClassName = "org/chromium/components/browser_ui/site_settings/SingleCategorySettings";
    static String sadrbrowsielSingleCategorySettingsClassName = "org/chromium/components/browser_ui/site_settings/adrbrowsielSingleCategorySettings";

    public adrbrowsielSingleCategorySettingsClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sSingleCategorySettingsClassName, sadrbrowsielSingleCategorySettingsClassName);
    }
}
