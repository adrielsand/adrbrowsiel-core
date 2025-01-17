/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.toolbar.bottom;

import android.view.View;

import org.chromium.ui.modelutil.PropertyKey;
import org.chromium.ui.modelutil.PropertyModelChangeProcessor;

/**
 * This class is responsible for pushing updates to both the Android view and the compositor
 * component of the browsing mode bottom toolbar. These updates are pulled from the
 * {@link BrowsingModeBottomToolbarModel} when a notification of an update is received.
 */
public class BrowsingModeBottomToolbarViewBinder
        implements PropertyModelChangeProcessor
                           .ViewBinder<BrowsingModeBottomToolbarModel, View, PropertyKey> {
    /**
     * Build a binder that handles interaction between the model and the views that make up the
     * browsing mode bottom toolbar.
     */
    BrowsingModeBottomToolbarViewBinder() {}

    @Override
    public final void bind(
            BrowsingModeBottomToolbarModel model, View view, PropertyKey propertyKey) {
        if (BrowsingModeBottomToolbarModel.PRIMARY_COLOR == propertyKey) {
            view.setBackgroundColor(model.get(BrowsingModeBottomToolbarModel.PRIMARY_COLOR));
        } else if (BrowsingModeBottomToolbarModel.IS_VISIBLE == propertyKey) {
            view.setVisibility(model.get(BrowsingModeBottomToolbarModel.IS_VISIBLE) ? View.VISIBLE
                                                                                    : View.GONE);
        } else {
            assert false : "Unhandled property detected in BrowsingModeBottomToolbarViewBinder!";
        }
    }
}
