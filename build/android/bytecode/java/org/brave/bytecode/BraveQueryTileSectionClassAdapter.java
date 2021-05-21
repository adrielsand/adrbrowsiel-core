/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.adrbrowsiel.bytecode;

import org.objectweb.asm.ClassVisitor;

public class adrbrowsielQueryTileSectionClassAdapter extends adrbrowsielClassVisitor {
    static String sQueryTileSectionClassName =
            "org/chromium/chrome/browser/query_tiles/QueryTileSection";
    static String sadrbrowsielQueryTileSectionClassName =
            "org/chromium/chrome/browser/query_tiles/adrbrowsielQueryTileSection";

    public adrbrowsielQueryTileSectionClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeMethodOwner(sQueryTileSectionClassName, "getMaxRowsForMostVisitedTiles",
                sadrbrowsielQueryTileSectionClassName);
    }
}
