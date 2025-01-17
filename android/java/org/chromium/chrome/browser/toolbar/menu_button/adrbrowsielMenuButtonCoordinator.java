/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.toolbar.menu_button;

import android.app.Activity;
import android.content.SharedPreferences;
import android.graphics.Canvas;
import android.view.View;

import androidx.annotation.IdRes;

import org.chromium.base.ContextUtils;
import org.chromium.base.supplier.OneshotSupplier;
import org.chromium.base.supplier.Supplier;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.browser_controls.BrowserStateBrowserControlsVisibilityDelegate;
import org.chromium.chrome.browser.theme.ThemeColorProvider;
import org.chromium.chrome.browser.toolbar.top.adrbrowsielToolbarLayout;
import org.chromium.chrome.browser.ui.appmenu.AppMenuCoordinator;
import org.chromium.ui.base.WindowAndroid;

public class adrbrowsielMenuButtonCoordinator extends MenuButtonCoordinator {
    private static final String adrbrowsiel_IS_MENU_FROM_BOTTOM = "adrbrowsiel_is_menu_from_bottom";

    private Activity mActivity;

    public adrbrowsielMenuButtonCoordinator(
            OneshotSupplier<AppMenuCoordinator> appMenuCoordinatorSupplier,
            BrowserStateBrowserControlsVisibilityDelegate controlsVisibilityDelegate,
            WindowAndroid windowAndroid, SetFocusFunction setUrlBarFocusFunction,
            Runnable requestRenderRunnable, boolean shouldShowAppUpdateBadge,
            Supplier<Boolean> isInOverviewModeSupplier, ThemeColorProvider themeColorProvider,
            @IdRes int menuButtonId) {
        super(appMenuCoordinatorSupplier, controlsVisibilityDelegate, windowAndroid,
                setUrlBarFocusFunction, requestRenderRunnable, shouldShowAppUpdateBadge,
                isInOverviewModeSupplier, themeColorProvider, menuButtonId);

        mActivity = windowAndroid.getActivity().get();
    }

    @Override
    public MenuButton getMenuButton() {
        updateMenuButtonState();
        return isMenuFromBottom() ? null : super.getMenuButton();
    }

    @Override
    public void drawTabSwitcherAnimationOverlay(View root, Canvas canvas, int alpha) {
        if (isMenuFromBottom()) return;
        super.drawTabSwitcherAnimationOverlay(root, canvas, alpha);
    }

    @Override
    public void setVisibility(boolean visible) {
        updateMenuButtonState();
        super.setVisibility(isMenuFromBottom() ? false : visible);
    }

    private void updateMenuButtonState() {
        adrbrowsielToolbarLayout layout = (adrbrowsielToolbarLayout) mActivity.findViewById(R.id.toolbar);
        assert layout != null;
        if (layout != null) {
            layout.updateMenuButtonState();
        }
    }

    public static void setMenuFromBottom(boolean isMenuFromBottom) {
        SharedPreferences prefs = ContextUtils.getAppSharedPreferences();
        prefs.edit().putBoolean(adrbrowsiel_IS_MENU_FROM_BOTTOM, isMenuFromBottom).apply();
    }

    public static boolean isMenuFromBottom() {
        SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
        return sharedPreferences.getBoolean(adrbrowsiel_IS_MENU_FROM_BOTTOM, true);
    }
}
