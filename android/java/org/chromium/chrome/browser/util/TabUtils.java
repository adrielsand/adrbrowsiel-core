/**
 * Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.util;

import android.app.Activity;
import android.content.Context;
import android.view.ContextThemeWrapper;
import android.view.MenuItem;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.PopupMenu;

import org.chromium.base.ApplicationStatus;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.app.adrbrowsielActivity;
import org.chromium.chrome.browser.app.ChromeActivity;
import org.chromium.chrome.browser.night_mode.GlobalNightModeStateProviderHolder;
import org.chromium.chrome.browser.tab.TabLaunchType;
import org.chromium.content_public.browser.LoadUrlParams;

public class TabUtils {
    public static void showTabPopupMenu(Context context, View view) {
        adrbrowsielActivity adrbrowsielActivity = adrbrowsielActivity.getadrbrowsielActivity();
        Context wrapper = new ContextThemeWrapper(context,
                GlobalNightModeStateProviderHolder.getInstance().isInNightMode()
                        ? R.style.NewTabPopupMenuDark
                        : R.style.NewTabPopupMenuLight);
        // Creating the instance of PopupMenu
        PopupMenu popup = new PopupMenu(wrapper, view);
        // Inflating the Popup using xml file
        popup.getMenuInflater().inflate(R.menu.new_tab_menu, popup.getMenu());

        if (adrbrowsielActivity != null && adrbrowsielActivity.getCurrentTabModel().isIncognito()) {
            popup.getMenu().findItem(R.id.new_tab_menu_id).setVisible(false);
        }
        // registering popup with OnMenuItemClickListener
        popup.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
            @Override
            public boolean onMenuItemClick(MenuItem item) {
                int id = item.getItemId();
                if (id == R.id.new_tab_menu_id) {
                    openNewTab(adrbrowsielActivity, false);
                } else if (id == R.id.new_incognito_tab_menu_id) {
                    openNewTab(adrbrowsielActivity, true);
                }
                return true;
            }
        });
        popup.show(); // showing popup menu
    }

    public static void openNewTab() {
        adrbrowsielActivity adrbrowsielActivity = adrbrowsielActivity.getadrbrowsielActivity();
        boolean isIncognito =
                adrbrowsielActivity != null ? adrbrowsielActivity.getCurrentTabModel().isIncognito() : false;
        openNewTab(adrbrowsielActivity, isIncognito);
    }

    private static void openNewTab(adrbrowsielActivity adrbrowsielActivity, boolean isIncognito) {
        if (adrbrowsielActivity == null) return;
        adrbrowsielActivity.getTabModelSelector().getModel(isIncognito).commitAllTabClosures();
        adrbrowsielActivity.getTabCreator(isIncognito).launchNTP();
    }

    public static void openUrlInNewTab(boolean isIncognito, String url) {
        adrbrowsielActivity adrbrowsielActivity = adrbrowsielActivity.getadrbrowsielActivity();
        if (adrbrowsielActivity != null) {
            adrbrowsielActivity.getTabCreator(isIncognito).launchUrl(url, TabLaunchType.FROM_CHROME_UI);
        }
    }

    public static void openUrlInSameTab(String url) {
        adrbrowsielActivity adrbrowsielActivity = adrbrowsielActivity.getadrbrowsielActivity();
        if (adrbrowsielActivity != null) {
            LoadUrlParams loadUrlParams = new LoadUrlParams(url);
            adrbrowsielActivity.getActivityTab().loadUrl(loadUrlParams);
        }
    }

    public static void enableRewardsButton() {
        adrbrowsielActivity adrbrowsielActivity = adrbrowsielActivity.getadrbrowsielActivity();
        if (adrbrowsielActivity == null || adrbrowsielActivity.getToolbarManager() == null) {
            return;
        }
        View toolbarView = adrbrowsielActivity.findViewById(R.id.toolbar);
        if (toolbarView == null) {
            return;
        }
        FrameLayout rewardsLayout = toolbarView.findViewById(R.id.adrbrowsiel_rewards_button_layout);
        if (rewardsLayout == null) {
            return;
        }
        rewardsLayout.setVisibility(View.VISIBLE);
    }
}
