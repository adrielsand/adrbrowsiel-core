/** Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
  * This Source Code Form is subject to the terms of the Mozilla Public
  * License, v. 2.0. If a copy of the MPL was not distributed with this file,
  * You can obtain one at http://mozilla.org/MPL/2.0/.
  */

package org.chromium.chrome.browser;

import org.chromium.base.annotations.JNINamespace;
import org.chromium.base.annotations.NativeMethods;
import org.chromium.chrome.browser.profiles.Profile;

@JNINamespace("adrbrowsiel_ads")
public class adrbrowsielAdsNativeHelper {
    private adrbrowsielAdsNativeHelper() {}

    public static boolean nativeIsadrbrowsielAdsEnabled(Profile profile) {
        return adrbrowsielAdsNativeHelperJni.get().isadrbrowsielAdsEnabled(profile);
    };

    public static void nativeSetAdsEnabled(Profile profile) {
        adrbrowsielAdsNativeHelperJni.get().setAdsEnabled(profile);
    };

    public static boolean nativeIsNewlySupportedLocale(Profile profile) {
        return adrbrowsielAdsNativeHelperJni.get().isNewlySupportedLocale(profile);
    };

    public static boolean nativeIsSupportedLocale(Profile profile) {
        return adrbrowsielAdsNativeHelperJni.get().isSupportedLocale(profile);
    };

    public static void nativeOnShowAdNotification(Profile profile, String j_notification_id) {
        adrbrowsielAdsNativeHelperJni.get().onShowAdNotification(profile, j_notification_id);
    };

    public static void nativeOnCloseAdNotification(
            Profile profile, String j_notification_id, boolean j_by_user) {
        adrbrowsielAdsNativeHelperJni.get().onCloseAdNotification(profile, j_notification_id, j_by_user);
    };

    public static void nativeOnClickAdNotification(Profile profile, String j_notification_id) {
        adrbrowsielAdsNativeHelperJni.get().onClickAdNotification(profile, j_notification_id);
    };

    @NativeMethods
    interface Natives {
        boolean isadrbrowsielAdsEnabled(Profile profile);
        void setAdsEnabled(Profile profile);
        boolean isSupportedLocale(Profile profile);
        boolean isNewlySupportedLocale(Profile profile);
        void onShowAdNotification(Profile profile, String j_notification_id);
        void onCloseAdNotification(Profile profile, String j_notification_id, boolean j_by_user);
        void onClickAdNotification(Profile profile, String j_notification_id);
    }
}
