/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Bundle;
import androidx.preference.Preference;
import androidx.preference.PreferenceFragmentCompat;
import android.os.Build;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.content.Intent;

import org.chromium.chrome.R;
import org.chromium.base.ContextUtils;
import org.chromium.chrome.browser.adrbrowsielFeatureList;
import org.chromium.chrome.browser.ChromeTabbedActivity;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.chrome.browser.preferences.adrbrowsielPrefServiceBridge;
import org.chromium.chrome.browser.settings.developer.adrbrowsielRewardsDebugPreferences;

public class adrbrowsielPreferenceFragment extends PreferenceFragmentCompat {
    protected static final int STORAGE_PERMISSION_EXPORT_REQUEST_CODE = 8000;
    protected static final int STORAGE_PERMISSION_IMPORT_REQUEST_CODE = STORAGE_PERMISSION_EXPORT_REQUEST_CODE + 1;

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        setHasOptionsMenu(true);
        super.onActivityCreated(savedInstanceState);
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        menu.clear();
        inflater.inflate(R.menu.exit_settings_menu, menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == R.id.close_menu_id) {
            Intent intent = new Intent(getActivity(), ChromeTabbedActivity.class);
            intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
            startActivity(intent);
        }
        return false;
    }

    @Override
    public void onCreatePreferences(Bundle bundle, String s) {
    }

    @Override
    public void onResume() {
        super.onResume();
        if (!ChromeFeatureList.isEnabled(adrbrowsielFeatureList.adrbrowsiel_REWARDS)
                || adrbrowsielPrefServiceBridge.getInstance().getSafetynetCheckFailed()) {
            if (getPreferenceScreen() == null) return;
            Preference adrbrowsielRewardsDebugPreference =
                    getPreferenceScreen().findPreference(adrbrowsielRewardsDebugPreferences.KEY);
            if (adrbrowsielRewardsDebugPreference != null) {
                getPreferenceScreen().removePreference(adrbrowsielRewardsDebugPreference);
            }
        }
    }

    protected boolean isStoragePermissionGranted(boolean isExport) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            Context context = ContextUtils.getApplicationContext();
            if (context.checkSelfPermission(
                    android.Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED) {
                return true;
            } else {
                requestPermissions(
                        new String[] { android.Manifest.permission.WRITE_EXTERNAL_STORAGE },
                        isExport ? STORAGE_PERMISSION_EXPORT_REQUEST_CODE : STORAGE_PERMISSION_IMPORT_REQUEST_CODE);
                return false;
            }
        }
        return true;
    }
}
