/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.toolbar.bottom;

import android.app.Activity;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnLongClickListener;
import android.view.ViewGroup;
import android.view.ViewStub;

import androidx.annotation.Nullable;

import org.chromium.base.Callback;
import org.chromium.base.supplier.ObservableSupplier;
import org.chromium.base.supplier.ObservableSupplierImpl;
import org.chromium.base.supplier.OneShotCallback;
import org.chromium.base.supplier.OneshotSupplier;
import org.chromium.base.supplier.Supplier;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.ActivityTabProvider;
import org.chromium.chrome.browser.browser_controls.BrowserControlsSizer;
import org.chromium.chrome.browser.compositor.layouts.LayoutManagerImpl;
import org.chromium.chrome.browser.fullscreen.FullscreenManager;
import org.chromium.chrome.browser.layouts.LayoutManager;
import org.chromium.chrome.browser.layouts.LayoutStateProvider;
import org.chromium.chrome.browser.tabmodel.IncognitoStateProvider;
import org.chromium.chrome.browser.theme.ThemeColorProvider;
import org.chromium.chrome.browser.toolbar.HomeButton;
import org.chromium.chrome.browser.toolbar.TabCountProvider;
import org.chromium.chrome.browser.ui.appmenu.AppMenuButtonHelper;
import org.chromium.components.browser_ui.widget.scrim.ScrimCoordinator;
import org.chromium.ui.base.WindowAndroid;
import org.chromium.ui.resources.ResourceManager;

public class adrbrowsielBottomControlsCoordinator extends BottomControlsCoordinator {
    // To delete in bytecode, members from parent class will be used instead.
    private BottomControlsMediator mMediator;

    // Own members.
    private @Nullable BottomToolbarCoordinator mBottomToolbarCoordinator;
    private OnLongClickListener mTabSwitcherLongclickListener;
    private ActivityTabProvider mTabProvider;
    private ThemeColorProvider mThemeColorProvider;
    private ObservableSupplier<AppMenuButtonHelper> mMenuButtonHelperSupplier;
    private Runnable mOpenHomepageAction;
    private Callback<Integer> mSetUrlBarFocusAction;
    private OneshotSupplier<LayoutStateProvider> mLayoutStateProviderSupplier;
    private ScrollingBottomViewResourceFrameLayout mRoot;

    public adrbrowsielBottomControlsCoordinator(
            OneshotSupplier<LayoutStateProvider> layoutStateProviderSupplier,
            OnLongClickListener tabSwitcherLongclickListener, ActivityTabProvider tabProvider,
            Runnable openHomepageAction, Callback<Integer> setUrlBarFocusAction,
            ObservableSupplier<AppMenuButtonHelper> menuButtonHelperSupplier,
            /* Below are parameters from BottomControlsCoordinator */
            Activity activity, WindowAndroid windowAndroid, LayoutManager layoutManager,
            ResourceManager resourceManager, BrowserControlsSizer controlsSizer,
            FullscreenManager fullscreenManager, ScrollingBottomViewResourceFrameLayout root,
            ThemeColorProvider themeColorProvider, BottomControlsContentDelegate contentDelegate,
            ObservableSupplier<Boolean> overlayPanelVisibilitySupplier) {
        super(activity, windowAndroid, layoutManager, resourceManager, controlsSizer,
                fullscreenManager, root, themeColorProvider, contentDelegate,
                overlayPanelVisibilitySupplier);

        mTabSwitcherLongclickListener = tabSwitcherLongclickListener;
        mTabProvider = tabProvider;
        mThemeColorProvider = themeColorProvider;
        mOpenHomepageAction = openHomepageAction;
        mSetUrlBarFocusAction = setUrlBarFocusAction;
        mLayoutStateProviderSupplier = layoutStateProviderSupplier;
        mMenuButtonHelperSupplier = menuButtonHelperSupplier;
        mRoot = root;
    }

    public void initializeWithNative(Activity activity, ResourceManager resourceManager,
            LayoutManagerImpl layoutManager, OnClickListener tabSwitcherListener,
            OnClickListener newTabClickListener, WindowAndroid windowAndroid,
            TabCountProvider tabCountProvider, IncognitoStateProvider incognitoStateProvider,
            ViewGroup topToolbarRoot, Runnable closeAllTabsAction) {
        if (BottomToolbarConfiguration.isBottomToolbarEnabled()) {
            mBottomToolbarCoordinator =
                    new BottomToolbarCoordinator(mRoot, mRoot.findViewById(R.id.bottom_toolbar),
                            mTabProvider, mTabSwitcherLongclickListener, mThemeColorProvider,
                            mOpenHomepageAction, mSetUrlBarFocusAction,
                            mLayoutStateProviderSupplier, mMenuButtonHelperSupplier, mMediator);

            mBottomToolbarCoordinator.initializeWithNative(tabSwitcherListener, newTabClickListener,
                    tabCountProvider, incognitoStateProvider, topToolbarRoot, closeAllTabsAction);
        }
    }

    @Override
    public void destroy() {
        super.destroy();

        if (mBottomToolbarCoordinator != null) mBottomToolbarCoordinator.destroy();
    }

    public void updateBookmarkButton(boolean isBookmarked, boolean editingAllowed) {
        if (mBottomToolbarCoordinator != null) {
            mBottomToolbarCoordinator.updateBookmarkButton(isBookmarked, editingAllowed);
        }
    }

    public void updateHomeButtonState() {
        if (mBottomToolbarCoordinator != null) {
            mBottomToolbarCoordinator.updateHomeButtonState();
        }
    }

    public void setBottomToolbarVisible(boolean visible) {
        if (mMediator instanceof adrbrowsielBottomControlsMediator) {
            ((adrbrowsielBottomControlsMediator) mMediator).setBottomToolbarVisible(visible);
        }
        if (mBottomToolbarCoordinator != null) {
            mBottomToolbarCoordinator.setBottomToolbarVisible(visible);
        }
    }

    public ObservableSupplierImpl<Boolean> getBottomToolbarVisibleSupplier() {
        if (mMediator instanceof adrbrowsielBottomControlsMediator) {
            return ((adrbrowsielBottomControlsMediator) mMediator).getBottomToolbarVisibleSupplier();
        }
        assert false : "Make sure mMediator is properly patched in bytecode.";
        return null;
    }

    public ObservableSupplierImpl<Boolean> getTabGroupUiVisibleSupplier() {
        if (mMediator instanceof adrbrowsielBottomControlsMediator) {
            return ((adrbrowsielBottomControlsMediator) mMediator).getTabGroupUiVisibleSupplier();
        }
        assert false : "Make sure mMediator is properly patched in bytecode.";
        return null;
    }
}
