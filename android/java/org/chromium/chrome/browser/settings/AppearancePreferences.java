/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import static org.chromium.chrome.browser.settings.MainSettings.PREF_UI_THEME;

import android.content.SharedPreferences;
import android.os.Bundle;

import androidx.preference.Preference;

import org.chromium.base.ContextUtils;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.adrbrowsielFeatureList;
import org.chromium.chrome.browser.adrbrowsielRelaunchUtils;
import org.chromium.chrome.browser.adrbrowsielRewardsNativeWorker;
import org.chromium.chrome.browser.adrbrowsielRewardsObserver;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.chrome.browser.night_mode.NightModeUtils;
import org.chromium.chrome.browser.preferences.adrbrowsielPreferenceKeys;
import org.chromium.chrome.browser.preferences.SharedPreferencesManager;
import org.chromium.chrome.browser.settings.adrbrowsielPreferenceFragment;
import org.chromium.chrome.browser.toolbar.bottom.BottomToolbarConfiguration;
import org.chromium.components.browser_ui.settings.ChromeSwitchPreference;
import org.chromium.components.browser_ui.settings.SettingsUtils;
import org.chromium.ui.base.DeviceFormFactor;

public class AppearancePreferences extends adrbrowsielPreferenceFragment
        implements Preference.OnPreferenceChangeListener, adrbrowsielRewardsObserver {
    public static final String PREF_HIDE_adrbrowsiel_REWARDS_ICON = "hide_adrbrowsiel_rewards_icon";
    public static final String PREF_adrbrowsiel_NIGHT_MODE_ENABLED = "adrbrowsiel_night_mode_enabled_key";
    public static final String PREF_adrbrowsiel_ENABLE_TAB_GROUPS = "adrbrowsiel_enable_tab_groups";

    private adrbrowsielRewardsNativeWorker madrbrowsielRewardsNativeWorker;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getActivity().setTitle(R.string.prefs_appearance);
        SettingsUtils.addPreferencesFromResource(this, R.xml.appearance_preferences);
        boolean isTablet = DeviceFormFactor.isNonMultiDisplayContextOnTablet(
                ContextUtils.getApplicationContext());
        if (isTablet) {
            removePreferenceIfPresent(adrbrowsielPreferenceKeys.adrbrowsiel_BOTTOM_TOOLBAR_ENABLED_KEY);
            removePreferenceIfPresent(PREF_adrbrowsiel_ENABLE_TAB_GROUPS);
        }

        if (!NightModeUtils.isNightModeSupported()) {
            removePreferenceIfPresent(PREF_UI_THEME);
        }

        if (!ChromeFeatureList.isEnabled(adrbrowsielFeatureList.adrbrowsiel_REWARDS)) {
            removePreferenceIfPresent(PREF_HIDE_adrbrowsiel_REWARDS_ICON);
        }
    }

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {}

    private void removePreferenceIfPresent(String key) {
        Preference preference = getPreferenceScreen().findPreference(key);
        if (preference != null) getPreferenceScreen().removePreference(preference);
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        ChromeSwitchPreference hideadrbrowsielRewardsIconPref =
                (ChromeSwitchPreference) findPreference(PREF_HIDE_adrbrowsiel_REWARDS_ICON);
        if (hideadrbrowsielRewardsIconPref != null) {
            SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
            hideadrbrowsielRewardsIconPref.setChecked(
                    sharedPreferences.getBoolean(PREF_HIDE_adrbrowsiel_REWARDS_ICON, false));
            hideadrbrowsielRewardsIconPref.setOnPreferenceChangeListener(this);
        }

        Preference nightModeEnabled = findPreference(PREF_adrbrowsiel_NIGHT_MODE_ENABLED);
        nightModeEnabled.setOnPreferenceChangeListener(this);
        if (nightModeEnabled instanceof ChromeSwitchPreference) {
            ((ChromeSwitchPreference) nightModeEnabled)
                    .setChecked(ChromeFeatureList.isEnabled(
                            adrbrowsielFeatureList.FORCE_WEB_CONTENTS_DARK_MODE));
        }

        Preference enableBottomToolbar =
                findPreference(adrbrowsielPreferenceKeys.adrbrowsiel_BOTTOM_TOOLBAR_ENABLED_KEY);
        if (enableBottomToolbar == null) return;

        enableBottomToolbar.setOnPreferenceChangeListener(this);
        if (enableBottomToolbar instanceof ChromeSwitchPreference) {
            boolean isTablet = DeviceFormFactor.isNonMultiDisplayContextOnTablet(
                    ContextUtils.getApplicationContext());
            ((ChromeSwitchPreference) enableBottomToolbar)
                    .setChecked(!isTablet && BottomToolbarConfiguration.isBottomToolbarEnabled());
        }

        Preference enableTabGroups = findPreference(PREF_adrbrowsiel_ENABLE_TAB_GROUPS);
        enableTabGroups.setOnPreferenceChangeListener(this);
        if (enableTabGroups instanceof ChromeSwitchPreference) {
            ((ChromeSwitchPreference) enableTabGroups)
                    .setChecked(ChromeFeatureList.isEnabled(ChromeFeatureList.TAB_GROUPS_ANDROID));
        }
    }

    @Override
    public void onStart() {
        madrbrowsielRewardsNativeWorker = adrbrowsielRewardsNativeWorker.getInstance();
        if (madrbrowsielRewardsNativeWorker != null) {
            madrbrowsielRewardsNativeWorker.AddObserver(this);
        }
        super.onStart();
    }

    @Override
    public void onStop() {
        if (madrbrowsielRewardsNativeWorker != null) {
            madrbrowsielRewardsNativeWorker.RemoveObserver(this);
        }
        super.onStop();
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        String key = preference.getKey();
        if (adrbrowsielPreferenceKeys.adrbrowsiel_BOTTOM_TOOLBAR_ENABLED_KEY.equals(key)) {
            SharedPreferences prefs = ContextUtils.getAppSharedPreferences();
            Boolean originalStatus = BottomToolbarConfiguration.isBottomToolbarEnabled();
            prefs.edit()
                    .putBoolean(
                            adrbrowsielPreferenceKeys.adrbrowsiel_BOTTOM_TOOLBAR_ENABLED_KEY, !originalStatus)
                    .apply();
            adrbrowsielRelaunchUtils.askForRelaunch(getActivity());
        } else if (PREF_HIDE_adrbrowsiel_REWARDS_ICON.equals(key)) {
            SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
            SharedPreferences.Editor sharedPreferencesEditor = sharedPreferences.edit();
            sharedPreferencesEditor.putBoolean(PREF_HIDE_adrbrowsiel_REWARDS_ICON, !(boolean) newValue);
            sharedPreferencesEditor.apply();
            adrbrowsielRelaunchUtils.askForRelaunch(getActivity());
        } else if (PREF_adrbrowsiel_NIGHT_MODE_ENABLED.equals(key)) {
            adrbrowsielFeatureList.enableFeature(
                    adrbrowsielFeatureList.ENABLE_FORCE_DARK, (boolean) newValue, true);
            adrbrowsielRelaunchUtils.askForRelaunch(getActivity());
        } else if (PREF_adrbrowsiel_ENABLE_TAB_GROUPS.equals(key)) {
            adrbrowsielFeatureList.enableFeature(
                    adrbrowsielFeatureList.ENABLE_TAB_GROUPS, (boolean) newValue, false);
            adrbrowsielFeatureList.enableFeature(
                    adrbrowsielFeatureList.ENABLE_TAB_GRID, (boolean) newValue, false);
            SharedPreferencesManager.getInstance().writeBoolean(
                    adrbrowsielPreferenceKeys.adrbrowsiel_DOUBLE_RESTART, true);
            adrbrowsielRelaunchUtils.askForRelaunch(getActivity());
        }

        return true;
    }
}
