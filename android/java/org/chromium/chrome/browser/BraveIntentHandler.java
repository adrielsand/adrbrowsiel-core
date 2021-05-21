/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;

public class adrbrowsielIntentHandler extends IntentHandler {
    private static final String CONNECTION_INFO_HELP_URL =
            "https://support.google.com/chrome?p=android_connection_info";
    private static final String adrbrowsiel_CONNECTION_INFO_HELP_URL =
            "https://support.adrbrowsiel.com/hc/en-us/articles/360018185871-How-do-I-check-if-a-site-s-connection-is-secure-";

    public adrbrowsielIntentHandler(Activity activity, IntentHandlerDelegate delegate) {
        super(activity, delegate);
    }

    @Override
    public boolean onNewIntent(Intent intent) {
        // Redirect requests if necessary
        String url = getUrlFromIntent(intent);
        if (url != null && url.equals(CONNECTION_INFO_HELP_URL)) {
            intent.setData(Uri.parse(adrbrowsiel_CONNECTION_INFO_HELP_URL));
        }
        return super.onNewIntent(intent);
    }
}
