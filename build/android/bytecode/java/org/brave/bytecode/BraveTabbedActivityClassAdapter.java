/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.adrbrowsiel.bytecode;

import org.objectweb.asm.ClassVisitor;

public class adrbrowsielTabbedActivityClassAdapter extends adrbrowsielClassVisitor {
    static String sChromeTabbedActivityClassName =
            "org/chromium/chrome/browser/ChromeTabbedActivity";
    static String sadrbrowsielActivityClassName = "org/chromium/chrome/browser/app/adrbrowsielActivity";
    static String sTabbedRootUiCoordinatorClassName =
            "org/chromium/chrome/browser/tabbed_mode/TabbedRootUiCoordinator";
    static String sTabbedAppMenuPropertiesDelegateClassName =
            "org/chromium/chrome/browser/tabbed_mode/TabbedAppMenuPropertiesDelegate";
    static String sadrbrowsielTabbedAppMenuPropertiesDelegateClassName =
            "org/chromium/chrome/browser/appmenu/adrbrowsielTabbedAppMenuPropertiesDelegate";
    static String sChromeTabCreatorClassName =
            "org/chromium/chrome/browser/tabmodel/ChromeTabCreator";
    static String sadrbrowsielTabCreatorClassName =
            "org/chromium/chrome/browser/tabmodel/adrbrowsielTabCreator";
    static String sAppMenuPropertiesDelegateImplClassName =
            "org/chromium/chrome/browser/app/appmenu/AppMenuPropertiesDelegateImpl";
    static String sadrbrowsielAppMenuPropertiesDelegateImplClassName =
            "org/chromium/chrome/browser/app/appmenu/adrbrowsielAppMenuPropertiesDelegateImpl";
    static String sCustomTabAppMenuPropertiesDelegateClassName =
            "org/chromium/chrome/browser/customtabs/CustomTabAppMenuPropertiesDelegate";

    public adrbrowsielTabbedActivityClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sChromeTabbedActivityClassName, sadrbrowsielActivityClassName);

        changeSuperName(sTabbedAppMenuPropertiesDelegateClassName,
                sadrbrowsielAppMenuPropertiesDelegateImplClassName);

        changeSuperName(sCustomTabAppMenuPropertiesDelegateClassName,
                sadrbrowsielAppMenuPropertiesDelegateImplClassName);

        redirectConstructor(sTabbedAppMenuPropertiesDelegateClassName,
                sadrbrowsielTabbedAppMenuPropertiesDelegateClassName);

        redirectConstructor(sAppMenuPropertiesDelegateImplClassName,
                sadrbrowsielAppMenuPropertiesDelegateImplClassName);

        redirectConstructor(sChromeTabCreatorClassName, sadrbrowsielTabCreatorClassName);

        makePublicMethod(sChromeTabbedActivityClassName, "hideOverview");
    }
}
