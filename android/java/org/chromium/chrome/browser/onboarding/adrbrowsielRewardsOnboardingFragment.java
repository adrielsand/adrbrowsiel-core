/**
 * Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.onboarding;

import android.content.Intent;
import android.os.Bundle;
import android.text.SpannableString;
import android.text.Spanned;
import android.text.TextPaint;
import android.text.method.LinkMovementMethod;
import android.text.method.ScrollingMovementMethod;
import android.text.style.ClickableSpan;
import android.text.style.ForegroundColorSpan;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;

import org.chromium.base.ContextUtils;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.adrbrowsielRewardsHelper;
import org.chromium.chrome.browser.adrbrowsielRewardsNativeWorker;
import org.chromium.chrome.browser.customtabs.CustomTabActivity;
import org.chromium.chrome.browser.onboarding.OnViewPagerAction;
import org.chromium.chrome.browser.onboarding.OnboardingPrefManager;
import org.chromium.chrome.browser.adrbrowsielAdsNativeHelper;
import org.chromium.chrome.browser.profiles.Profile;

public class adrbrowsielRewardsOnboardingFragment extends Fragment {
    private OnViewPagerAction onViewPagerAction;

    private TextView tvTitle;
    private TextView tvText;
    private TextView tvAgree;

    private Button btnSkip;
    private Button btnNext;

    private static final String adrbrowsiel_TERMS_PAGE =
        "https://basicattentiontoken.org/user-terms-of-service/";

    private boolean isAdsAvailable;

    private boolean isAnonWallet;

    public adrbrowsielRewardsOnboardingFragment() {
        // Required empty public constructor
    }

    @Override
    public View onCreateView(
        LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        isAdsAvailable = OnboardingPrefManager.getInstance().isAdsAvailable();

        isAnonWallet = adrbrowsielRewardsHelper.isAnonWallet();

        // Inflate the layout for this fragment
        View root = inflater.inflate(R.layout.fragment_adrbrowsiel_rewards_onboarding, container, false);

        initializeViews(root);

        setActions();

        return root;
    }

    private void initializeViews(View root) {
        tvTitle = root.findViewById(R.id.section_title);
        tvText = root.findViewById(R.id.section_text);

        tvAgree = root.findViewById(R.id.agree_text);

        btnSkip = root.findViewById(R.id.btn_skip);
        btnNext = root.findViewById(R.id.btn_next);
    }

    private void setActions() {
        btnNext.setText(getResources().getString(R.string.earn_and_give));
        btnSkip.setText(getResources().getString(R.string.skip));

        String adrbrowsielRewardsText = "<b>" + String.format(getResources().getString(R.string.earn_tokens), isAnonWallet ? getResources().getString(R.string.point) : getResources().getString(R.string.token)) + "</b> " + getResources().getString(R.string.adrbrowsiel_rewards_onboarding_text);
        Spanned textToInsert = adrbrowsielRewardsHelper.spannedFromHtmlString(adrbrowsielRewardsText);
        tvText.setText(textToInsert);
        tvText.setMovementMethod(new ScrollingMovementMethod());

        String termsText = getResources().getString(R.string.terms_text) + " "
                           + getResources().getString(R.string.terms_of_service) + ".";
        Spanned textToAgree = adrbrowsielRewardsHelper.spannedFromHtmlString(termsText);
        SpannableString ss = new SpannableString(textToAgree.toString());

        ClickableSpan clickableSpan = new ClickableSpan() {
            @Override
            public void onClick(@NonNull View textView) {
                CustomTabActivity.showInfoPage(getActivity(), adrbrowsiel_TERMS_PAGE);
            }
            @Override
            public void updateDrawState(@NonNull TextPaint ds) {
                super.updateDrawState(ds);
                ds.setUnderlineText(false);
            }
        };

        ss.setSpan(clickableSpan, getResources().getString(R.string.terms_text).length(),
                   ss.length() - 1, Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);

        ForegroundColorSpan foregroundSpan =
            new ForegroundColorSpan(getResources().getColor(R.color.onboarding_orange));
        ss.setSpan(foregroundSpan, getResources().getString(R.string.terms_text).length(),
                   ss.length() - 1, Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
        tvAgree.setMovementMethod(LinkMovementMethod.getInstance());
        tvAgree.setText(ss);

        btnSkip.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                assert onViewPagerAction != null;
                if (onViewPagerAction != null)
                    onViewPagerAction.onSkip();
            }
        });

        btnNext.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                adrbrowsielAdsNativeHelper.nativeSetAdsEnabled(Profile.getLastUsedRegularProfile());
                adrbrowsielRewardsNativeWorker.getInstance().SetAutoContributeEnabled(true);
                assert onViewPagerAction != null;
                if (onViewPagerAction != null)
                    onViewPagerAction.onNext();
            }
        });
    }

    public void setOnViewPagerAction(OnViewPagerAction onViewPagerAction) {
        this.onViewPagerAction = onViewPagerAction;
    }
}
