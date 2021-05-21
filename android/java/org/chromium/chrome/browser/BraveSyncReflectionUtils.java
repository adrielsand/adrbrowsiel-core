/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser;

import java.lang.reflect.Method;

import org.chromium.base.Log;
import org.chromium.base.ThreadUtils;
import org.chromium.chrome.browser.adrbrowsielConfig;

// The purpose of this class is to hide adrbrowsielSyncWorker object under `enable_adrbrowsiel_sync`
// and create it without explict import
public class adrbrowsielSyncReflectionUtils {
    private static Object sadrbrowsielSyncWorker;
    private static boolean sInitialized;
    private static String TAG = "SYNC";

    public static Object getSyncWorker() {
        // May be invoked in non-UI thread when we do validation for camera QR in callback
        if (!sInitialized) {
            if (adrbrowsielConfig.SYNC_ENABLED) {
                try {
                    sadrbrowsielSyncWorker =
                        Class.forName("org.chromium.chrome.browser.adrbrowsielSyncWorker")
                             .getConstructor()
                             .newInstance();
                } catch (Exception e) {
                    Log.e(TAG, "Cannot create adrbrowsielSyncWorker ", e);
                }
            }
            sInitialized = true;
        }
        return sadrbrowsielSyncWorker;
    }

    public static void showInformers() {
        if (!adrbrowsielConfig.SYNC_ENABLED) {
            return;
        }

        try {
            Method method = Class.forName("org.chromium.chrome.browser.adrbrowsielSyncInformers").getDeclaredMethod("show");
            method.invoke(null);
        } catch (Exception e) {
            Log.e(TAG, "Cannot show sync informers with reflection ", e);
        }
    }
}
