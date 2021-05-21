/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.ntp;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.util.AttributeSet;
import android.view.View;
import android.widget.LinearLayout;

import org.chromium.base.ContextUtils;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.settings.adrbrowsielSearchEngineUtils;
import org.chromium.components.search_engines.TemplateUrl;

public class adrbrowsielDuckDuckGoOfferView extends LinearLayout {
    public static String DDG_SEARCH_ENGINE_SHORT_NAME = "DuckDuckGo";
    public static String PREF_DDG_OFFER_SHOWN = "adrbrowsiel_ddg_offer_shown";

    private Context mContext;
    private View mDDGOfferLink;

    public adrbrowsielDuckDuckGoOfferView(Context context, AttributeSet attrs) {
        super(context, attrs);
        mContext = context;
    }

    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();

        mDDGOfferLink = findViewById(R.id.ddg_offer_link);
        mDDGOfferLink.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                showDDGOffer(true);
            }
        });

        showDDGOffer(false);
    }

    private void showDDGOffer(boolean forceShow) {
        if (adrbrowsielSearchEngineUtils.getDSEShortName(true).equals(DDG_SEARCH_ENGINE_SHORT_NAME)) {
            mDDGOfferLink.setVisibility(View.GONE);
            return;
        }
        if (!forceShow
                && ContextUtils.getAppSharedPreferences().getBoolean(PREF_DDG_OFFER_SHOWN, false)) {
            return;
        }
        ContextUtils.getAppSharedPreferences()
                .edit()
                .putBoolean(PREF_DDG_OFFER_SHOWN, true)
                .apply();
        new AlertDialog.Builder(mContext, R.style.adrbrowsielDialogTheme)
                .setView(R.layout.ddg_offer_layout)
                .setPositiveButton(R.string.ddg_offer_positive,
                        new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                TemplateUrl templateUrl = adrbrowsielSearchEngineUtils
                                        .getTemplateUrlByShortName(DDG_SEARCH_ENGINE_SHORT_NAME);
                                if (templateUrl != null) {
                                    adrbrowsielSearchEngineUtils.setDSEPrefs(templateUrl, true);
                                    adrbrowsielSearchEngineUtils.updateActiveDSE(true);
                                }
                                mDDGOfferLink.setVisibility(View.GONE);
                            }
                        })
                .setNegativeButton(R.string.ddg_offer_negative, null)
                .show();
    }
}
