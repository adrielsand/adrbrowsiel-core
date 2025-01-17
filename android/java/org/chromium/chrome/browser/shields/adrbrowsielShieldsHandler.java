/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.shields;

import android.Manifest;
import android.animation.Animator;
import android.animation.Animator.AnimatorListener;
import android.animation.AnimatorListenerAdapter;
import android.animation.AnimatorSet;
import android.app.Activity;
import android.content.Context;
import android.content.ContextWrapper;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Point;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Build;
import android.provider.MediaStore;
import android.text.SpannableString;
import android.text.Spanned;
import android.text.method.LinkMovementMethod;
import android.text.style.ClickableSpan;
import android.text.style.ImageSpan;
import android.text.style.StyleSpan;
import android.util.TypedValue;
import android.view.ContextThemeWrapper;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.TranslateAnimation;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListPopupWindow;
import android.widget.PopupMenu;
import android.widget.PopupWindow;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.Switch;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;
import androidx.core.widget.TextViewCompat;

import org.chromium.base.Log;
import org.chromium.base.SysUtils;
import org.chromium.base.task.AsyncTask;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.adrbrowsielRewardsHelper;
import org.chromium.chrome.browser.adrbrowsielRewardsNativeWorker;
import org.chromium.chrome.browser.app.adrbrowsielActivity;
import org.chromium.chrome.browser.adrbrowsiel_stats.adrbrowsielStatsUtil;
import org.chromium.chrome.browser.night_mode.GlobalNightModeStateProviderHolder;
import org.chromium.chrome.browser.onboarding.OnboardingPrefManager;
import org.chromium.chrome.browser.preferences.website.adrbrowsielShieldsContentSettings;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.shields.adrbrowsielShieldsMenuObserver;
import org.chromium.chrome.browser.shields.adrbrowsielShieldsUtils;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.util.ConfigurationUtils;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

/**
 * Object responsible for handling the creation, showing, hiding of the adrbrowsielShields menu.
 */
public class adrbrowsielShieldsHandler implements adrbrowsielRewardsHelper.LargeIconReadyCallback {

    private static class BlockersInfo {
        public BlockersInfo() {
            mAdsBlocked = 0;
            mTrackersBlocked = 0;
            mHTTPSUpgrades = 0;
            mScriptsBlocked = 0;
            mFingerprintsBlocked = 0;
        }

        public int mAdsBlocked;
        public int mTrackersBlocked;
        public int mHTTPSUpgrades;
        public int mScriptsBlocked;
        public int mFingerprintsBlocked;
    }

    private final Context mContext;
    private PopupWindow mPopupWindow;
    private AnimatorSet mMenuItemEnterAnimator;
    private adrbrowsielShieldsMenuObserver mMenuObserver;
    private View mHardwareButtonMenuAnchor;
    private final Map<Integer, BlockersInfo> mTabsStat =
        Collections.synchronizedMap(new HashMap<Integer, BlockersInfo>());

    private Switch madrbrowsielShieldsBlockTrackersSwitch;
    private OnCheckedChangeListener madrbrowsielShieldsAdsTrackingChangeListener;
    private Switch madrbrowsielShieldsHTTPSEverywhereSwitch;
    private OnCheckedChangeListener madrbrowsielShieldsHTTPSEverywhereChangeListener;
    private Switch madrbrowsielShieldsBlockingScriptsSwitch;
    private OnCheckedChangeListener madrbrowsielShieldsBlockingScriptsChangeListener;

    private View mPopupView;
    private LinearLayout mMainLayout;
    private LinearLayout mSecondaryLayout;
    private LinearLayout mAboutLayout;
    private LinearLayout mToggleLayout;
    private LinearLayout mThankYouLayout;
    private LinearLayout mPrivacyReportLayout;
    private LinearLayout mReportBrokenSiteLayout;
    private TextView mSiteBlockCounterText;
    private TextView mShieldsDownText;
    private TextView mSiteBrokenWarningText;
    private View mBottomDivider;
    private ImageView mToggleIcon;

    private adrbrowsielRewardsNativeWorker madrbrowsielRewardsNativeWorker;
    private adrbrowsielRewardsHelper mIconFetcher;

    private String mHost;
    private String mTitle;
    private int mTabId;
    private Profile mProfile;

    private static Context scanForActivity(Context cont) {
        if (cont == null)
            return null;
        else if (cont instanceof Activity)
            return cont;
        else if (cont instanceof ContextWrapper)
            return scanForActivity(((ContextWrapper)cont).getBaseContext());

        return cont;
    }

    /**
     * Constructs a adrbrowsielShieldsHandler object.
     * @param context Context that is using the adrbrowsielShieldsMenu.
     */
    public adrbrowsielShieldsHandler(Context context) {
        mContext = scanForActivity(context);
        mHardwareButtonMenuAnchor = null;
        if (mContext != null) {
            mHardwareButtonMenuAnchor = ((Activity)mContext).findViewById(R.id.menu_anchor_stub);
        }
    }

