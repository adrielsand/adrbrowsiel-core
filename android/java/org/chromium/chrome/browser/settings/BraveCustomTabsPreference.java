/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.os.Bundle;
import android.content.SharedPreferences;
import androidx.preference.Preference;

import org.chromium.base.ContextUtils;
import org.chromium.chrome.browser.adrbrowsielLaunchIntentDispatcher;
import org.chromium.chrome.browser.preferences.adrbrowsielPreferenceKeys;
import org.chromium.chrome.browser.settings.adrbrowsielPreferenceFragment;
import org.chromium.chrome.R;
import org.chromium.components.browser_ui.settings.ChromeSwitchPreference;
import org.chromium.components.browser_ui.settings.SettingsUtils;

public class adrbrowsielCustomTabsPreference extends adrbrowsielPreferenceFragment
        implements Preference.OnPreferenceChangeListener {
    public static int getPreferenceSummary() {
        return adrbrowsielLaunchIntentDispatcher.useCustomTabs() ? R.string.text_on : R.string.text_off;
    }

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        getActivity().setTitle(R.string.prefs_use_custom_tabs);
        SettingsUtils.addPreferencesFromResource(this, R.xml.use_custom_tabs_adrbrowsiel_preference);

        ChromeSwitchPreference pref = (ChromeSwitchPreference) findPreference(
                adrbrowsielPreferenceKeys.adrbrowsiel_USE_CUSTOM_TABS);
        pref.setChecked(adrbrowsielLaunchIntentDispatcher.useCustomTabs());
        pref.setOnPreferenceChangeListener(this);
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        SharedPreferences.Editor sharedPreferencesEditor = ContextUtils.getAppSharedPreferences().edit();
        sharedPreferencesEditor.putBoolean(adrbrowsielPreferenceKeys.adrbrowsiel_USE_CUSTOM_TABS, (boolean) newValue);
        sharedPreferencesEditor.apply();
        return true;
    }
}
