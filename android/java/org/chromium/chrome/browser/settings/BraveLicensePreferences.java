/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.os.Bundle;

import org.chromium.base.Log;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.adrbrowsielRewardsHelper;
import org.chromium.chrome.browser.settings.adrbrowsielPreferenceFragment;
import org.chromium.components.browser_ui.settings.SettingsUtils;

import java.io.IOException;
import java.io.InputStream;
import java.util.Scanner;

/**
 * Fragment to display adrbrowsiel license information.
 */
public class adrbrowsielLicensePreferences extends adrbrowsielPreferenceFragment {
    private static final String TAG = "adrbrowsielLicense";

    private static final String PREF_adrbrowsiel_LICENSE_TEXT = "adrbrowsiel_license_text";
    private static final String ASSET_adrbrowsiel_LICENSE = "LICENSE.html";

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String s) {
        SettingsUtils.addPreferencesFromResource(this, R.xml.adrbrowsiel_license_preferences);
        getActivity().setTitle(R.string.adrbrowsiel_license_text);
        adrbrowsielLicensePreference licenseText =
                (adrbrowsielLicensePreference) findPreference(PREF_adrbrowsiel_LICENSE_TEXT);
        try {
            InputStream in = getActivity().getAssets().open(ASSET_adrbrowsiel_LICENSE);
            Scanner scanner = new Scanner(in).useDelimiter("\\A");
            String summary = scanner.hasNext() ? scanner.next() : "";
            in.close();
            licenseText.setSummary(adrbrowsielRewardsHelper.spannedFromHtmlString(summary));
        } catch (IOException e) {
            Log.e(TAG, "Could not load license text: " + e);
        }
    }
}
