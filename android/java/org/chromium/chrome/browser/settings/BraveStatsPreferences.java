/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.widget.Toast;

import androidx.preference.Preference;
import androidx.preference.Preference.OnPreferenceChangeListener;

import org.chromium.base.ContextUtils;
import org.chromium.base.task.AsyncTask;
import org.chromium.base.ThreadUtils;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.adrbrowsielRelaunchUtils;
import org.chromium.chrome.browser.ntp_background_images.NTPBackgroundImagesBridge;
import org.chromium.chrome.browser.preferences.adrbrowsielPrefServiceBridge;
import org.chromium.chrome.browser.settings.adrbrowsielPreferenceFragment;
import org.chromium.components.browser_ui.settings.ChromeSwitchPreference;
import org.chromium.components.browser_ui.settings.SettingsUtils;
import org.chromium.chrome.browser.onboarding.OnboardingPrefManager;
import org.chromium.chrome.browser.local_database.DatabaseHelper;

/**
 * Fragment to keep track of all the display related preferences.
 */
public class adrbrowsielStatsPreferences extends adrbrowsielPreferenceFragment
    implements OnPreferenceChangeListener {

    // deprecated preferences from browser-android-tabs
    public static final String PREF_adrbrowsiel_STATS = "adrbrowsiel_stats";
    public static final String PREF_adrbrowsiel_STATS_NOTIFICATION = "adrbrowsiel_stats_notification";
    public static final String PREF_CLEAR_adrbrowsiel_STATS = "clear_adrbrowsiel_stats";

    private ChromeSwitchPreference adrbrowsielStatsPref;
    private ChromeSwitchPreference adrbrowsielStatsNotificationPref;

    private DatabaseHelper mDatabaseHelper = DatabaseHelper.getInstance();

    private SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
    private SharedPreferences.Editor sharedPreferencesEditor = sharedPreferences.edit();

    public static int getPreferenceSummary() {
        return OnboardingPrefManager.getInstance().isadrbrowsielStatsEnabled() ? R.string.text_on : R.string.text_off;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getActivity().setTitle(R.string.adrbrowsiel_stats);
        SettingsUtils.addPreferencesFromResource(this, R.xml.adrbrowsiel_stats_preferences);
        findPreference(PREF_CLEAR_adrbrowsiel_STATS).setOnPreferenceClickListener(new Preference.OnPreferenceClickListener() {
            @Override
            public boolean onPreferenceClick(Preference preference) {
                new AsyncTask<Void>() {
                    @Override
                    protected Void doInBackground() {
                        try {
                            mDatabaseHelper.clearStatsTable();
                            mDatabaseHelper.clearSavedBandwidthTable();
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
                        Toast.makeText(ContextUtils.getApplicationContext(), ContextUtils.getApplicationContext().getResources().getString(R.string.data_has_been_cleared), Toast.LENGTH_SHORT).show();
                    }
                } .executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
                return true;
            }
        });
    }

    private void removePreferenceIfPresent(String key) {
        Preference preference = getPreferenceScreen().findPreference(key);
        if (preference != null) getPreferenceScreen().removePreference(preference);
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        adrbrowsielStatsPref = (ChromeSwitchPreference) findPreference(PREF_adrbrowsiel_STATS);
        if (adrbrowsielStatsPref != null) {
            adrbrowsielStatsPref.setEnabled(true);
            adrbrowsielStatsPref.setChecked(OnboardingPrefManager.getInstance().isadrbrowsielStatsEnabled());
            adrbrowsielStatsPref.setOnPreferenceChangeListener(this);
        }
        adrbrowsielStatsNotificationPref = (ChromeSwitchPreference) findPreference(PREF_adrbrowsiel_STATS_NOTIFICATION);
        if (adrbrowsielStatsNotificationPref != null) {
            adrbrowsielStatsNotificationPref.setEnabled(true);
            adrbrowsielStatsNotificationPref.setChecked(OnboardingPrefManager.getInstance().isadrbrowsielStatsNotificationEnabled());
            adrbrowsielStatsNotificationPref.setOnPreferenceChangeListener(this);
        }
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        setOnPreferenceValue(preference.getKey(), (boolean)newValue);
        return true;
    }

    public static void setOnPreferenceValue(String preferenceName, boolean newValue) {
        if (PREF_adrbrowsiel_STATS.equals(preferenceName)) {
            OnboardingPrefManager.getInstance().setadrbrowsielStatsEnabled((boolean)newValue);
        } else if (PREF_adrbrowsiel_STATS_NOTIFICATION.equals(preferenceName)) {
            OnboardingPrefManager.getInstance().setadrbrowsielStatsNotificationEnabled((boolean)newValue);
        } else {
            SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
            SharedPreferences.Editor sharedPreferencesEditor = sharedPreferences.edit();
            sharedPreferencesEditor.putBoolean(preferenceName, newValue);
            sharedPreferencesEditor.apply();
        }
    }

    public static void setOnPreferenceValue(String preferenceName, int newValue) {
        SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
        SharedPreferences.Editor sharedPreferencesEditor = sharedPreferences.edit();
        sharedPreferencesEditor.putInt(preferenceName, newValue);
        sharedPreferencesEditor.apply();
    }
}
