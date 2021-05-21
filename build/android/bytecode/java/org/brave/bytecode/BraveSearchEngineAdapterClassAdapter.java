/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.adrbrowsiel.bytecode;

import org.objectweb.asm.ClassVisitor;

public class adrbrowsielSearchEngineAdapterClassAdapter extends adrbrowsielClassVisitor {
    static String sSearchEngineAdapterClassName =
            "org/chromium/chrome/browser/search_engines/settings/SearchEngineAdapter";

    static String sadrbrowsielSearchEngineAdapterBaseClassName =
            "org/chromium/chrome/browser/search_engines/settings/adrbrowsielBaseSearchEngineAdapter";

    static String sSearchEngineSettingsClassName =
            "org/chromium/chrome/browser/search_engines/settings/SearchEngineSettings";

    static String sadrbrowsielSearchEnginePreferenceClassName =
            "org/chromium/chrome/browser/search_engines/settings/adrbrowsielSearchEnginePreference";

    static String sMethodGetPermissionsLinkMessage = "getPermissionsLinkMessage";

    static String sMethodGetSearchEngineSourceType = "getSearchEngineSourceType";

    static String sMethodSortAndFilterUnnecessaryTemplateUrl =
            "sortAndFilterUnnecessaryTemplateUrl";

    public adrbrowsielSearchEngineAdapterClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeSuperName(sSearchEngineAdapterClassName, sadrbrowsielSearchEngineAdapterBaseClassName);
        changeMethodOwner(sSearchEngineAdapterClassName, sMethodGetSearchEngineSourceType,
                sadrbrowsielSearchEngineAdapterBaseClassName);
        changeMethodOwner(sSearchEngineAdapterClassName, sMethodGetPermissionsLinkMessage,
                sadrbrowsielSearchEngineAdapterBaseClassName);
        changeMethodOwner(sSearchEngineAdapterClassName, sMethodSortAndFilterUnnecessaryTemplateUrl,
                sadrbrowsielSearchEngineAdapterBaseClassName);

        deleteField(sadrbrowsielSearchEnginePreferenceClassName, "mSearchEngineAdapter");
        makeProtectedField(sSearchEngineSettingsClassName, "mSearchEngineAdapter");

        makePublicMethod(sSearchEngineSettingsClassName, "createAdapterIfNecessary");
        addMethodAnnotation(sadrbrowsielSearchEnginePreferenceClassName, "createAdapterIfNecessary",
                "Ljava/lang/Override;");
    }
}
