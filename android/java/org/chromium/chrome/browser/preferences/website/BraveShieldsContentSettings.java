/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.preferences.website;

import org.chromium.base.annotations.CalledByNative;
import org.chromium.base.annotations.JNINamespace;
import org.chromium.base.annotations.NativeMethods;
import org.chromium.chrome.browser.preferences.website.adrbrowsielShieldsContentSettingsObserver;
import org.chromium.chrome.browser.profiles.Profile;

import java.util.ArrayList;
import java.util.List;

@JNINamespace("chrome::android")
public class adrbrowsielShieldsContentSettings {
    static public final String RESOURCE_IDENTIFIER_ADS = "shieldsAds";
    static public final String RESOURCE_IDENTIFIER_TRACKERS = "trackers";
    static public final String RESOURCE_IDENTIFIER_ADS_TRACKERS = "ads_trackers";
    static public final String RESOURCE_IDENTIFIER_DATA_SAVED = "data_saved";
    static public final String RESOURCE_IDENTIFIER_HTTP_UPGRADABLE_RESOURCES = "httpUpgradableResources";
    static public final String RESOURCE_IDENTIFIER_adrbrowsiel_SHIELDS = "adrbrowsielShields";
    static public final String RESOURCE_IDENTIFIER_FINGERPRINTING = "fingerprinting";
    static public final String RESOURCE_IDENTIFIER_COOKIES = "shieldsCookies";
    static public final String RESOURCE_IDENTIFIER_REFERRERS = "referrers";
    static public final String RESOURCE_IDENTIFIER_JAVASCRIPTS = "javascript";

    static public final String BLOCK_RESOURCE = "block";
    static public final String BLOCK_THIRDPARTY_RESOURCE = "block_third_party";
    static public final String DEFAULT = "default";
    static public final String ALLOW_RESOURCE = "allow";
    static public final String AGGRESSIVE = "aggressive";

    private long mNativeadrbrowsielShieldsContentSettings;
    private List<adrbrowsielShieldsContentSettingsObserver> madrbrowsielShieldsContentSettingsObservers;
    private static adrbrowsielShieldsContentSettings sInstance;

    public static adrbrowsielShieldsContentSettings getInstance() {
        if (sInstance == null) sInstance = new adrbrowsielShieldsContentSettings();
        return sInstance;
    }

    private adrbrowsielShieldsContentSettings() {
        mNativeadrbrowsielShieldsContentSettings = 0;
        madrbrowsielShieldsContentSettingsObservers =
            new ArrayList<adrbrowsielShieldsContentSettingsObserver>();
        init();
    }

    public void addObserver(adrbrowsielShieldsContentSettingsObserver observer) {
        madrbrowsielShieldsContentSettingsObservers.add(observer);
    }

    public void removeObserver(adrbrowsielShieldsContentSettingsObserver observer) {
        madrbrowsielShieldsContentSettingsObservers.remove(observer);
    }

    private void init() {
        if (mNativeadrbrowsielShieldsContentSettings == 0) {
            adrbrowsielShieldsContentSettingsJni.get().init(this);
        }
    }

    @Override
    protected void finalize() {
        destroy();
    }

    private void destroy() {
        if (mNativeadrbrowsielShieldsContentSettings == 0) {
            return;
        }
        adrbrowsielShieldsContentSettingsJni.get().destroy(mNativeadrbrowsielShieldsContentSettings);
    }

    static public void setShields(Profile profile, String host, String resourceIndentifier, boolean value,
                                  boolean fromTopShields) {
        String setting_string = (value ? BLOCK_RESOURCE : ALLOW_RESOURCE);
        if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_adrbrowsiel_SHIELDS)) {
            adrbrowsielShieldsContentSettingsJni.get().setadrbrowsielShieldsEnabled(value, host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_ADS_TRACKERS)) {
            adrbrowsielShieldsContentSettingsJni.get().setAdControlType(setting_string, host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_HTTP_UPGRADABLE_RESOURCES)) {
            adrbrowsielShieldsContentSettingsJni.get().setHTTPSEverywhereEnabled(value, host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_JAVASCRIPTS)) {
            adrbrowsielShieldsContentSettingsJni.get().setNoScriptControlType(setting_string, host, profile);
        }
    }

    public static void setShieldsValue(Profile profile, String host, String resourceIndentifier,
                                       String settingOption, boolean fromTopShields) {
        if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_FINGERPRINTING)) {
            adrbrowsielShieldsContentSettingsJni.get().setFingerprintingControlType(settingOption, host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_COOKIES)) {
            adrbrowsielShieldsContentSettingsJni.get().setCookieControlType(settingOption, host, profile);
        }
    }

    public static boolean getShields(Profile profile, String host, String resourceIndentifier) {
        String settings = BLOCK_RESOURCE;
        if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_adrbrowsiel_SHIELDS)) {
            return adrbrowsielShieldsContentSettingsJni.get().getadrbrowsielShieldsEnabled(host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_ADS_TRACKERS)) {
            settings = adrbrowsielShieldsContentSettingsJni.get().getAdControlType(host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_HTTP_UPGRADABLE_RESOURCES)) {
            return adrbrowsielShieldsContentSettingsJni.get().getHTTPSEverywhereEnabled(host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_JAVASCRIPTS)) {
            settings = adrbrowsielShieldsContentSettingsJni.get().getNoScriptControlType(host, profile);
        }

        return !settings.equals(ALLOW_RESOURCE);
    }

    public static String getShieldsValue(Profile profile, String host, String resourceIndentifier) {
        String settings = BLOCK_RESOURCE;
        if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_FINGERPRINTING)) {
            settings = adrbrowsielShieldsContentSettingsJni.get().getFingerprintingControlType(host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_COOKIES)) {
            settings = adrbrowsielShieldsContentSettingsJni.get().getCookieControlType(host, profile);
        }
        return settings;
    }

    @CalledByNative
    private void setNativePtr(long nativePtr) {
        assert mNativeadrbrowsielShieldsContentSettings == 0;
        mNativeadrbrowsielShieldsContentSettings = nativePtr;
    }

    @CalledByNative
    private void blockedEvent(int tabId, String block_type, String subresource) {
        for (adrbrowsielShieldsContentSettingsObserver observer : madrbrowsielShieldsContentSettingsObservers) {
            observer.blockEvent(tabId, block_type, subresource);
        }
    }

    @CalledByNative
    private void savedBandwidth(long savings) {
        for (adrbrowsielShieldsContentSettingsObserver observer : madrbrowsielShieldsContentSettingsObservers) {
            observer.savedBandwidth(savings);
        }
    }

    @NativeMethods
    interface Natives {
        void init(adrbrowsielShieldsContentSettings self);
        void destroy(long nativeadrbrowsielShieldsContentSettings);

        void setadrbrowsielShieldsEnabled(boolean enabled, String url, Profile profile);
        boolean getadrbrowsielShieldsEnabled(String url, Profile profile);
        void setAdControlType(String type, String url, Profile profile);
        String getAdControlType(String url, Profile profile);
        void setCookieControlType(String type, String url, Profile profile);
        String getCookieControlType(String url, Profile profile);
        void setFingerprintingControlType(String type, String url, Profile profile);
        String getFingerprintingControlType(String url, Profile profile);
        void setHTTPSEverywhereEnabled(boolean enabled, String url, Profile profile);
        boolean getHTTPSEverywhereEnabled(String url, Profile profile);
        void setNoScriptControlType(String type, String url, Profile profile);
        String getNoScriptControlType(String url, Profile profile);
    }
}
