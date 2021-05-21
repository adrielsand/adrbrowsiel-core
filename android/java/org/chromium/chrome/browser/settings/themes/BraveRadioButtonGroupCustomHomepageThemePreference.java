/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings.themes;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.RadioGroup;

import androidx.preference.PreferenceViewHolder;

import org.chromium.chrome.browser.adrbrowsielRelaunchUtils;
import org.chromium.chrome.browser.night_mode.R;
import org.chromium.chrome.browser.night_mode.settings.RadioButtonGroupThemePreference;
import org.chromium.chrome.browser.ntp_background_images.NTPBackgroundImagesBridge;
import org.chromium.chrome.browser.preferences.adrbrowsielPref;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.components.browser_ui.widget.RadioButtonWithDescription;
import org.chromium.components.user_prefs.UserPrefs;

public class adrbrowsielRadioButtonGroupCustomHomepageThemePreference extends RadioButtonGroupThemePreference {

    private RadioButtonWithDescription adrbrowsielDefaultView;
    private RadioButtonWithDescription refView;
    private NTPBackgroundImagesBridge mNTPBackgroundImagesBridge;

    public adrbrowsielRadioButtonGroupCustomHomepageThemePreference(Context context, AttributeSet attrs) {
        super(context, attrs);
        Profile mProfile = Profile.getLastUsedRegularProfile();
        mNTPBackgroundImagesBridge = NTPBackgroundImagesBridge.getInstance(mProfile);
    }

    @Override
    public void onBindViewHolder(PreferenceViewHolder holder) {
        super.onBindViewHolder(holder);

        holder.findViewById(R.id.system_default).setVisibility(View.GONE);

        RadioButtonWithDescription adrbrowsielDefaultView = (RadioButtonWithDescription)holder.findViewById(R.id.light);
        RadioButtonWithDescription refView = (RadioButtonWithDescription)holder.findViewById(R.id.dark);
        if (mNTPBackgroundImagesBridge != null && mNTPBackgroundImagesBridge.isSuperReferral()) {
            refView.setPrimaryText(mNTPBackgroundImagesBridge.getSuperReferralThemeName());
            if(UserPrefs.get(Profile.getLastUsedRegularProfile()).getInteger(adrbrowsielPref.NEW_TAB_PAGE_SUPER_REFERRAL_THEMES_OPTION) == 1 ? true : false) {
                refView.setChecked(true);
                adrbrowsielDefaultView.setChecked(false);
            } else {
                refView.setChecked(false);
                adrbrowsielDefaultView.setChecked(true);
            }
        }
        adrbrowsielDefaultView.setPrimaryText(
                getContext().getResources().getString(org.chromium.chrome.R.string.adrbrowsiel_default));
    }

    @Override
    public void onCheckedChanged(RadioGroup group, int checkedId) {
        super.onCheckedChanged(group, checkedId);
        UserPrefs.get(Profile.getLastUsedRegularProfile()).setInteger(adrbrowsielPref.NEW_TAB_PAGE_SUPER_REFERRAL_THEMES_OPTION, checkedId == R.id.light ? 0 : 1 );
        adrbrowsielRelaunchUtils.askForRelaunch(getContext());
    }
}
