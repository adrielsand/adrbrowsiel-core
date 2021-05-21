/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.content.SharedPreferences;
import android.os.Bundle;

import androidx.preference.Preference;
import androidx.preference.Preference.OnPreferenceChangeListener;

import org.chromium.base.ContextUtils;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.adrbrowsielRelaunchUtils;
import org.chromium.chrome.browser.adrbrowsielRewardsNativeWorker;
import org.chromium.chrome.browser.adrbrowsielRewardsObserver;
import org.chromium.chrome.browser.adrbrowsielRewardsPanelPopup;
import org.chromium.chrome.browser.preferences.adrbrowsielPrefServiceBridge;
import org.chromium.chrome.browser.settings.adrbrowsielPreferenceFragment;
import org.chromium.components.browser_ui.settings.ChromeSwitchPreference;
import org.chromium.components.browser_ui.settings.SettingsUtils;

/**
 * Fragment to keep track of all adrbrowsiel Rewards related preferences.
 */
public class adrbrowsielRewardsPreferences extends adrbrowsielPreferenceFragment
        implements OnPreferenceChangeListener, adrbrowsielRewardsObserver {
    public static final String PREF_ADS_SWITCH = "ads_switch";

    // flag, if exists: default state (off) for background adrbrowsiel ads has been set
    public static final String PREF_ADS_SWITCH_DEFAULT_HAS_BEEN_SET = "ads_switch_default_set";

    private ChromeSwitchPreference mAdsSwitch;

    private adrbrowsielRewardsNativeWorker madrbrowsielRewardsNativeWorker;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getActivity().setTitle(R.string.adrbrowsiel_ui_adrbrowsiel_rewards);
        SettingsUtils.addPreferencesFromResource(this, R.xml.adrbrowsiel_rewards_preferences);
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        mAdsSwitch = (ChromeSwitchPreference) findPreference(PREF_ADS_SWITCH);
        boolean isAdsInBackgroundEnabled = getPrefAdsInBackgroundEnabled();
        mAdsSwitch.setChecked(isAdsInBackgroundEnabled);
        mAdsSwitch.setOnPreferenceChangeListener(new OnPreferenceChangeListener() {
            @Override
            public boolean onPreferenceChange(Preference preference, Object newValue) {
                setPrefAdsInBackgroundEnabled((boolean) newValue);
                return true;
            }
        });
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

    /**
     * Returns the user preference for whether the adrbrowsiel ads in background is enabled.
     *
     */
    public static boolean getPrefAdsInBackgroundEnabled() {
        SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
        return sharedPreferences.getBoolean(PREF_ADS_SWITCH, true);
    }

    /**
     * Sets the user preference for whether the adrbrowsiel ads in background is enabled.
     */
    public void setPrefAdsInBackgroundEnabled(boolean enabled) {
        SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
        SharedPreferences.Editor sharedPreferencesEditor = sharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(PREF_ADS_SWITCH, enabled);
        sharedPreferencesEditor.apply();
    }

    @Override
    public void onDisplayPreferenceDialog(Preference preference) {
        if (preference instanceof adrbrowsielRewardsResetPreference) {
            adrbrowsielRewardsResetPreferenceDialog dialogFragment =
                    adrbrowsielRewardsResetPreferenceDialog.newInstance(
                            (adrbrowsielRewardsResetPreference) preference);
            dialogFragment.setTargetFragment(this, 0);
            dialogFragment.show(getFragmentManager(), adrbrowsielRewardsResetPreferenceDialog.TAG);
        } else {
            super.onDisplayPreferenceDialog(preference);
        }
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        return true;
    }

    @Override
    public void OnResetTheWholeState(boolean success) {
        if (success) {
            SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
            SharedPreferences.Editor sharedPreferencesEditor = sharedPreferences.edit();
            sharedPreferencesEditor.putBoolean(
                    adrbrowsielRewardsPanelPopup.PREF_GRANTS_NOTIFICATION_RECEIVED, false);
            sharedPreferencesEditor.putBoolean(
                    adrbrowsielRewardsPanelPopup.PREF_WAS_adrbrowsiel_REWARDS_TURNED_ON, false);
            sharedPreferencesEditor.apply();
            adrbrowsielPrefServiceBridge.getInstance().setSafetynetCheckFailed(false);
            adrbrowsielRelaunchUtils.askForRelaunch(getActivity());
        } else {
            adrbrowsielRelaunchUtils.askForRelaunchCustom(getActivity());
        }
    }

    @Override
    public void onCreatePreferences(Bundle bundle, String s) {}
}
