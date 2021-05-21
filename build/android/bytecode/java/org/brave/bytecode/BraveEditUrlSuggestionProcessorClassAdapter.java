/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.adrbrowsiel.bytecode;

import org.objectweb.asm.ClassVisitor;

public class adrbrowsielEditUrlSuggestionProcessorClassAdapter extends adrbrowsielClassVisitor {
    static String sEditUrlSuggestionProcessor =
            "org/chromium/chrome/browser/omnibox/suggestions/editurl/EditUrlSuggestionProcessor";
    static String sadrbrowsielEditUrlSuggestionProcessor =
            "org/chromium/chrome/browser/omnibox/suggestions/editurl/adrbrowsielEditUrlSuggestionProcessor";

    public adrbrowsielEditUrlSuggestionProcessorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sadrbrowsielEditUrlSuggestionProcessor, "mHasClearedOmniboxForFocus");
        makeProtectedField(sEditUrlSuggestionProcessor, "mHasClearedOmniboxForFocus");
    }
}
