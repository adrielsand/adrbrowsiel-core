/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.toolbar.top;

import android.animation.Animator;
import android.animation.ObjectAnimator;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.PorterDuff;
import android.graphics.drawable.Drawable;
import android.text.Spannable;
import android.text.SpannableString;
import android.text.SpannableStringBuilder;
import android.text.Spanned;
import android.text.TextPaint;
import android.text.method.LinkMovementMethod;
import android.text.style.ClickableSpan;
import android.text.style.ForegroundColorSpan;
import android.text.style.ImageSpan;
import android.util.AttributeSet;
import android.util.Pair;
import android.view.Gravity;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.PopupWindow;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.widget.AppCompatImageView;
import androidx.core.content.ContextCompat;
import androidx.core.content.res.ResourcesCompat;

import org.chromium.base.ApiCompatibilityUtils;
import org.chromium.base.adrbrowsielReflectionUtil;
import org.chromium.base.ContextUtils;
import org.chromium.base.Log;
import org.chromium.base.MathUtils;
import org.chromium.base.ThreadUtils;
import org.chromium.base.supplier.BooleanSupplier;
import org.chromium.base.task.AsyncTask;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.adrbrowsielAdsNativeHelper;
import org.chromium.chrome.browser.adrbrowsielFeatureList;
import org.chromium.chrome.browser.adrbrowsielRelaunchUtils;
import org.chromium.chrome.browser.adrbrowsielRewardsHelper;
import org.chromium.chrome.browser.adrbrowsielRewardsNativeWorker;
import org.chromium.chrome.browser.adrbrowsielRewardsObserver;
import org.chromium.chrome.browser.adrbrowsielRewardsPanelPopup;
import org.chromium.chrome.browser.app.adrbrowsielActivity;
import org.chromium.chrome.browser.adrbrowsiel_stats.adrbrowsielStatsUtil;
import org.chromium.chrome.browser.custom_layout.popup_window_tooltip.PopupWindowTooltip;
import org.chromium.chrome.browser.custom_layout.popup_window_tooltip.PopupWindowTooltipUtils;
import org.chromium.chrome.browser.customtabs.CustomTabActivity;
import org.chromium.chrome.browser.customtabs.features.toolbar.CustomTabToolbar;
import org.chromium.chrome.browser.dialogs.adrbrowsielAdsSignupDialog;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.chrome.browser.lifecycle.ConfigurationChangedObserver;
import org.chromium.chrome.browser.local_database.adrbrowsielStatsTable;
import org.chromium.chrome.browser.local_database.DatabaseHelper;
import org.chromium.chrome.browser.local_database.SavedBandwidthTable;
import org.chromium.chrome.browser.notifications.retention.RetentionNotificationUtil;
import org.chromium.chrome.browser.ntp.adrbrowsielNewTabPageLayout;
import org.chromium.chrome.browser.ntp.NewTabPage;
import org.chromium.chrome.browser.onboarding.OnboardingPrefManager;
import org.chromium.chrome.browser.onboarding.SearchActivity;
import org.chromium.chrome.browser.preferences.adrbrowsielPref;
import org.chromium.chrome.browser.preferences.adrbrowsielPrefServiceBridge;
import org.chromium.chrome.browser.preferences.website.adrbrowsielShieldsContentSettings;
import org.chromium.chrome.browser.preferences.website.adrbrowsielShieldsContentSettingsObserver;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.settings.AppearancePreferences;
import org.chromium.chrome.browser.settings.adrbrowsielSearchEngineUtils;
import org.chromium.chrome.browser.shields.adrbrowsielShieldsHandler;
import org.chromium.chrome.browser.shields.adrbrowsielShieldsMenuObserver;
import org.chromium.chrome.browser.shields.adrbrowsielShieldsUtils;
import org.chromium.chrome.browser.shields.ShieldsTooltipEnum;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.tab.TabImpl;
import org.chromium.chrome.browser.tab.TabSelectionType;
import org.chromium.chrome.browser.tabmodel.TabModelSelector;
import org.chromium.chrome.browser.tabmodel.TabModelSelectorTabModelObserver;
import org.chromium.chrome.browser.tabmodel.TabModelSelectorTabObserver;
import org.chromium.chrome.browser.theme.ThemeUtils;
import org.chromium.chrome.browser.toolbar.HomeButton;
import org.chromium.chrome.browser.toolbar.ToolbarColors;
import org.chromium.chrome.browser.toolbar.ToolbarDataProvider;
import org.chromium.chrome.browser.toolbar.ToolbarTabController;
import org.chromium.chrome.browser.toolbar.bottom.BottomToolbarVariationManager;
import org.chromium.chrome.browser.toolbar.menu_button.adrbrowsielMenuButtonCoordinator;
import org.chromium.chrome.browser.toolbar.menu_button.MenuButtonCoordinator;
import org.chromium.chrome.browser.toolbar.top.ToolbarLayout;
import org.chromium.chrome.browser.util.PackageUtils;
import org.chromium.components.browser_ui.styles.ChromeColors;
import org.chromium.components.embedder_support.util.UrlConstants;
import org.chromium.components.embedder_support.util.UrlUtilities;
import org.chromium.components.url_formatter.UrlFormatter;
import org.chromium.components.user_prefs.UserPrefs;
import org.chromium.content_public.browser.NavigationHandle;
import org.chromium.ui.UiUtils;
import org.chromium.ui.interpolators.BakedBezierInterpolator;
import org.chromium.ui.widget.Toast;
import org.chromium.url.GURL;

import java.net.URL;
import java.util.Calendar;
import java.util.Date;
import java.util.EnumSet;
import java.util.List;
import java.util.Locale;