    public void addStat(int tabId, String block_type, String subresource) {
        if (!mTabsStat.containsKey(tabId)) {
            mTabsStat.put(tabId, new BlockersInfo());
        }
        BlockersInfo blockersInfo = mTabsStat.get(tabId);
        if (block_type.equals(adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_ADS)) {
            blockersInfo.mAdsBlocked++;
        } else if (block_type.equals(adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_TRACKERS)) {
            blockersInfo.mTrackersBlocked++;
        } else if (block_type.equals(adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_HTTP_UPGRADABLE_RESOURCES)) {
            blockersInfo.mHTTPSUpgrades++;
        } else if (block_type.equals(adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_JAVASCRIPTS)) {
            blockersInfo.mScriptsBlocked++;
        } else if (block_type.equals(adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING)) {
            blockersInfo.mFingerprintsBlocked++;
        }
    }

    public void removeStat(int tabId) {
        if (!mTabsStat.containsKey(tabId)) {
            return;
        }
        mTabsStat.remove(tabId);
    }

    public void clearadrbrowsielShieldsCount(int tabId) {
        mTabsStat.put(tabId, new BlockersInfo());
    }

    public void addObserver(adrbrowsielShieldsMenuObserver menuObserver) {
        mMenuObserver = menuObserver;
    }

    public void show(View anchorView, Tab tab) {
        if (mHardwareButtonMenuAnchor == null) return;

        mHost = tab.getUrlString();
        mTitle = tab.getUrl().getHost();
        mTabId = tab.getId();
        mProfile = Profile.fromWebContents(tab.getWebContents());

        madrbrowsielRewardsNativeWorker = adrbrowsielRewardsNativeWorker.getInstance();
        mIconFetcher = new adrbrowsielRewardsHelper(tab);
        mPopupWindow = showPopupMenu(anchorView);

        updateValues(mTabId);
    }

    public PopupWindow showPopupMenu(View anchorView) {
        int rotation = ((Activity)mContext).getWindowManager().getDefaultDisplay().getRotation();
        // This fixes the bug where the bottom of the menu starts at the top of
        // the keyboard, instead of overlapping the keyboard as it should.
        int displayHeight = mContext.getResources().getDisplayMetrics().heightPixels;
        int widthHeight = mContext.getResources().getDisplayMetrics().widthPixels;
        int currentDisplayWidth = widthHeight;

        // In appcompat 23.2.1, DisplayMetrics are not updated after rotation change. This is a
        // workaround for it. See crbug.com/599048.
        // TODO(ianwen): Remove the rotation check after we roll to 23.3.0.
        if (rotation == Surface.ROTATION_0 || rotation == Surface.ROTATION_180) {
            currentDisplayWidth = Math.min(displayHeight, widthHeight);
            displayHeight = Math.max(displayHeight, widthHeight);
        } else if (rotation == Surface.ROTATION_90 || rotation == Surface.ROTATION_270) {
            currentDisplayWidth = Math.max(displayHeight, widthHeight);
            displayHeight = Math.min(displayHeight, widthHeight);
        } else {
            assert false : "Rotation unexpected";
        }
        if (anchorView == null) {
            Rect rect = new Rect();
            ((Activity)mContext).getWindow().getDecorView().getWindowVisibleDisplayFrame(rect);
            int statusBarHeight = rect.top;
            mHardwareButtonMenuAnchor.setY((displayHeight - statusBarHeight));

            anchorView = mHardwareButtonMenuAnchor;
        }

        ContextThemeWrapper wrapper = new ContextThemeWrapper(mContext, R.style.OverflowMenuThemeOverlay);
        Point pt = new Point();
        ((Activity)mContext).getWindowManager().getDefaultDisplay().getSize(pt);
        // Get the height and width of the display.
        Rect appRect = new Rect();
        ((Activity)mContext).getWindow().getDecorView().getWindowVisibleDisplayFrame(appRect);

        // Use full size of window for abnormal appRect.
        if (appRect.left < 0 && appRect.top < 0) {
            appRect.left = 0;
            appRect.top = 0;
            appRect.right = ((Activity)mContext).getWindow().getDecorView().getWidth();
            appRect.bottom = ((Activity)mContext).getWindow().getDecorView().getHeight();
        }

        LayoutInflater inflater = (LayoutInflater) anchorView.getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        mPopupView = inflater.inflate(R.layout.adrbrowsiel_shields_main_layout, null);
        setUpViews();

        //Specify the length and width through constants
        int width;
        if (ConfigurationUtils.isLandscape(mContext)) {
            width = (int) ((mContext.getResources().getDisplayMetrics().widthPixels) * 0.50);
        } else {
            width = (int) ((mContext.getResources().getDisplayMetrics().widthPixels) * 0.75);
        }
        int height = LinearLayout.LayoutParams.WRAP_CONTENT;

        //Make Inactive Items Outside Of PopupWindow
        boolean focusable = true;

        //Create a window with our parameters
        PopupWindow popupWindow = new PopupWindow(mPopupView, width, height, focusable);
        popupWindow.setBackgroundDrawable(new ColorDrawable(Color.WHITE));
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            popupWindow.setElevation(20);
        }
        // mPopup.setBackgroundDrawable(mContext.getResources().getDrawable(android.R.drawable.picture_frame));
        //Set the location of the window on the screen
        popupWindow.showAsDropDown(anchorView, 0, 0);
        popupWindow.setInputMethodMode(PopupWindow.INPUT_METHOD_NOT_NEEDED);
        popupWindow.setAnimationStyle(R.style.OverflowMenuAnim);

        // Turn off window animations for low end devices, and on Android M, which has built-in menu
        // animations.
        if (SysUtils.isLowEndDevice() || Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            popupWindow.setAnimationStyle(0);
        }

