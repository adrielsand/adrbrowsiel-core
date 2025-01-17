/**
 * Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.onboarding;

import android.os.Bundle;
import android.text.SpannableString;
import android.text.Spanned;
import android.text.TextPaint;
import android.text.method.LinkMovementMethod;
import android.text.style.ClickableSpan;
import android.text.style.ForegroundColorSpan;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import org.chromium.base.Log;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.adrbrowsielRewardsHelper;
import org.chromium.chrome.browser.app.adrbrowsielActivity;
import org.chromium.chrome.browser.customtabs.CustomTabActivity;
import org.chromium.chrome.browser.night_mode.GlobalNightModeStateProviderHolder;
import org.chromium.chrome.browser.onboarding.OnboardingPrefManager;
import org.chromium.chrome.browser.preferences.adrbrowsielPrefServiceBridge;
import org.chromium.chrome.browser.util.PackageUtils;

public class P3aOnboardingActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_p3a_onboarding);

        boolean isFirstInstall = PackageUtils.isFirstInstall(this);

        TextView p3aOnboardingTitle = findViewById(R.id.p3a_onboarding_title);
        p3aOnboardingTitle.setText(isFirstInstall
                        ? getResources().getString(R.string.p3a_onboarding_title_text_1)
                        : getResources().getString(R.string.p3a_onboarding_title_text_2));
        CheckBox p3aOnboardingCheckbox = findViewById(R.id.p3a_onboarding_checkbox);
        boolean isP3aEnabled = true;
        try {
            isP3aEnabled = adrbrowsielPrefServiceBridge.getInstance().getP3AEnabled();
        } catch (Exception e) {
            Log.e("P3aOnboarding", e.getMessage());
        }
        p3aOnboardingCheckbox.setChecked(isP3aEnabled);
        p3aOnboardingCheckbox.setOnCheckedChangeListener(
                new CompoundButton.OnCheckedChangeListener() {
                    @Override
                    public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                        try {
                            adrbrowsielPrefServiceBridge.getInstance().setP3AEnabled(isChecked);
                            adrbrowsielPrefServiceBridge.getInstance().setP3ANoticeAcknowledged(true);
                        } catch (Exception e) {
                            Log.e("P3aOnboarding", e.getMessage());
                        }
                    }
                });
        ImageView p3aOnboardingImg = findViewById(R.id.p3a_onboarding_img);
        p3aOnboardingImg.setImageResource(isFirstInstall
                        ? R.drawable.ic_adrbrowsiel_logo
                        : (GlobalNightModeStateProviderHolder.getInstance().isInNightMode()
                                        ? R.drawable.ic_spot_graphic_dark
                                        : R.drawable.ic_spot_graphic));
        TextView p3aOnboardingText = findViewById(R.id.p3a_onboarding_text);
        Button btnContinue = findViewById(R.id.btn_continue);
        btnContinue.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (PackageUtils.isFirstInstall(P3aOnboardingActivity.this)
                        && !OnboardingPrefManager.getInstance().isNewOnboardingShown()
                        && adrbrowsielActivity.getadrbrowsielActivity() != null) {
                    adrbrowsielActivity.getadrbrowsielActivity().showOnboardingV2(false);
                }
                OnboardingPrefManager.getInstance().setP3aOnboardingShown(true);
                OnboardingPrefManager.getInstance().setShowDefaultBrowserModalAfterP3A(true);
                finish();
            }
        });

        String productAnalysisString =
                String.format(getResources().getString(R.string.p3a_onboarding_checkbox_text,
                        getResources().getString(R.string.private_product_analysis_text)));
        int productAnalysisIndex = productAnalysisString.indexOf(
                getResources().getString(R.string.private_product_analysis_text));
        Spanned productAnalysisSpanned =
                adrbrowsielRewardsHelper.spannedFromHtmlString(productAnalysisString);
        SpannableString productAnalysisTextSS =
                new SpannableString(productAnalysisSpanned.toString());

        ClickableSpan productAnalysisClickableSpan = new ClickableSpan() {
            @Override
            public void onClick(@NonNull View textView) {
                CustomTabActivity.showInfoPage(P3aOnboardingActivity.this, adrbrowsielActivity.P3A_URL);
            }
            @Override
            public void updateDrawState(@NonNull TextPaint ds) {
                super.updateDrawState(ds);
                ds.setUnderlineText(false);
            }
        };

        productAnalysisTextSS.setSpan(productAnalysisClickableSpan, productAnalysisIndex,
                productAnalysisIndex
                        + getResources().getString(R.string.private_product_analysis_text).length(),
                Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
        productAnalysisTextSS.setSpan(
                new ForegroundColorSpan(getResources().getColor(R.color.adrbrowsiel_blue_tint_color)),
                productAnalysisIndex,
                productAnalysisIndex
                        + getResources().getString(R.string.private_product_analysis_text).length(),
                Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
        p3aOnboardingText.setMovementMethod(LinkMovementMethod.getInstance());
        p3aOnboardingText.setText(productAnalysisTextSS);
    }

    @Override
    public void onBackPressed() {}
}