public abstract class adrbrowsielToolbarLayout extends ToolbarLayout
        implements OnClickListener, View.OnLongClickListener, adrbrowsielRewardsObserver,
                   adrbrowsielRewardsNativeWorker.PublisherObserver {
    public static final String PREF_HIDE_adrbrowsiel_REWARDS_ICON = "hide_adrbrowsiel_rewards_icon";
    private static final String JAPAN_COUNTRY_CODE = "JP";

    private static final long MB_10 = 10000000;
    private static final long MINUTES_10 = 10 * 60 * 1000;

    private static final int URL_FOCUS_TOOLBAR_BUTTONS_TRANSLATION_X_DP = 10;

    private DatabaseHelper mDatabaseHelper = DatabaseHelper.getInstance();

    private ImageButton madrbrowsielShieldsButton;
    private ImageButton madrbrowsielRewardsButton;
    private HomeButton mHomeButton;
    private FrameLayout mShieldsLayout;
    private FrameLayout mRewardsLayout;
    private adrbrowsielShieldsHandler madrbrowsielShieldsHandler;
    private TabModelSelectorTabObserver mTabModelSelectorTabObserver;
    private TabModelSelectorTabModelObserver mTabModelSelectorTabModelObserver;
    private adrbrowsielRewardsNativeWorker madrbrowsielRewardsNativeWorker;
    private adrbrowsielRewardsPanelPopup mRewardsPopup;
    private adrbrowsielShieldsContentSettings madrbrowsielShieldsContentSettings;
    private adrbrowsielShieldsContentSettingsObserver madrbrowsielShieldsContentSettingsObserver;
    private TextView madrbrowsielRewardsNotificationsCount;
    private ImageView madrbrowsielRewardsOnboardingIcon;
    private boolean mShieldsLayoutIsColorBackground;
    private int mCurrentToolbarColor;

    private boolean mIsPublisherVerified;
    private boolean mIsNotificationPosted;
    private boolean mIsInitialNotificationPosted; // initial red circle notification

    private PopupWindowTooltip mShieldsPopupWindowTooltip;

    private boolean mIsBottomToolbarVisible;

    public adrbrowsielToolbarLayout(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    void destroy() {
        if (madrbrowsielShieldsContentSettings != null) {
            madrbrowsielShieldsContentSettings.removeObserver(madrbrowsielShieldsContentSettingsObserver);
        }
        super.destroy();

        if (madrbrowsielRewardsNativeWorker != null) {
            madrbrowsielRewardsNativeWorker.RemoveObserver(this);
            madrbrowsielRewardsNativeWorker.RemovePublisherObserver(this);
        }
    }

    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();

        if (this instanceof ToolbarTablet) {
            ImageButton forwardButton = findViewById(R.id.forward_button);
            if (forwardButton != null) {
                final Drawable forwardButtonDrawable = UiUtils.getTintedDrawable(getContext(),
                        R.drawable.btn_right_tablet, R.color.default_icon_color_tint_list);
                forwardButton.setImageDrawable(forwardButtonDrawable);
            }
        }

        mShieldsLayout = (FrameLayout) findViewById(R.id.adrbrowsiel_shields_button_layout);
        mRewardsLayout = (FrameLayout) findViewById(R.id.adrbrowsiel_rewards_button_layout);
        madrbrowsielRewardsNotificationsCount = (TextView) findViewById(R.id.br_notifications_count);
        madrbrowsielRewardsOnboardingIcon = findViewById(R.id.br_rewards_onboarding_icon);
        madrbrowsielShieldsButton = (ImageButton) findViewById(R.id.adrbrowsiel_shields_button);
        madrbrowsielRewardsButton = (ImageButton) findViewById(R.id.adrbrowsiel_rewards_button);
        mHomeButton = (HomeButton) findViewById(R.id.home_button);

        if (mHomeButton != null) {
            mHomeButton.setOnLongClickListener(this);
        }

        if (madrbrowsielShieldsButton != null) {
            madrbrowsielShieldsButton.setClickable(true);
            madrbrowsielShieldsButton.setOnClickListener(this);
            madrbrowsielShieldsButton.setOnLongClickListener(this);
        }

        if (madrbrowsielRewardsButton != null) {
            madrbrowsielRewardsButton.setClickable(true);
            madrbrowsielRewardsButton.setOnClickListener(this);
            madrbrowsielRewardsButton.setOnLongClickListener(this);
        }

        madrbrowsielShieldsHandler = new adrbrowsielShieldsHandler(getContext());
        madrbrowsielShieldsHandler.addObserver(new adrbrowsielShieldsMenuObserver() {
            @Override
            public void onMenuTopShieldsChanged(boolean isOn, boolean isTopShield) {
                Tab currentTab = getToolbarDataProvider().getTab();
                if (currentTab == null) {
                    return;
                }
                if (isTopShield) {
                    updateadrbrowsielShieldsButtonState(currentTab);
                }
                if (currentTab.isLoading()) {
                    currentTab.stopLoading();
                }
                currentTab.reloadIgnoringCache();
                if (null != madrbrowsielShieldsHandler) {
                    // Clean the adrbrowsielry Panel
                    madrbrowsielShieldsHandler.updateValues(0, 0, 0, 0);
                }
            }
        });
        madrbrowsielShieldsContentSettingsObserver = new adrbrowsielShieldsContentSettingsObserver() {
            @Override
            public void blockEvent(int tabId, String block_type, String subresource) {
                madrbrowsielShieldsHandler.addStat(tabId, block_type, subresource);
                Tab currentTab = getToolbarDataProvider().getTab();
                if (currentTab == null || currentTab.getId() != tabId) {
                    return;
                }
                madrbrowsielShieldsHandler.updateValues(tabId);
                if (!isIncognito() && OnboardingPrefManager.getInstance().isadrbrowsielStatsEnabled()
                        && (block_type.equals(adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_ADS)
                                || block_type.equals(adrbrowsielShieldsContentSettings
                                                             .RESOURCE_IDENTIFIER_TRACKERS))) {
                    addStatsToDb(block_type, subresource, currentTab.getUrlString());
                }
            }

            @Override
            public void savedBandwidth(long savings) {
                if (!isIncognito() && OnboardingPrefManager.getInstance().isadrbrowsielStatsEnabled()) {
                    addSavedBandwidthToDb(savings);
                }
            }
        };
        // Initially show shields off image. Shields button state will be updated when tab is
        // shown and loading state is changed.
        updateadrbrowsielShieldsButtonState(null);
        if (adrbrowsielReflectionUtil.EqualTypes(this.getClass(), ToolbarPhone.class)) {
            if (getMenuButtonCoordinator() != null && isMenuButtonOnBottom()) {
                getMenuButtonCoordinator().setVisibility(false);
            }
        }

        if (adrbrowsielReflectionUtil.EqualTypes(this.getClass(), CustomTabToolbar.class)) {
            LinearLayout customActionButtons = findViewById(R.id.action_buttons);
            assert customActionButtons != null : "Something has changed in the upstream!";
            if (customActionButtons != null && madrbrowsielShieldsButton != null) {
                ViewGroup.MarginLayoutParams adrbrowsielShieldsButtonLayout =
                        (ViewGroup.MarginLayoutParams) madrbrowsielShieldsButton.getLayoutParams();
                ViewGroup.MarginLayoutParams actionButtonsLayout =
                        (ViewGroup.MarginLayoutParams) customActionButtons.getLayoutParams();
                actionButtonsLayout.setMarginEnd(actionButtonsLayout.getMarginEnd()
                        + adrbrowsielShieldsButtonLayout.getMarginEnd());
                customActionButtons.setLayoutParams(actionButtonsLayout);
            }
        }
    }

    @Override
    protected void onNativeLibraryReady() {
        super.onNativeLibraryReady();
        madrbrowsielShieldsContentSettings = adrbrowsielShieldsContentSettings.getInstance();
        madrbrowsielShieldsContentSettings.addObserver(madrbrowsielShieldsContentSettingsObserver);

        SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
        if (ChromeFeatureList.isEnabled(adrbrowsielFeatureList.adrbrowsiel_REWARDS)
                && !adrbrowsielPrefServiceBridge.getInstance().getSafetynetCheckFailed()
                && !sharedPreferences.getBoolean(
                        AppearancePreferences.PREF_HIDE_adrbrowsiel_REWARDS_ICON, false)
                && mRewardsLayout != null) {
            mRewardsLayout.setVisibility(View.VISIBLE);
        }
        if (mShieldsLayout != null) {
            updateShieldsLayoutBackground(
                    !(mRewardsLayout != null && mRewardsLayout.getVisibility() == View.VISIBLE));
            mShieldsLayout.setVisibility(View.VISIBLE);
        }
        madrbrowsielRewardsNativeWorker = adrbrowsielRewardsNativeWorker.getInstance();
        if (madrbrowsielRewardsNativeWorker != null) {
            madrbrowsielRewardsNativeWorker.AddObserver(this);
            madrbrowsielRewardsNativeWorker.AddPublisherObserver(this);
            madrbrowsielRewardsNativeWorker.TriggerOnNotifyFrontTabUrlChanged();
            madrbrowsielRewardsNativeWorker.GetAllNotifications();
        }
    }

    @Override
    public void setTabModelSelector(TabModelSelector selector) {
        // We might miss events before calling setTabModelSelector, so we need
        // to proactively update the shields button state here, otherwise shields
        // might sometimes show as disabled while it is actually enabled.
        updateadrbrowsielShieldsButtonState(getToolbarDataProvider().getTab());
        mTabModelSelectorTabObserver = new TabModelSelectorTabObserver(selector) {
            @Override
            public void onShown(Tab tab, @TabSelectionType int type) {
                // Update shields button state when visible tab is changed.
                updateadrbrowsielShieldsButtonState(tab);
            }

            @Override
            public void onPageLoadStarted(Tab tab, GURL url) {
                if (getToolbarDataProvider().getTab() == tab) {
                    updateadrbrowsielShieldsButtonState(tab);
                }
                madrbrowsielShieldsHandler.clearadrbrowsielShieldsCount(tab.getId());
                dismissShieldsTooltip();
            }

            @Override
            public void onPageLoadFinished(final Tab tab, GURL url) {
                if (getToolbarDataProvider().getTab() == tab) {
                    madrbrowsielShieldsHandler.updateHost(url.getSpec());
                    updateadrbrowsielShieldsButtonState(tab);

                    Profile mProfile = Profile.getLastUsedRegularProfile();
                    long trackersBlockedCount =
                            adrbrowsielPrefServiceBridge.getInstance().getTrackersBlockedCount(mProfile);
                    long adsBlockedCount =
                            adrbrowsielPrefServiceBridge.getInstance().getAdsBlockedCount(mProfile);
                    long dataSaved = adrbrowsielPrefServiceBridge.getInstance().getDataSaved(mProfile);
                    long estimatedMillisecondsSaved = (trackersBlockedCount + adsBlockedCount)
                            * adrbrowsielStatsUtil.MILLISECONDS_PER_ITEM;

                    if (!OnboardingPrefManager.getInstance().isAdsTrackersNotificationStarted()
                            && (trackersBlockedCount + adsBlockedCount) > 250
                            && PackageUtils.isFirstInstall(getContext())) {
                        RetentionNotificationUtil.scheduleNotification(
                                getContext(), RetentionNotificationUtil.adrbrowsiel_STATS_ADS_TRACKERS);
                        OnboardingPrefManager.getInstance().setAdsTrackersNotificationStarted(true);
                    }

                    if (!OnboardingPrefManager.getInstance().isDataSavedNotificationStarted()
                            && dataSaved > MB_10 && PackageUtils.isFirstInstall(getContext())) {
                        RetentionNotificationUtil.scheduleNotification(
                                getContext(), RetentionNotificationUtil.adrbrowsiel_STATS_DATA);
                        OnboardingPrefManager.getInstance().setDataSavedNotificationStarted(true);
                    }

                    if (!OnboardingPrefManager.getInstance().isTimeSavedNotificationStarted()
                            && estimatedMillisecondsSaved > MINUTES_10
                            && PackageUtils.isFirstInstall(getContext())) {
                        RetentionNotificationUtil.scheduleNotification(
                                getContext(), RetentionNotificationUtil.adrbrowsiel_STATS_TIME);
                        OnboardingPrefManager.getInstance().setTimeSavedNotificationStarted(true);
                    }
                    if (madrbrowsielShieldsButton != null && madrbrowsielShieldsButton.isShown()
                            && madrbrowsielShieldsHandler != null && !madrbrowsielShieldsHandler.isShowing()) {
                        checkForTooltip(tab);
                    }
                }
            }

            @Override
            public void onDidFinishNavigation(Tab tab, NavigationHandle navigation) {
                if (getToolbarDataProvider().getTab() == tab && madrbrowsielRewardsNativeWorker != null
                        && !tab.isIncognito()) {
                    madrbrowsielRewardsNativeWorker.OnNotifyFrontTabUrlChanged(
                            tab.getId(), tab.getUrlString());
                }
                if (PackageUtils.isFirstInstall(getContext()) && tab.getUrlString() != null
                        && (tab.getUrlString().equals(adrbrowsielActivity.REWARDS_SETTINGS_URL)
                                || tab.getUrlString().equals(
                                        adrbrowsielActivity.adrbrowsiel_REWARDS_SETTINGS_URL))
                        && !adrbrowsielAdsNativeHelper.nativeIsadrbrowsielAdsEnabled(
                                Profile.getLastUsedRegularProfile())
                        && adrbrowsielRewardsHelper.shouldShowadrbrowsielRewardsOnboardingModal()
                        && ChromeFeatureList.isEnabled(adrbrowsielFeatureList.adrbrowsiel_REWARDS)) {
                    showadrbrowsielRewardsOnboardingModal();
                    adrbrowsielRewardsHelper.updateadrbrowsielRewardsAppOpenCount();
                    adrbrowsielRewardsHelper.setShowadrbrowsielRewardsOnboardingModal(false);
                }
            }

            @Override
            public void onDestroyed(Tab tab) {
                madrbrowsielShieldsHandler.removeStat(tab.getId());
            }
        };

        mTabModelSelectorTabModelObserver = new TabModelSelectorTabModelObserver(selector) {
            @Override
            public void didSelectTab(Tab tab, @TabSelectionType int type, int lastId) {
                if (getToolbarDataProvider().getTab() == tab && madrbrowsielRewardsNativeWorker != null
                        && !tab.isIncognito()) {
                    madrbrowsielRewardsNativeWorker.OnNotifyFrontTabUrlChanged(
                            tab.getId(), tab.getUrlString());
                }
            }
        };
    }

    private void checkForTooltip(Tab tab) {
        if (!adrbrowsielShieldsUtils.isTooltipShown) {
            if (!adrbrowsielShieldsUtils.hasShieldsTooltipShown(adrbrowsielShieldsUtils.PREF_SHIELDS_TOOLTIP)
                    && madrbrowsielShieldsHandler.getTrackersBlockedCount(tab.getId())
                                    + madrbrowsielShieldsHandler.getAdsBlockedCount(tab.getId())
                            > 0) {
                showTooltip(ShieldsTooltipEnum.ONE_TIME_ADS_TRACKER_BLOCKED_TOOLTIP,
                        adrbrowsielShieldsUtils.PREF_SHIELDS_TOOLTIP);
            } else if (!adrbrowsielShieldsUtils.hasShieldsTooltipShown(
                               adrbrowsielShieldsUtils.PREF_SHIELDS_VIDEO_ADS_BLOCKED_TOOLTIP)
                    && shouldShowVideoTooltip(tab.getUrlString())) {
                showTooltip(ShieldsTooltipEnum.VIDEO_ADS_BLOCKED_TOOLTIP,
                        adrbrowsielShieldsUtils.PREF_SHIELDS_VIDEO_ADS_BLOCKED_TOOLTIP);
            } else if (!adrbrowsielShieldsUtils.hasShieldsTooltipShown(
                               adrbrowsielShieldsUtils.PREF_SHIELDS_ADS_TRACKER_BLOCKED_TOOLTIP)
                    && madrbrowsielShieldsHandler.getTrackersBlockedCount(tab.getId())
                                    + madrbrowsielShieldsHandler.getAdsBlockedCount(tab.getId())
                            > 10) {
                showTooltip(ShieldsTooltipEnum.ADS_TRACKER_BLOCKED_TOOLTIP,
                        adrbrowsielShieldsUtils.PREF_SHIELDS_ADS_TRACKER_BLOCKED_TOOLTIP);
            } else if (!adrbrowsielShieldsUtils.hasShieldsTooltipShown(
                               adrbrowsielShieldsUtils.PREF_SHIELDS_HTTPS_UPGRADE_TOOLTIP)
                    && madrbrowsielShieldsHandler.getHttpsUpgradeCount(tab.getId()) > 0) {
                showTooltip(ShieldsTooltipEnum.HTTPS_UPGRADE_TOOLTIP,
                        adrbrowsielShieldsUtils.PREF_SHIELDS_HTTPS_UPGRADE_TOOLTIP);
            } else if (!adrbrowsielShieldsUtils.hasShieldsTooltipShown(
                               adrbrowsielShieldsUtils.PREF_SHIELDS_HTTPS_UPGRADE_TOOLTIP)
                    && madrbrowsielShieldsHandler.getHttpsUpgradeCount(tab.getId()) > 0) {
                showTooltip(ShieldsTooltipEnum.HTTPS_UPGRADE_TOOLTIP,
                        adrbrowsielShieldsUtils.PREF_SHIELDS_HTTPS_UPGRADE_TOOLTIP);
            } else {
                int trackersPlusAdsBlocked =
                        madrbrowsielShieldsHandler.getTrackersBlockedCount(tab.getId())
                        + madrbrowsielShieldsHandler.getAdsBlockedCount(tab.getId());
                chooseStatsShareTier(tab, trackersPlusAdsBlocked);
            }
        }
    }

    private void chooseStatsShareTier(Tab tab, int trackersPlusAdsBlocked) {
        String countryCode = Locale.getDefault().getCountry();
        // the tooltip for stats sharing is shown only for Japan
        if (!countryCode.equals(JAPAN_COUNTRY_CODE)) {
            return;
        }
        int totalBlocked = Integer.parseInt(adrbrowsielStatsUtil.getAdsTrackersBlocked().first);

        // show after adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_SHOW_DIFF (20) blocked stuff above the TIER
        // threshold
        if (!adrbrowsielShieldsUtils.hasShieldsTooltipShown(
                    adrbrowsielShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER1)
                && (totalBlocked >= adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_TIER1
                                        + adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_SHOW_DIFF
                        && totalBlocked < adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_TIER2)) {
            showTooltip(ShieldsTooltipEnum.adrbrowsiel_SHARE_STATS_TIER1_TOOLTIP,
                    adrbrowsielShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER1);
        } else if (!adrbrowsielShieldsUtils.hasShieldsTooltipShown(
                           adrbrowsielShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER2)
                && (totalBlocked >= adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_TIER2
                                        + adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_SHOW_DIFF
                        && totalBlocked < adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_TIER3)) {
            showTooltip(ShieldsTooltipEnum.adrbrowsiel_SHARE_STATS_TIER2_TOOLTIP,
                    adrbrowsielShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER2);
        } else if (!adrbrowsielShieldsUtils.hasShieldsTooltipShown(
                           adrbrowsielShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER3)
                && (totalBlocked >= adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_TIER3
                                        + adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_SHOW_DIFF
                        && totalBlocked < adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_TIER4)) {
            showTooltip(ShieldsTooltipEnum.adrbrowsiel_SHARE_STATS_TIER3_TOOLTIP,
                    adrbrowsielShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER3);
        } else if (!adrbrowsielShieldsUtils.hasShieldsTooltipShown(
                           adrbrowsielShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER4)
                && (totalBlocked >= adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_TIER4
                                        + adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_SHOW_DIFF
                        && totalBlocked < adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_TIER5)) {
            showTooltip(ShieldsTooltipEnum.adrbrowsiel_SHARE_STATS_TIER4_TOOLTIP,
                    adrbrowsielShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER4);
        } else if (!adrbrowsielShieldsUtils.hasShieldsTooltipShown(
                           adrbrowsielShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER5)
                && (totalBlocked >= adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_TIER5
                                        + adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_SHOW_DIFF
                        && totalBlocked < adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_TIER6)) {
            showTooltip(ShieldsTooltipEnum.adrbrowsiel_SHARE_STATS_TIER5_TOOLTIP,
                    adrbrowsielShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER5);
        } else if (!adrbrowsielShieldsUtils.hasShieldsTooltipShown(
                           adrbrowsielShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER6)
                && (totalBlocked >= adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_TIER6
                                        + adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_SHOW_DIFF
                        && totalBlocked < adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_TIER7)) {
            showTooltip(ShieldsTooltipEnum.adrbrowsiel_SHARE_STATS_TIER6_TOOLTIP,
                    adrbrowsielShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER6);
        } else if (!adrbrowsielShieldsUtils.hasShieldsTooltipShown(
                           adrbrowsielShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER7)
                && (totalBlocked >= adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_TIER7
                                        + adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_SHOW_DIFF
                        && totalBlocked < adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_TIER8)) {
            showTooltip(ShieldsTooltipEnum.adrbrowsiel_SHARE_STATS_TIER7_TOOLTIP,
                    adrbrowsielShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER7);
        } else if (!adrbrowsielShieldsUtils.hasShieldsTooltipShown(
                           adrbrowsielShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER8)
                && (totalBlocked >= adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_TIER8
                                        + adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_SHOW_DIFF
                        && totalBlocked < adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_TIER9)) {
            showTooltip(ShieldsTooltipEnum.adrbrowsiel_SHARE_STATS_TIER8_TOOLTIP,
                    adrbrowsielShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER8);
        } else if (!adrbrowsielShieldsUtils.hasShieldsTooltipShown(
                           adrbrowsielShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER9)
                && (totalBlocked >= adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_TIER9
                                + adrbrowsielShieldsUtils.adrbrowsiel_BLOCKED_SHOW_DIFF)) {
            showTooltip(ShieldsTooltipEnum.adrbrowsiel_SHARE_STATS_TIER9_TOOLTIP,
                    adrbrowsielShieldsUtils.PREF_SHARE_SHIELDS_TOOLTIP_TIER9);
        }
    }

    private boolean shouldShowVideoTooltip(String tabUrl) {
        try {
            URL url = new URL(tabUrl);
            for (String videoUrl : adrbrowsielShieldsUtils.videoSitesList) {
                if (url.getHost().contains(videoUrl)) {
                    return true;
                }
            }
            String countryCode = Locale.getDefault().getCountry();
            if (countryCode.equals(JAPAN_COUNTRY_CODE)) {
                for (String videoUrl : adrbrowsielShieldsUtils.videoSitesListJp) {
                    if (url.getHost().contains(videoUrl)) {
                        return true;
                    }
                }
            }
            return false;
        } catch (Exception ex) {
            // Do nothing if url is invalid.
            return false;
        }
    }

    private EnumSet<ShieldsTooltipEnum> getStatsSharingEnums() {
        return EnumSet.of(ShieldsTooltipEnum.adrbrowsiel_SHARE_STATS_TIER1_TOOLTIP,
                ShieldsTooltipEnum.adrbrowsiel_SHARE_STATS_TIER2_TOOLTIP,
                ShieldsTooltipEnum.adrbrowsiel_SHARE_STATS_TIER3_TOOLTIP,
                ShieldsTooltipEnum.adrbrowsiel_SHARE_STATS_TIER4_TOOLTIP,
                ShieldsTooltipEnum.adrbrowsiel_SHARE_STATS_TIER5_TOOLTIP,
                ShieldsTooltipEnum.adrbrowsiel_SHARE_STATS_TIER6_TOOLTIP,
                ShieldsTooltipEnum.adrbrowsiel_SHARE_STATS_TIER7_TOOLTIP,
                ShieldsTooltipEnum.adrbrowsiel_SHARE_STATS_TIER8_TOOLTIP,
                ShieldsTooltipEnum.adrbrowsiel_SHARE_STATS_TIER9_TOOLTIP);
    }

    private void showTooltip(ShieldsTooltipEnum shieldsTooltipEnum, String tooltipPref) {
        mShieldsPopupWindowTooltip = new PopupWindowTooltip.Builder(getContext())
                                             .anchorView(madrbrowsielShieldsButton)
                                             .arrowColor(getContext().getResources().getColor(
                                                     shieldsTooltipEnum.getArrowColor()))
                                             .gravity(Gravity.BOTTOM)
                                             .dismissOnOutsideTouch(true)
                                             .dismissOnInsideTouch(false)
                                             .modal(true)
                                             .contentView(R.layout.adrbrowsiel_shields_tooltip_layout)
                                             .build();
        mShieldsPopupWindowTooltip.findViewById(R.id.shields_tooltip_layout)
                .setBackgroundDrawable(ContextCompat.getDrawable(
                        getContext(), shieldsTooltipEnum.getTooltipBackground()));

        if (shieldsTooltipEnum == ShieldsTooltipEnum.ONE_TIME_ADS_TRACKER_BLOCKED_TOOLTIP) {
            Button btnTooltip = mShieldsPopupWindowTooltip.findViewById(R.id.btn_tooltip);
            btnTooltip.setVisibility(View.VISIBLE);
            btnTooltip.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    dismissShieldsTooltip();
                    showShieldsMenu(madrbrowsielShieldsButton);
                }
            });
        } else if (getStatsSharingEnums().contains(shieldsTooltipEnum)) {
            Button btnTooltip = mShieldsPopupWindowTooltip.findViewById(R.id.btn_tooltip);

            SpannableStringBuilder shareStringBuilder = new SpannableStringBuilder();
            shareStringBuilder
                    .append(getContext().getResources().getString(
                            R.string.adrbrowsiel_stats_share_button))
                    .append("  ");
            shareStringBuilder.setSpan(new ImageSpan(getContext(), R.drawable.ic_share_white),
                    shareStringBuilder.length() - 1, shareStringBuilder.length(), 0);
            btnTooltip.setText(shareStringBuilder, TextView.BufferType.SPANNABLE);

            btnTooltip.setVisibility(View.VISIBLE);

            btnTooltip.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    dismissShieldsTooltip();
                    if (adrbrowsielStatsUtil.hasWritePermission(adrbrowsielActivity.getadrbrowsielActivity())) {
                        adrbrowsielStatsUtil.shareStats(R.layout.adrbrowsiel_stats_share_layout);
                    }
                }
            });
        }

        TextView tooltipTitle = mShieldsPopupWindowTooltip.findViewById(R.id.txt_tooltip_title);
        SpannableStringBuilder ssb =
                new SpannableStringBuilder(new StringBuilder("\t\t")
                                                   .append(getContext().getResources().getString(
                                                           shieldsTooltipEnum.getTitle()))
                                                   .toString());
        ssb.setSpan(new ImageSpan(getContext(), R.drawable.ic_shield_done_filled_20dp), 0, 1,
                Spannable.SPAN_INCLUSIVE_EXCLUSIVE);
        tooltipTitle.setText(ssb, TextView.BufferType.SPANNABLE);

        TextView tooltipText = mShieldsPopupWindowTooltip.findViewById(R.id.txt_tooltip_text);
        tooltipText.setText(getContext().getResources().getString(shieldsTooltipEnum.getText()));

        if (madrbrowsielShieldsButton != null && madrbrowsielShieldsButton.isShown()) {
            mShieldsPopupWindowTooltip.show();
            adrbrowsielShieldsUtils.setShieldsTooltipShown(tooltipPref, true);
            adrbrowsielShieldsUtils.isTooltipShown = true;
        }
    }

    public void dismissShieldsTooltip() {
        if (mShieldsPopupWindowTooltip != null && mShieldsPopupWindowTooltip.isShowing()) {
            mShieldsPopupWindowTooltip.dismiss();
            mShieldsPopupWindowTooltip = null;
        }
    }

    public void reopenShieldsPanel() {
        if (madrbrowsielShieldsHandler != null && madrbrowsielShieldsHandler.isShowing()) {
            madrbrowsielShieldsHandler.hideadrbrowsielShieldsMenu();
            showShieldsMenu(madrbrowsielShieldsButton);
        }
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        dismissShieldsTooltip();
        reopenShieldsPanel();
    }

    private void showadrbrowsielRewardsOnboardingModal() {
        Context context = getContext();
        final Dialog dialog = new Dialog(context);
        dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
        dialog.setCancelable(false);
        dialog.setContentView(R.layout.adrbrowsiel_rewards_onboarding_modal);
        dialog.getWindow().setBackgroundDrawableResource(android.R.color.transparent);

        View adrbrowsielRewardsOnboardingModalView =
                dialog.findViewById(R.id.adrbrowsiel_rewards_onboarding_modal_layout);
        adrbrowsielRewardsOnboardingModalView.setBackgroundColor(
                context.getResources().getColor(android.R.color.white));
        adrbrowsielRewardsOnboardingModalView.setVisibility(View.VISIBLE);

        String tosText =
                String.format(context.getResources().getString(R.string.adrbrowsiel_rewards_tos_text),
                        context.getResources().getString(R.string.terms_of_service),
                        context.getResources().getString(R.string.privacy_policy));
        int termsOfServiceIndex =
                tosText.indexOf(context.getResources().getString(R.string.terms_of_service));
        Spanned tosTextSpanned = adrbrowsielRewardsHelper.spannedFromHtmlString(tosText);
        SpannableString tosTextSS = new SpannableString(tosTextSpanned.toString());

        ClickableSpan tosClickableSpan = new ClickableSpan() {
            @Override
            public void onClick(@NonNull View textView) {
                CustomTabActivity.showInfoPage(context, adrbrowsielActivity.adrbrowsiel_TERMS_PAGE);
            }
            @Override
            public void updateDrawState(@NonNull TextPaint ds) {
                super.updateDrawState(ds);
                ds.setUnderlineText(false);
            }
        };

        tosTextSS.setSpan(tosClickableSpan, termsOfServiceIndex,
                termsOfServiceIndex
                        + context.getResources().getString(R.string.terms_of_service).length(),
                Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
        tosTextSS.setSpan(new ForegroundColorSpan(context.getResources().getColor(
                                  R.color.adrbrowsiel_rewards_modal_theme_color)),
                termsOfServiceIndex,
                termsOfServiceIndex
                        + context.getResources().getString(R.string.terms_of_service).length(),
                Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);

        ClickableSpan privacyProtectionClickableSpan = new ClickableSpan() {
            @Override
            public void onClick(@NonNull View textView) {
                CustomTabActivity.showInfoPage(context, adrbrowsielActivity.adrbrowsiel_PRIVACY_POLICY);
            }
            @Override
            public void updateDrawState(@NonNull TextPaint ds) {
                super.updateDrawState(ds);
                ds.setUnderlineText(false);
            }
        };

        int privacyPolicyIndex =
                tosText.indexOf(context.getResources().getString(R.string.privacy_policy));
        tosTextSS.setSpan(privacyProtectionClickableSpan, privacyPolicyIndex,
                privacyPolicyIndex
                        + context.getResources().getString(R.string.privacy_policy).length(),
                Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
        tosTextSS.setSpan(new ForegroundColorSpan(context.getResources().getColor(
                                  R.color.adrbrowsiel_rewards_modal_theme_color)),
                privacyPolicyIndex,
                privacyPolicyIndex
                        + context.getResources().getString(R.string.privacy_policy).length(),
                Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);

        TextView tosAndPpText = adrbrowsielRewardsOnboardingModalView.findViewById(
                R.id.adrbrowsiel_rewards_onboarding_modal_tos_pp_text);
        tosAndPpText.setMovementMethod(LinkMovementMethod.getInstance());
        tosAndPpText.setText(tosTextSS);

        TextView takeQuickTourButton =
                adrbrowsielRewardsOnboardingModalView.findViewById(R.id.take_quick_tour_button);
        takeQuickTourButton.setOnClickListener((new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                adrbrowsielRewardsHelper.setShowadrbrowsielRewardsOnboardingOnce(true);
                openRewardsPanel();
                dialog.dismiss();
            }
        }));
        Button btnadrbrowsielRewards =
                adrbrowsielRewardsOnboardingModalView.findViewById(R.id.btn_adrbrowsiel_rewards);
        btnadrbrowsielRewards.setOnClickListener((new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                adrbrowsielAdsNativeHelper.nativeSetAdsEnabled(Profile.getLastUsedRegularProfile());
                adrbrowsielRewardsNativeWorker.getInstance().SetAutoContributeEnabled(true);
                dialog.dismiss();
            }
        }));
        AppCompatImageView modalCloseButton = adrbrowsielRewardsOnboardingModalView.findViewById(
                R.id.adrbrowsiel_rewards_onboarding_modal_close);
        modalCloseButton.setOnClickListener((new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                dialog.dismiss();
            }
        }));

        dialog.show();
    }

    private void addSavedBandwidthToDb(long savings) {
        new AsyncTask<Void>() {
            @Override
            protected Void doInBackground() {
                try {
                    SavedBandwidthTable savedBandwidthTable = new SavedBandwidthTable(
                            savings, adrbrowsielStatsUtil.getCalculatedDate("yyyy-MM-dd", 0));
                    long rowId = mDatabaseHelper.insertSavedBandwidth(savedBandwidthTable);
                } catch (Exception e) {
                    // Do nothing if url is invalid.
                    // Just return w/o showing shields popup.
                    return null;
                }
                return null;
            }
            @Override
            protected void onPostExecute(Void result) {
                assert ThreadUtils.runningOnUiThread();
                if (isCancelled()) return;
            }
        }.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    private void addStatsToDb(String statType, String statSite, String url) {
        new AsyncTask<Void>() {
            @Override
            protected Void doInBackground() {
                try {
                    URL urlObject = new URL(url);
                    URL siteObject = new URL(statSite);
                    adrbrowsielStatsTable adrbrowsielStatsTable = new adrbrowsielStatsTable(url, urlObject.getHost(),
                            statType, statSite, siteObject.getHost(),
                            adrbrowsielStatsUtil.getCalculatedDate("yyyy-MM-dd", 0));
                    long rowId = mDatabaseHelper.insertStats(adrbrowsielStatsTable);
                } catch (Exception e) {
                    // Do nothing if url is invalid.
                    // Just return w/o showing shields popup.
                    return null;
                }
                return null;
            }
            @Override
            protected void onPostExecute(Void result) {
                assert ThreadUtils.runningOnUiThread();
                if (isCancelled()) return;
            }
        }.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    public void hideRewardsOnboardingIcon() {
        if (madrbrowsielRewardsOnboardingIcon != null) {
            madrbrowsielRewardsOnboardingIcon.setVisibility(View.GONE);
        }
        if (madrbrowsielRewardsNotificationsCount != null) {
            madrbrowsielRewardsNotificationsCount.setVisibility(View.GONE);
        }
        SharedPreferences sharedPref = ContextUtils.getAppSharedPreferences();
        SharedPreferences.Editor editor = sharedPref.edit();
        editor.putBoolean(adrbrowsielRewardsPanelPopup.PREF_WAS_TOOLBAR_BAT_LOGO_BUTTON_PRESSED, true);
        editor.apply();
    }

    @Override
    public void onClick(View v) {
        onClickImpl(v);
    }

    public void onClickImpl(View v) {
        if (madrbrowsielShieldsHandler == null) {
            assert false;
            return;
        }
        if (madrbrowsielShieldsButton == v && madrbrowsielShieldsButton != null) {
            showShieldsMenu(madrbrowsielShieldsButton);
        } else if (madrbrowsielRewardsButton == v && madrbrowsielRewardsButton != null) {
            // Context context = getContext();
            // if (checkForRewardsOnboarding()) {
            //   OnboardingPrefManager.getInstance().showOnboarding(context);
            //   hideRewardsOnboardingIcon();
            // } else {
            //   if (null != mRewardsPopup) {
            //     return;
            //   }
            //   mRewardsPopup = new adrbrowsielRewardsPanelPopup(v);
            //   mRewardsPopup.showLikePopDownMenu();
            // }
            if (null != mRewardsPopup) {
                return;
            }
            hideRewardsOnboardingIcon();
            OnboardingPrefManager.getInstance().setOnboardingShown(true);
            mRewardsPopup = new adrbrowsielRewardsPanelPopup(v);
            mRewardsPopup.showLikePopDownMenu();
            if (madrbrowsielRewardsNotificationsCount.isShown()) {
                SharedPreferences sharedPref = ContextUtils.getAppSharedPreferences();
                SharedPreferences.Editor editor = sharedPref.edit();
                editor.putBoolean(
                        adrbrowsielRewardsPanelPopup.PREF_WAS_TOOLBAR_BAT_LOGO_BUTTON_PRESSED, true);
                editor.apply();
                madrbrowsielRewardsNotificationsCount.setVisibility(View.INVISIBLE);
                mIsInitialNotificationPosted = false;
            }
        }
    }

    private boolean checkForRewardsOnboarding() {
        return PackageUtils.isFirstInstall(getContext())
                && !adrbrowsielAdsNativeHelper.nativeIsadrbrowsielAdsEnabled(
                        Profile.getLastUsedRegularProfile())
                && ChromeFeatureList.isEnabled(adrbrowsielFeatureList.adrbrowsiel_REWARDS)
                && !OnboardingPrefManager.getInstance().isOnboardingShown();
    }

    private void showShieldsMenu(View madrbrowsielShieldsButton) {
        Tab currentTab = getToolbarDataProvider().getTab();
        if (currentTab == null) {
            return;
        }
        try {
            URL url = new URL(currentTab.getUrlString());
            // Don't show shields popup if protocol is not valid for shields.
            if (!isValidProtocolForShields(url.getProtocol())) {
                return;
            }
            madrbrowsielShieldsHandler.show(madrbrowsielShieldsButton, currentTab);
        } catch (Exception e) {
            // Do nothing if url is invalid.
            // Just return w/o showing shields popup.
            return;
        }
    }

    @Override
    public boolean onLongClick(View v) {
        // Use null as the default description since Toast.showAnchoredToast
        // will return false if it is null.
        String description = null;
        Context context = getContext();
        Resources resources = context.getResources();

        if (v == madrbrowsielShieldsButton) {
            description = resources.getString(R.string.accessibility_toolbar_btn_adrbrowsiel_shields);
        } else if (v == madrbrowsielRewardsButton) {
            description = resources.getString(R.string.accessibility_toolbar_btn_adrbrowsiel_rewards);
        } else if (v == mHomeButton) {
            description = resources.getString(R.string.accessibility_toolbar_btn_home);
        }

        return Toast.showAnchoredToast(context, v, description);
    }

    @Override
    public void onUrlFocusChange(boolean hasFocus) {
        Context context = getContext();
        if (hasFocus && PackageUtils.isFirstInstall(context)
                && adrbrowsielActivity.getadrbrowsielActivity() != null
                && adrbrowsielActivity.getadrbrowsielActivity().getActivityTab() != null
                && UrlUtilities.isNTPUrl(
                        adrbrowsielActivity.getadrbrowsielActivity().getActivityTab().getUrlString())
                && !OnboardingPrefManager.getInstance().hasSearchEngineOnboardingShown()
                && !adrbrowsielSearchEngineUtils.getDSEShortName(true).equals(
                        OnboardingPrefManager.DUCKDUCKGO)) {
            Intent searchActivityIntent = new Intent(context, SearchActivity.class);
            context.startActivity(searchActivityIntent);
        }
        super.onUrlFocusChange(hasFocus);
    }

    public void populateUrlAnimatorSet(boolean showExpandedState,
            int urlFocusToolbarButtonsDuration, int urlClearFocusTabStackDelayMs,
            List<Animator> animators) {
        if (madrbrowsielShieldsButton != null) {
            Animator animator;
            if (showExpandedState) {
                float density = getContext().getResources().getDisplayMetrics().density;
                boolean isRtl = getLayoutDirection() == LAYOUT_DIRECTION_RTL;
                float toolbarButtonTranslationX =
                        MathUtils.flipSignIf(URL_FOCUS_TOOLBAR_BUTTONS_TRANSLATION_X_DP, isRtl)
                        * density;
                animator = ObjectAnimator.ofFloat(
                        madrbrowsielShieldsButton, TRANSLATION_X, toolbarButtonTranslationX);
                animator.setDuration(urlFocusToolbarButtonsDuration);
                animator.setInterpolator(BakedBezierInterpolator.FADE_OUT_CURVE);
                animators.add(animator);

                animator = ObjectAnimator.ofFloat(madrbrowsielShieldsButton, ALPHA, 0);
                animator.setDuration(urlFocusToolbarButtonsDuration);
                animator.setInterpolator(BakedBezierInterpolator.FADE_OUT_CURVE);
                animators.add(animator);
            } else {
                animator = ObjectAnimator.ofFloat(madrbrowsielShieldsButton, TRANSLATION_X, 0);
                animator.setDuration(urlFocusToolbarButtonsDuration);
                animator.setStartDelay(urlClearFocusTabStackDelayMs);
                animator.setInterpolator(BakedBezierInterpolator.TRANSFORM_CURVE);
                animators.add(animator);

                animator = ObjectAnimator.ofFloat(madrbrowsielShieldsButton, ALPHA, 1);
                animator.setDuration(urlFocusToolbarButtonsDuration);
                animator.setStartDelay(urlClearFocusTabStackDelayMs);
                animator.setInterpolator(BakedBezierInterpolator.TRANSFORM_CURVE);
                animators.add(animator);
            }
        }
    }

    private void updateModernLocationBarColor(int color) {
        updateModernLocationBarColorImpl(color);
    }

    public void updateModernLocationBarColorImpl(int color) {
        if (mShieldsLayout != null && mShieldsLayoutIsColorBackground) {
            mShieldsLayout.setBackgroundColor(
                    ChromeColors.getDefaultThemeColor(getContext().getResources(), isIncognito()));
        }
        mCurrentToolbarColor = color;
        if (mShieldsLayout != null) {
            mShieldsLayout.getBackground().setColorFilter(color, PorterDuff.Mode.SRC_IN);
        }
        if (mRewardsLayout != null) {
            mRewardsLayout.getBackground().setColorFilter(color, PorterDuff.Mode.SRC_IN);
        }
    }

    public int getBoundsAfterAccountingForRightButtons(ViewGroup toolbarButtonsContainer) {
        if (toolbarButtonsContainer == null || mShieldsLayout == null) {
            assert false;
            return 0;
        }
        ViewGroup.MarginLayoutParams params =
                (ViewGroup.MarginLayoutParams) toolbarButtonsContainer.getLayoutParams();

        int rewardsLen = (mRewardsLayout == null || mRewardsLayout.getVisibility() == View.GONE)
                ? 0
                : mRewardsLayout.getWidth();
        return toolbarButtonsContainer.getMeasuredWidth() - mShieldsLayout.getWidth() - rewardsLen
                + params.getMarginEnd();
    }

    /**
     * If |tab| is null, set disabled image to shields button and |urlString| is
     * ignored.
     * If |urlString| is null, url is fetched from |tab|.
     */
    private void updateadrbrowsielShieldsButtonState(Tab tab) {
        if (madrbrowsielShieldsButton == null) {
            assert false;
            return;
        }

        if (tab == null) {
            madrbrowsielShieldsButton.setImageResource(R.drawable.btn_adrbrowsiel_off);
            return;
        }
        madrbrowsielShieldsButton.setImageResource(
                isShieldsOnForTab(tab) ? R.drawable.btn_adrbrowsiel : R.drawable.btn_adrbrowsiel_off);

        SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();

        if (mRewardsLayout == null) return;
        if (isIncognito()) {
            mRewardsLayout.setVisibility(View.GONE);
            updateShieldsLayoutBackground(true);
        } else if (isNativeLibraryReady()
                && ChromeFeatureList.isEnabled(adrbrowsielFeatureList.adrbrowsiel_REWARDS)
                && !adrbrowsielPrefServiceBridge.getInstance().getSafetynetCheckFailed()
                && !sharedPreferences.getBoolean(
                        AppearancePreferences.PREF_HIDE_adrbrowsiel_REWARDS_ICON, false)) {
            mRewardsLayout.setVisibility(View.VISIBLE);
            updateShieldsLayoutBackground(false);
        }
    }

    private boolean isShieldsOnForTab(Tab tab) {
        if (tab == null) {
            assert false;
            return false;
        }
        return adrbrowsielShieldsContentSettings.getShields(
                Profile.fromWebContents(((TabImpl) tab).getWebContents()), tab.getUrlString(),
                adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_adrbrowsiel_SHIELDS);
    }

    private boolean isValidProtocolForShields(String protocol) {
        if (protocol.equals("http") || protocol.equals("https")) {
            return true;
        }

        return false;
    }

    public void dismissRewardsPanel() {
        if (mRewardsPopup != null) {
            mRewardsPopup.dismiss();
            mRewardsPopup = null;
        }
    }

    public void onRewardsPanelDismiss() {
        mRewardsPopup = null;
    }

    public void openRewardsPanel() {
        onClick(madrbrowsielRewardsButton);
    }

    public boolean isShieldsTooltipShown() {
        if (mShieldsPopupWindowTooltip != null) {
            return mShieldsPopupWindowTooltip.isShowing();
        }
        return false;
    }

    @Override
    public void OnNotificationAdded(String id, int type, long timestamp, String[] args) {
        if (madrbrowsielRewardsNativeWorker == null) {
            return;
        }

        if (type == adrbrowsielRewardsNativeWorker.REWARDS_NOTIFICATION_BACKUP_WALLET) {
            madrbrowsielRewardsNativeWorker.DeleteNotification(id);
        } else if (type == adrbrowsielRewardsNativeWorker.REWARDS_NOTIFICATION_GRANT) {
            // Set flag
            SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
            SharedPreferences.Editor sharedPreferencesEditor = sharedPreferences.edit();
            sharedPreferencesEditor.putBoolean(
                    adrbrowsielRewardsPanelPopup.PREF_GRANTS_NOTIFICATION_RECEIVED, true);
            sharedPreferencesEditor.apply();
        }
        madrbrowsielRewardsNativeWorker.GetAllNotifications();
    }

    private boolean mayShowadrbrowsielAdsOnboardingDialog() {
        Context context = getContext();

        if (adrbrowsielAdsSignupDialog.shouldShowNewUserDialog(context)) {
            adrbrowsielAdsSignupDialog.showNewUserDialog(getContext());
            return true;
        } else if (adrbrowsielAdsSignupDialog.shouldShowNewUserDialogIfRewardsIsSwitchedOff(context)) {
            adrbrowsielAdsSignupDialog.showNewUserDialog(getContext());
            return true;
        } else if (adrbrowsielAdsSignupDialog.shouldShowExistingUserDialog(context)) {
            adrbrowsielAdsSignupDialog.showExistingUserDialog(getContext());
            return true;
        }

        return false;
    }

    @Override
    public void OnNotificationsCount(int count) {
        if (madrbrowsielRewardsNotificationsCount != null) {
            if (count != 0) {
                String value = Integer.toString(count);
                if (count > 99) {
                    madrbrowsielRewardsNotificationsCount.setBackground(
                            ResourcesCompat.getDrawable(getContext().getResources(),
                                    R.drawable.adrbrowsiel_rewards_rectangle, /* theme= */ null));
                    value = "99+";
                } else {
                    madrbrowsielRewardsNotificationsCount.setBackground(
                            ResourcesCompat.getDrawable(getContext().getResources(),
                                    R.drawable.adrbrowsiel_rewards_circle, /* theme= */ null));
                }
                madrbrowsielRewardsNotificationsCount.setText(value);
                madrbrowsielRewardsNotificationsCount.setVisibility(View.VISIBLE);
                mIsNotificationPosted = true;
            } else {
                madrbrowsielRewardsNotificationsCount.setText("");
                madrbrowsielRewardsNotificationsCount.setBackgroundResource(0);
                madrbrowsielRewardsNotificationsCount.setVisibility(View.INVISIBLE);
                mIsNotificationPosted = false;
                updateVerifiedPublisherMark();
            }
        }

        updateNotificationBadgeForNewInstall();
        if (!PackageUtils.isFirstInstall(getContext())
                && !OnboardingPrefManager.getInstance().isAdsAvailable()) {
            mayShowadrbrowsielAdsOnboardingDialog();
        }

        if (checkForRewardsOnboarding()) {
            if (madrbrowsielRewardsOnboardingIcon != null) {
                madrbrowsielRewardsOnboardingIcon.setVisibility(View.VISIBLE);
            }
            if (madrbrowsielRewardsNotificationsCount != null) {
                madrbrowsielRewardsNotificationsCount.setVisibility(View.GONE);
            }
        }
    }

    private void updateNotificationBadgeForNewInstall() {
        SharedPreferences sharedPref = ContextUtils.getAppSharedPreferences();
        boolean shownBefore = sharedPref.getBoolean(
                adrbrowsielRewardsPanelPopup.PREF_WAS_TOOLBAR_BAT_LOGO_BUTTON_PRESSED, false);
        boolean shouldShow = madrbrowsielRewardsNotificationsCount != null && !shownBefore;
        mIsInitialNotificationPosted = shouldShow; // initial notification

        if (!shouldShow) return;

        madrbrowsielRewardsNotificationsCount.setText("");
        madrbrowsielRewardsNotificationsCount.setBackground(ResourcesCompat.getDrawable(
                getContext().getResources(), R.drawable.adrbrowsiel_rewards_circle, /* theme= */ null));
        madrbrowsielRewardsNotificationsCount.setVisibility(View.VISIBLE);
    }

    @Override
    public void onThemeColorChanged(int color, boolean shouldAnimate) {
        final int textBoxColor = ThemeUtils.getTextBoxColorForToolbarBackgroundInNonNativePage(
                getContext().getResources(), color, isIncognito());
        updateModernLocationBarColor(textBoxColor);
    }

    /**
     * adrbrowsielRewardsNativeWorker.PublisherObserver:
     *   Update a 'verified publisher' checkmark on url bar BAT icon only if
     *   no notifications are posted.
     */
    @Override
    public void onFrontTabPublisherChanged(boolean verified) {
        mIsPublisherVerified = verified;
        updateVerifiedPublisherMark();
    }

    private void updateVerifiedPublisherMark() {
        if (madrbrowsielRewardsNotificationsCount == null) {
            // Most likely we are on a custom page
            return;
        }
        if (mIsInitialNotificationPosted) {
            return;
        } else if (!mIsNotificationPosted) {
            if (mIsPublisherVerified) {
                madrbrowsielRewardsNotificationsCount.setVisibility(View.VISIBLE);
                madrbrowsielRewardsNotificationsCount.setBackground(ResourcesCompat.getDrawable(
                        getContext().getResources(), R.drawable.bat_verified, /* theme= */ null));
            } else {
                madrbrowsielRewardsNotificationsCount.setBackgroundResource(0);
                madrbrowsielRewardsNotificationsCount.setVisibility(View.INVISIBLE);
            }
        }
    }

    public void onBottomToolbarVisibilityChanged(boolean isVisible) {
        mIsBottomToolbarVisible = isVisible;
        if (adrbrowsielReflectionUtil.EqualTypes(this.getClass(), ToolbarPhone.class)
                && getMenuButtonCoordinator() != null) {
            getMenuButtonCoordinator().setVisibility(!isVisible);
            ToggleTabStackButton toggleTabStackButton = findViewById(R.id.tab_switcher_button);
            if (toggleTabStackButton != null) {
                toggleTabStackButton.setVisibility(isTabSwitcherOnBottom() ? GONE : VISIBLE);
            }
        }
    }

    private void updateShieldsLayoutBackground(boolean rounded) {
        if (!(this instanceof ToolbarTablet) || (mShieldsLayout == null)) return;

        if (rounded) {
            mShieldsLayout.setBackgroundDrawable(
                    ApiCompatibilityUtils.getDrawable(getContext().getResources(),
                            R.drawable.modern_toolbar_background_grey_end_segment));
            mShieldsLayoutIsColorBackground = false;
        } else {
            mShieldsLayout.setBackgroundColor(
                    ChromeColors.getDefaultThemeColor(getContext().getResources(), isIncognito()));
            mShieldsLayoutIsColorBackground = true;
        }
        updateModernLocationBarColor(mCurrentToolbarColor);
    }

    private boolean isTabSwitcherOnBottom() {
        return mIsBottomToolbarVisible && BottomToolbarVariationManager.isTabSwitcherOnBottom();
    }

    private boolean isMenuButtonOnBottom() {
        return mIsBottomToolbarVisible && BottomToolbarVariationManager.isMenuButtonOnBottom();
    }

    @Override
    protected void initialize(ToolbarDataProvider toolbarDataProvider,
            ToolbarTabController tabController, MenuButtonCoordinator menuButtonCoordinator,
            BooleanSupplier isInVrSupplier) {
        super.initialize(toolbarDataProvider, tabController, menuButtonCoordinator, isInVrSupplier);
        adrbrowsielMenuButtonCoordinator.setMenuFromBottom(isMenuButtonOnBottom());
    }

    public void updateMenuButtonState() {
        adrbrowsielMenuButtonCoordinator.setMenuFromBottom(mIsBottomToolbarVisible);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        if (adrbrowsielReflectionUtil.EqualTypes(this.getClass(), CustomTabToolbar.class)
                || adrbrowsielReflectionUtil.EqualTypes(this.getClass(), ToolbarPhone.class)) {
            updateMenuButtonState();
            Tab tab = getToolbarDataProvider() != null ? getToolbarDataProvider().getTab() : null;
            if (tab != null && ((TabImpl) tab).getWebContents() != null) {
                updateadrbrowsielShieldsButtonState(tab);
            }
        }
        super.onDraw(canvas);
    }
}