        Rect bgPadding = new Rect();
        int popupWidth = wrapper.getResources().getDimensionPixelSize(R.dimen.menu_width)
                         + bgPadding.left + bgPadding.right;
        popupWindow.setWidth(popupWidth);

        return popupWindow;
    }

    public void updateHost(String host) {
        mHost = host;
    }

    public void updateValues(int tabId) {
        if (!mTabsStat.containsKey(tabId)) {
            return;
        }
        BlockersInfo blockersInfo = mTabsStat.get(tabId);
        updateValues(blockersInfo.mAdsBlocked + blockersInfo.mTrackersBlocked,
                     blockersInfo.mHTTPSUpgrades, blockersInfo.mScriptsBlocked, blockersInfo.mFingerprintsBlocked);
    }

    public int getAdsBlockedCount(int tabId) {
        if (!mTabsStat.containsKey(tabId)) {
            return 0;
        }

        BlockersInfo blockersInfo = mTabsStat.get(tabId);
        return blockersInfo.mAdsBlocked;
    }

    public int getTrackersBlockedCount(int tabId) {
        if (!mTabsStat.containsKey(tabId)) {
            return 0;
        }

        BlockersInfo blockersInfo = mTabsStat.get(tabId);
        return blockersInfo.mTrackersBlocked;
    }

    public int getHttpsUpgradeCount(int tabId) {
        if (!mTabsStat.containsKey(tabId)) {
            return 0;
        }

        BlockersInfo blockersInfo = mTabsStat.get(tabId);
        return blockersInfo.mHTTPSUpgrades;
    }

    public void updateValues(int adsAndTrackers, int httpsUpgrades, int scriptsBlocked, int fingerprintsBlocked) {
        if (mContext == null) {
            return;
        }
        final int fadsAndTrackers = adsAndTrackers;
        final int fhttpsUpgrades = httpsUpgrades;
        final int fscriptsBlocked = scriptsBlocked;
        final int ffingerprintsBlocked = fingerprintsBlocked;
        ((Activity)mContext).runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (!isShowing()) {
                    return;
                }
                try {
                    mSiteBlockCounterText.setText(String.valueOf(fadsAndTrackers
                                                  + fhttpsUpgrades
                                                  + fscriptsBlocked
                                                  + ffingerprintsBlocked));
                } catch (NullPointerException exc) {
                    // It means that the adrbrowsielry Panel was destroyed during the update, we just do nothing
                }
            }
        });
    }

    public boolean isShowing() {
        if (null == mPopupWindow) {
            return false;
        }

        return mPopupWindow.isShowing();
    }

    public void hideadrbrowsielShieldsMenu() {
        if (isShowing()) {
            mPopupWindow.dismiss();
        }
    }

    private void initViews() {
        mMainLayout = mPopupView.findViewById(R.id.main_layout);
        mSecondaryLayout = mPopupView.findViewById(R.id.adrbrowsiel_shields_secondary_layout_id);
        mAboutLayout = mPopupView.findViewById(R.id.adrbrowsiel_shields_about_layout_id);
        mToggleLayout = mPopupView.findViewById(R.id.adrbrowsiel_shields_toggle_layout_id);
        mSiteBlockCounterText = mPopupView.findViewById(R.id.site_block_count_text);
        mShieldsDownText = mPopupView.findViewById(R.id.shield_down_text);
        mSiteBrokenWarningText = mPopupView.findViewById(R.id.site_broken_warning_text);

        mReportBrokenSiteLayout = mPopupView.findViewById(R.id.adrbrowsiel_shields_report_site_layout_id);
        mThankYouLayout = mPopupView.findViewById(R.id.adrbrowsiel_shields_thank_you_layout_id);
        mPrivacyReportLayout = mPopupView.findViewById(R.id.adrbrowsiel_shields_privacy_report_layout_id);

        mBottomDivider = mToggleLayout.findViewById(R.id.bottom_divider);
        mToggleIcon = mToggleLayout.findViewById(R.id.toggle_favicon);
    }

    private void setUpMainLayout() {
        String favIconURL = madrbrowsielRewardsNativeWorker.GetPublisherFavIconURL(mTabId);
        Tab currentActiveTab = mIconFetcher.getTab();
        String url = currentActiveTab.getUrlString();
        final String favicon_url = (favIconURL.isEmpty()) ? url : favIconURL;
        mIconFetcher.retrieveLargeIcon(favicon_url, this);

        TextView mSiteText = mMainLayout.findViewById(R.id.site_text);
        mSiteText.setText(mTitle.replaceFirst("^(http[s]?://www\\.|http[s]?://|www\\.)", ""));

        Switch mShieldMainSwitch = mMainLayout.findViewById(R.id.site_switch);

        ImageView helpImage = (ImageView) mMainLayout.findViewById(R.id.help);
        ImageView shareImage = (ImageView) mMainLayout.findViewById(R.id.share);

        helpImage.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mMainLayout.setVisibility(View.GONE);
                mAboutLayout.setVisibility(View.VISIBLE);
                setUpAboutLayout();
            }
        });

        shareImage.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mMainLayout.setVisibility(View.GONE);
                if (adrbrowsielStatsUtil.hasWritePermission(adrbrowsielActivity.getadrbrowsielActivity())) {
                    adrbrowsielStatsUtil.shareStats(R.layout.adrbrowsiel_stats_share_layout);
                }
            }
        });

        mToggleIcon.setColorFilter(mContext.getResources().getColor(R.color.shield_toggle_button_tint));
        mToggleLayout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                setToggleView(!mSecondaryLayout.isShown());
            }
        });

        ImageView mPrivacyReportIcon = mPrivacyReportLayout.findViewById(R.id.toggle_favicon);
        mPrivacyReportIcon.setImageResource(R.drawable.ic_arrow_forward);
        mPrivacyReportIcon.setColorFilter(
                mContext.getResources().getColor(R.color.default_icon_color_tint_list));
        TextView mViewPrivacyReportText = mPrivacyReportLayout.findViewById(R.id.toggle_text);
        mViewPrivacyReportText.setText(R.string.view_full_privacy_report);
        mPrivacyReportLayout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                adrbrowsielStatsUtil.showadrbrowsielStats();
                hideadrbrowsielShieldsMenu();
            }
        });
        if (OnboardingPrefManager.getInstance().isadrbrowsielStatsEnabled()) {
            mPrivacyReportLayout.setVisibility(View.VISIBLE);
        } else {
            mPrivacyReportLayout.setVisibility(View.GONE);
        }

        setUpSecondaryLayout();

        setupMainSwitchClick(mShieldMainSwitch);
    }

    private void shareStats() {
        View shareStatsLayout = adrbrowsielStatsUtil.getLayout(R.layout.adrbrowsiel_stats_share_layout);
        adrbrowsielStatsUtil.updateadrbrowsielShareStatsLayoutAndShare(shareStatsLayout);
    }

    private void setToggleView(boolean shouldShow) {
        if (shouldShow) {
            mSecondaryLayout.setVisibility(View.VISIBLE);
            mBottomDivider.setVisibility(View.VISIBLE);
            mToggleIcon.setImageResource(R.drawable.ic_toggle_up);
        } else {
            mSecondaryLayout.setVisibility(View.GONE);
            mBottomDivider.setVisibility(View.GONE);
            mToggleIcon.setImageResource(R.drawable.ic_toggle_down);
        }
    }

    private void setUpSecondaryLayout() {
        TextView shieldsText = mSecondaryLayout.findViewById(R.id.adrbrowsiel_shields_text);
        shieldsText.setText(mTitle.replaceFirst("^(http[s]?://www\\.|http[s]?://|www\\.)", ""));

        setUpSwitchLayouts();
        setUpCookiesLayout();
        setUpFingerprintingLayout();
    }

    private void setUpSwitchLayouts() {
        LinearLayout mBlockTrackersLayout = mSecondaryLayout.findViewById(R.id.adrbrowsiel_shields_block_trackers_id);
        TextView mBlockTrackersText = mBlockTrackersLayout.findViewById(R.id.adrbrowsiel_shields_switch_text);
        madrbrowsielShieldsBlockTrackersSwitch = mBlockTrackersLayout.findViewById(R.id.adrbrowsiel_shields_switch);
        mBlockTrackersText.setText(R.string.adrbrowsiel_shields_ads_and_trackers);
        setupAdsTrackingSwitchClick(madrbrowsielShieldsBlockTrackersSwitch);

        LinearLayout mUpgradeHttpsLayout = mSecondaryLayout.findViewById(R.id.adrbrowsiel_shields_upgrade_https_id);
        TextView mUpgradeHttpsText = mUpgradeHttpsLayout.findViewById(R.id.adrbrowsiel_shields_switch_text);
        madrbrowsielShieldsHTTPSEverywhereSwitch = mUpgradeHttpsLayout.findViewById(R.id.adrbrowsiel_shields_switch);
        mUpgradeHttpsText.setText(R.string.adrbrowsiel_shields_https_everywhere_switch);
        setupHTTPSEverywhereSwitchClick(madrbrowsielShieldsHTTPSEverywhereSwitch);

        LinearLayout mBlockScriptsLayout = mSecondaryLayout.findViewById(R.id.adrbrowsiel_shields_block_scripts_id);
        TextView mBlockScriptsText = mBlockScriptsLayout.findViewById(R.id.adrbrowsiel_shields_switch_text);
        madrbrowsielShieldsBlockingScriptsSwitch = mBlockScriptsLayout.findViewById(R.id.adrbrowsiel_shields_switch);
        mBlockScriptsText.setText(R.string.adrbrowsiel_shields_blocks_scripts_switch);
        setupBlockingScriptsSwitchClick(madrbrowsielShieldsBlockingScriptsSwitch);
    }

    private void setUpCookiesLayout() {
        LinearLayout mBlockCookiesLayout = mPopupView.findViewById(R.id.adrbrowsiel_shields_block_cookies_layout_id);
        TextView mCookiesOptionTitle = mBlockCookiesLayout.findViewById(R.id.option_title);
        mCookiesOptionTitle.setText(R.string.block_cookies);
        TextView mCookiesOptionText = mBlockCookiesLayout.findViewById(R.id.option_text);
        mCookiesOptionText.setText(R.string.block_cookies_text);
        RadioButton mCookiesOption1 = mBlockCookiesLayout.findViewById(R.id.option1);
        mCookiesOption1.setText(R.string.block_cookies_option_1);
        RadioButton mCookiesOption2 = mBlockCookiesLayout.findViewById(R.id.option2);
        mCookiesOption2.setText(R.string.block_cross_site_cookies);
        RadioButton mCookiesOption3 = mBlockCookiesLayout.findViewById(R.id.option3);
        mCookiesOption3.setText(R.string.block_cookies_option_3);
        Button mCookiesDoneButton = mBlockCookiesLayout.findViewById(R.id.done_button);
        mCookiesDoneButton.setOnClickListener(mDoneClickListener);
        ImageView mCookiesBackButton = mBlockCookiesLayout.findViewById(R.id.back_button);
        mCookiesBackButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mBlockCookiesLayout.setVisibility(View.GONE);
                mMainLayout.setVisibility(View.VISIBLE);
            }
        });

        LinearLayout mCookiesLayout = mSecondaryLayout.findViewById(R.id.adrbrowsiel_shields_cookies_layout_id);
        mCookiesLayout.setBackground(null);
        mCookiesLayout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mMainLayout.setVisibility(View.GONE);
                mBlockCookiesLayout.setVisibility(View.VISIBLE);
            }
        });
        ImageView mCookiesIcon = mCookiesLayout.findViewById(R.id.toggle_favicon);
        mCookiesIcon.setImageResource(R.drawable.ic_chevron_right);
        mCookiesIcon.setColorFilter(mContext.getResources().getColor(R.color.default_icon_color_tint_list));
        TextView mCookiesText = mCookiesLayout.findViewById(R.id.toggle_text);
        mCookiesText.setText(R.string.block_cookies);

        String settingOption = adrbrowsielShieldsContentSettings.getShieldsValue(mProfile, mHost, adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_COOKIES);
        if (settingOption.equals(adrbrowsielShieldsContentSettings.BLOCK_RESOURCE)) {
            mCookiesOption1.setChecked(true);
        } else if (settingOption.equals(adrbrowsielShieldsContentSettings.BLOCK_THIRDPARTY_RESOURCE)) {
            mCookiesOption2.setChecked(true);
        } else if (settingOption.equals(adrbrowsielShieldsContentSettings.ALLOW_RESOURCE)) {
            mCookiesOption3.setChecked(true);
        }

        RadioGroup mCookiesOptionGroup = mBlockCookiesLayout.findViewById(R.id.options_radio_group);
        mCookiesOptionGroup.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                RadioButton checkedRadioButton = (RadioButton)group.findViewById(checkedId);
                boolean isChecked = checkedRadioButton.isChecked();

                if (isChecked) {
                    if (checkedId == R.id.option1) {
                        adrbrowsielShieldsContentSettings.setShieldsValue(mProfile, mHost, adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_COOKIES, adrbrowsielShieldsContentSettings.BLOCK_RESOURCE, false);
                    } else if (checkedId == R.id.option2) {
                        adrbrowsielShieldsContentSettings.setShieldsValue(mProfile, mHost, adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_COOKIES, adrbrowsielShieldsContentSettings.BLOCK_THIRDPARTY_RESOURCE, false);
                    } else if (checkedId == R.id.option3) {
                        adrbrowsielShieldsContentSettings.setShieldsValue(mProfile, mHost, adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_COOKIES, adrbrowsielShieldsContentSettings.ALLOW_RESOURCE, false);
                    }
                    if (null != mMenuObserver) {
                        mMenuObserver.onMenuTopShieldsChanged(isChecked, false);
                    }
                }
            }
        });
    }

    private void setUpFingerprintingLayout() {
        LinearLayout mBlockFingerPrintingLayout = mPopupView.findViewById(R.id.adrbrowsiel_shields_block_fingerprinting_layout_id);
        TextView mFingerprintingOptionTitle = mBlockFingerPrintingLayout.findViewById(R.id.option_title);
        mFingerprintingOptionTitle.setText(R.string.block_fingerprinting);
        TextView mFingerprintingOptionText = mBlockFingerPrintingLayout.findViewById(R.id.option_text);
        mFingerprintingOptionText.setText(R.string.block_fingerprinting_text);
        RadioButton mFingerprintingOption1 = mBlockFingerPrintingLayout.findViewById(R.id.option1);
        mFingerprintingOption1.setText(R.string.block_fingerprinting_option_1);
        RadioButton mFingerprintingOption2 = mBlockFingerPrintingLayout.findViewById(R.id.option2);
        mFingerprintingOption2.setText(R.string.block_fingerprinting_option_2);
        RadioButton mFingerprintingOption3 = mBlockFingerPrintingLayout.findViewById(R.id.option3);
        mFingerprintingOption3.setText(R.string.block_fingerprinting_option_3);
        Button mFingerprintingDoneButton = mBlockFingerPrintingLayout.findViewById(R.id.done_button);
        mFingerprintingDoneButton.setOnClickListener(mDoneClickListener);
        ImageView mFingerprintingBackButton = mBlockFingerPrintingLayout.findViewById(R.id.back_button);
        mFingerprintingBackButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mBlockFingerPrintingLayout.setVisibility(View.GONE);
                mMainLayout.setVisibility(View.VISIBLE);
            }
        });

        LinearLayout mFingerPrintingLayout = mSecondaryLayout.findViewById(R.id.adrbrowsiel_shields_fingerprinting_layout_id);
        mFingerPrintingLayout.setBackground(null);
        mFingerPrintingLayout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mMainLayout.setVisibility(View.GONE);
                mBlockFingerPrintingLayout.setVisibility(View.VISIBLE);
            }
        });
        ImageView mFingerPrintingIcon = mFingerPrintingLayout.findViewById(R.id.toggle_favicon);
        mFingerPrintingIcon.setImageResource(R.drawable.ic_chevron_right);
        mFingerPrintingIcon.setColorFilter(mContext.getResources().getColor(R.color.default_icon_color_tint_list));
        TextView mFingerPrintingText = mFingerPrintingLayout.findViewById(R.id.toggle_text);
        mFingerPrintingText.setText(R.string.block_fingerprinting);

        String settingOption = adrbrowsielShieldsContentSettings.getShieldsValue(mProfile, mHost, adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING);
        if (settingOption.equals(adrbrowsielShieldsContentSettings.BLOCK_RESOURCE)) {
            mFingerprintingOption1.setChecked(true);
        } else if (settingOption.equals(adrbrowsielShieldsContentSettings.DEFAULT)) {
            mFingerprintingOption2.setChecked(true);
        } else if (settingOption.equals(adrbrowsielShieldsContentSettings.ALLOW_RESOURCE)) {
            mFingerprintingOption3.setChecked(true);
        }

        RadioGroup mFingerprintingOptionGroup = mBlockFingerPrintingLayout.findViewById(R.id.options_radio_group);
        mFingerprintingOptionGroup.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                RadioButton checkedRadioButton = (RadioButton)group.findViewById(checkedId);
                boolean isChecked = checkedRadioButton.isChecked();

                if (isChecked) {
                    if (checkedId == R.id.option1) {
                        adrbrowsielShieldsContentSettings.setShieldsValue(mProfile, mHost, adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING, adrbrowsielShieldsContentSettings.BLOCK_RESOURCE, false);
                    } else if (checkedId == R.id.option2) {
                        adrbrowsielShieldsContentSettings.setShieldsValue(mProfile, mHost, adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING, adrbrowsielShieldsContentSettings.DEFAULT, false);
                    } else if (checkedId == R.id.option3) {
                        adrbrowsielShieldsContentSettings.setShieldsValue(mProfile, mHost, adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING, adrbrowsielShieldsContentSettings.ALLOW_RESOURCE, false);
                    }
                    if (null != mMenuObserver) {
                        mMenuObserver.onMenuTopShieldsChanged(isChecked, false);
                    }
                }
            }
        });
    }

    private void setUpAboutLayout() {
        TextView mAboutText = mAboutLayout.findViewById(R.id.about_text);
        mAboutText.setVisibility(View.VISIBLE);
        TextView mOptionTitle = mAboutLayout.findViewById(R.id.option_title);
        mOptionTitle.setText(R.string.about_adrbrowsiel_shields_text);
        TextView mOptionText = mAboutLayout.findViewById(R.id.option_text);
        mOptionText.setVisibility(View.GONE);
        RadioGroup mOptionGroup = mAboutLayout.findViewById(R.id.options_radio_group);
        mOptionGroup.setVisibility(View.GONE);
        Button mDoneButton = mAboutLayout.findViewById(R.id.done_button);
        mDoneButton.setOnClickListener(mDoneClickListener);
        ImageView mBackButton = mAboutLayout.findViewById(R.id.back_button);
        mBackButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mAboutLayout.setVisibility(View.GONE);
                mMainLayout.setVisibility(View.VISIBLE);
            }
        });
    }

    private void setUpReportBrokenSiteLayout() {
        TextView mReportSiteUrlText = mReportBrokenSiteLayout.findViewById(R.id.report_site_url);
        mReportSiteUrlText.setText(mTitle);

        Button mCancelButton = mReportBrokenSiteLayout.findViewById(R.id.btn_cancel);
        mCancelButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                hideadrbrowsielShieldsMenu();
            }
        });

        Button mSubmitButton = mReportBrokenSiteLayout.findViewById(R.id.btn_submit);
        mSubmitButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                adrbrowsielShieldsUtils.adrbrowsielShieldsWorkerTask mWorkerTask = new adrbrowsielShieldsUtils.adrbrowsielShieldsWorkerTask(mTitle);
                mWorkerTask.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
                mReportBrokenSiteLayout.setVisibility(View.GONE);
                mThankYouLayout.setVisibility(View.VISIBLE);
            }
        });
    }

    private void setUpMainSwitchLayout(boolean isChecked) {
        TextView mShieldDownText = mMainLayout.findViewById(R.id.shield_down_text);
        Button mReportBrokenSiteButton = mMainLayout.findViewById(R.id.btn_report_broken_site);
        mReportBrokenSiteButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mMainLayout.setVisibility(View.GONE);
                mReportBrokenSiteLayout.setVisibility(View.VISIBLE);
                setUpReportBrokenSiteLayout();
            }
        });

        LinearLayout mSiteBlockLayout = mMainLayout.findViewById(R.id.site_block_layout);
        TextView mSiteBrokenWarningText = mMainLayout.findViewById(R.id.site_broken_warning_text);

        TextView mShieldsUpText = mMainLayout.findViewById(R.id.shield_up_text);
        String madrbrowsielShieldsText = mContext.getResources().getString(R.string.adrbrowsiel_shields_onboarding_title);

        if (isChecked) {
            mShieldDownText.setVisibility(View.GONE);
            mReportBrokenSiteButton.setVisibility(View.GONE);

            mSiteBlockLayout.setVisibility(View.VISIBLE);
            mSiteBrokenWarningText.setVisibility(View.VISIBLE);
            mToggleLayout.setVisibility(View.VISIBLE);

            String mUpText = mContext.getResources().getString(R.string.up);
            SpannableString mSpanString = new SpannableString(madrbrowsielShieldsText + " " + mUpText);
            mSpanString.setSpan(new StyleSpan(Typeface.BOLD), mSpanString.length() - mUpText.length(), mSpanString.length(), 0);
            mShieldsUpText.setText(mSpanString);
        } else {
            mShieldDownText.setVisibility(View.VISIBLE);
            mReportBrokenSiteButton.setVisibility(View.VISIBLE);

            mSiteBlockLayout.setVisibility(View.GONE);
            mSiteBrokenWarningText.setVisibility(View.GONE);
            mToggleLayout.setVisibility(View.GONE);
            setToggleView(false);

            String mDownText = mContext.getResources().getString(R.string.down);
            SpannableString mSpanString = new SpannableString(madrbrowsielShieldsText + " " + mDownText);
            mSpanString.setSpan(new StyleSpan(Typeface.BOLD), mSpanString.length() - mDownText.length(), mSpanString.length(), 0);
            mShieldsUpText.setText(mSpanString);
        }
    }

    private void setUpViews() {
        boolean isNightMode = GlobalNightModeStateProviderHolder.getInstance().isInNightMode();

        initViews();

        setUpMainLayout();
    }

    private void setupAdsTrackingSwitchClick(Switch adrbrowsielShieldsAdsTrackingSwitch) {
        if (null == adrbrowsielShieldsAdsTrackingSwitch) {
            return;
        }
        setupAdsTrackingSwitch(adrbrowsielShieldsAdsTrackingSwitch, false);

        madrbrowsielShieldsAdsTrackingChangeListener = new OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView,
                                         boolean isChecked) {
                if (0 != mHost.length()) {
                    adrbrowsielShieldsContentSettings.setShields(mProfile, mHost, adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_ADS_TRACKERS, isChecked, false);
                    if (null != mMenuObserver) {
                        mMenuObserver.onMenuTopShieldsChanged(isChecked, false);
                    }
                }
            }
        };

        adrbrowsielShieldsAdsTrackingSwitch.setOnCheckedChangeListener(madrbrowsielShieldsAdsTrackingChangeListener);
    }

    private void setupAdsTrackingSwitch(Switch adrbrowsielShieldsAdsTrackingSwitch, boolean fromTopSwitch) {
        if (null == adrbrowsielShieldsAdsTrackingSwitch) {
            return;
        }
        if (fromTopSwitch) {
            // Prevents to fire an event when top shields changed
            adrbrowsielShieldsAdsTrackingSwitch.setOnCheckedChangeListener(null);
        }
        if (0 != mHost.length()) {
            if (adrbrowsielShieldsContentSettings.getShields(mProfile, mHost, adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_adrbrowsiel_SHIELDS)) {
                if (adrbrowsielShieldsContentSettings.getShields(mProfile, mHost, adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_ADS_TRACKERS)) {
                    adrbrowsielShieldsAdsTrackingSwitch.setChecked(true);
                } else {
                    adrbrowsielShieldsAdsTrackingSwitch.setChecked(false);
                }
                adrbrowsielShieldsAdsTrackingSwitch.setEnabled(true);
            } else {
                adrbrowsielShieldsAdsTrackingSwitch.setChecked(false);
                adrbrowsielShieldsAdsTrackingSwitch.setEnabled(false);
            }
        }
        if (fromTopSwitch) {
            adrbrowsielShieldsAdsTrackingSwitch.setOnCheckedChangeListener(madrbrowsielShieldsAdsTrackingChangeListener);
        }
    }

    private void setupHTTPSEverywhereSwitchClick(Switch adrbrowsielShieldsHTTPSEverywhereSwitch) {
        if (null == adrbrowsielShieldsHTTPSEverywhereSwitch) {
            return;
        }
        setupHTTPSEverywhereSwitch(adrbrowsielShieldsHTTPSEverywhereSwitch, false);

        madrbrowsielShieldsHTTPSEverywhereChangeListener = new OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView,
                                         boolean isChecked) {
                if (0 != mHost.length()) {
                    adrbrowsielShieldsContentSettings.setShields(mProfile, mHost, adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_HTTP_UPGRADABLE_RESOURCES, isChecked, false);
                    if (null != mMenuObserver) {
                        mMenuObserver.onMenuTopShieldsChanged(isChecked, false);
                    }
                }
            }
        };

        adrbrowsielShieldsHTTPSEverywhereSwitch.setOnCheckedChangeListener(madrbrowsielShieldsHTTPSEverywhereChangeListener);
    }

    private void setupHTTPSEverywhereSwitch(Switch adrbrowsielShieldsHTTPSEverywhereSwitch, boolean fromTopSwitch) {
        if (null == adrbrowsielShieldsHTTPSEverywhereSwitch) {
            return;
        }
        if (fromTopSwitch) {
            // Prevents to fire an event when top shields changed
            adrbrowsielShieldsHTTPSEverywhereSwitch.setOnCheckedChangeListener(null);
        }
        if (0 != mHost.length()) {
            if (adrbrowsielShieldsContentSettings.getShields(mProfile, mHost, adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_adrbrowsiel_SHIELDS)) {
                if (adrbrowsielShieldsContentSettings.getShields(mProfile, mHost, adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_HTTP_UPGRADABLE_RESOURCES)) {
                    adrbrowsielShieldsHTTPSEverywhereSwitch.setChecked(true);
                } else {
                    adrbrowsielShieldsHTTPSEverywhereSwitch.setChecked(false);
                }
                adrbrowsielShieldsHTTPSEverywhereSwitch.setEnabled(true);
            } else {
                adrbrowsielShieldsHTTPSEverywhereSwitch.setChecked(false);
                adrbrowsielShieldsHTTPSEverywhereSwitch.setEnabled(false);
            }
        }
        if (fromTopSwitch) {
            adrbrowsielShieldsHTTPSEverywhereSwitch.setOnCheckedChangeListener(madrbrowsielShieldsHTTPSEverywhereChangeListener);
        }
    }

    private void setupBlockingScriptsSwitchClick(Switch adrbrowsielShieldsBlockingScriptsSwitch) {
        if (null == adrbrowsielShieldsBlockingScriptsSwitch) {
            return;
        }
        setupBlockingScriptsSwitch(adrbrowsielShieldsBlockingScriptsSwitch, false);

        madrbrowsielShieldsBlockingScriptsChangeListener = new OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView,
                                         boolean isChecked) {
                if (0 != mHost.length()) {
                    adrbrowsielShieldsContentSettings.setShields(mProfile, mHost, adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_JAVASCRIPTS, isChecked, false);
                    if (null != mMenuObserver) {
                        mMenuObserver.onMenuTopShieldsChanged(isChecked, false);
                    }
                }
            }
        };

        adrbrowsielShieldsBlockingScriptsSwitch.setOnCheckedChangeListener(madrbrowsielShieldsBlockingScriptsChangeListener);
    }

    private void setupBlockingScriptsSwitch(Switch adrbrowsielShieldsBlockingScriptsSwitch, boolean fromTopSwitch) {
        if (null == adrbrowsielShieldsBlockingScriptsSwitch) {
            return;
        }
        if (fromTopSwitch) {
            // Prevents to fire an event when top shields changed
            adrbrowsielShieldsBlockingScriptsSwitch.setOnCheckedChangeListener(null);
        }
        if (0 != mHost.length()) {
            if (adrbrowsielShieldsContentSettings.getShields(mProfile, mHost, adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_adrbrowsiel_SHIELDS)) {
                if (adrbrowsielShieldsContentSettings.getShields(mProfile, mHost, adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_JAVASCRIPTS)) {
                    adrbrowsielShieldsBlockingScriptsSwitch.setChecked(true);
                } else {
                    adrbrowsielShieldsBlockingScriptsSwitch.setChecked(false);
                }
                adrbrowsielShieldsBlockingScriptsSwitch.setEnabled(true);
            } else {
                adrbrowsielShieldsBlockingScriptsSwitch.setChecked(false);
                adrbrowsielShieldsBlockingScriptsSwitch.setEnabled(false);
            }
        }
        if (fromTopSwitch) {
            adrbrowsielShieldsBlockingScriptsSwitch.setOnCheckedChangeListener(madrbrowsielShieldsBlockingScriptsChangeListener);
        }
    }

    private void setupMainSwitchClick(Switch adrbrowsielShieldsSwitch) {
        if (null == adrbrowsielShieldsSwitch) {
            return;
        }
        if (0 != mHost.length()) {
            if (adrbrowsielShieldsContentSettings.getShields(mProfile, mHost, adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_adrbrowsiel_SHIELDS)) {
                adrbrowsielShieldsSwitch.setChecked(true);
                setUpMainSwitchLayout(true);
            } else {
                adrbrowsielShieldsSwitch.setChecked(false);
                setUpMainSwitchLayout(false);
            }
        }
        adrbrowsielShieldsSwitch.setOnCheckedChangeListener(new OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView,
                                         boolean isChecked) {
                if (0 != mHost.length()) {
                    adrbrowsielShieldsContentSettings.setShields(mProfile, mHost, adrbrowsielShieldsContentSettings.RESOURCE_IDENTIFIER_adrbrowsiel_SHIELDS, isChecked, false);
                    setupAdsTrackingSwitch(madrbrowsielShieldsBlockTrackersSwitch, true);
                    setupHTTPSEverywhereSwitch(madrbrowsielShieldsHTTPSEverywhereSwitch, true);
                    setupBlockingScriptsSwitch(madrbrowsielShieldsBlockingScriptsSwitch, true);
                    if (null != mMenuObserver) {
                        mMenuObserver.onMenuTopShieldsChanged(isChecked, true);
                    }
                }

                setUpMainSwitchLayout(isChecked);
            }
        });
    }

    @Override
    public void onLargeIconReady(Bitmap icon) {
        SetFavIcon(icon);
    }


    private void SetFavIcon(Bitmap bmp) {
        if (bmp != null) {
            ((Activity)mContext).runOnUiThread(
            new Runnable() {
                @Override
                public void run() {
                    ImageView iv = (ImageView) mPopupView.findViewById(R.id.site_favicon);
                    if (iv != null) iv.setImageBitmap(adrbrowsielRewardsHelper.getCircularBitmap(bmp));
                }
            });
        }
    }

    private View.OnClickListener mDoneClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            hideadrbrowsielShieldsMenu();
        }
    };
}
