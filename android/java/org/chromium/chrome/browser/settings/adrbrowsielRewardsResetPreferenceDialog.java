/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.os.Bundle;
import androidx.preference.PreferenceDialogFragmentCompat;
import android.view.View;

import org.chromium.chrome.browser.adrbrowsielRewardsNativeWorker;

/**
 * The dialog used to reset adrbrowsiel Rewards.
 */
public class adrbrowsielRewardsResetPreferenceDialog extends PreferenceDialogFragmentCompat {
    public static final String TAG = "adrbrowsielRewardsResetPreferenceDialog";

    public static adrbrowsielRewardsResetPreferenceDialog newInstance(
            adrbrowsielRewardsResetPreference preference) {
        adrbrowsielRewardsResetPreferenceDialog fragment = new adrbrowsielRewardsResetPreferenceDialog();
        Bundle bundle = new Bundle(1);
        bundle.putString(PreferenceDialogFragmentCompat.ARG_KEY, preference.getKey());
        fragment.setArguments(bundle);
        return fragment;
    }

    @Override
    protected void onBindDialogView(View view) {
        super.onBindDialogView(view);
    }

    @Override
    public void onDialogClosed(boolean positive) {}
    
}
