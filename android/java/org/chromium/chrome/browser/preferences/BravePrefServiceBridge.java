/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.preferences;

import androidx.annotation.NonNull;

import org.chromium.base.Log;
import org.chromium.base.ThreadUtils;
import org.chromium.base.annotations.JNINamespace;
import org.chromium.base.annotations.NativeMethods;
import org.chromium.chrome.browser.profiles.Profile;

@JNINamespace("chrome::android")
public class adrbrowsielPrefServiceBridge {
    private adrbrowsielPrefServiceBridge() {
    }

    private static adrbrowsielPrefServiceBridge sInstance;

    public static adrbrowsielPrefServiceBridge getInstance() {
        ThreadUtils.assertOnUiThread();
        if (sInstance == null) {
            sInstance = new adrbrowsielPrefServiceBridge();
        }
        return sInstance;
    }

    /**
     * @param whether HTTPSE should be enabled.
     */
    public void setHTTPSEEnabled(boolean enabled) {
        adrbrowsielPrefServiceBridgeJni.get().setHTTPSEEnabled(enabled);
    }

    /**
     * @param whether the IPFS gateway should be enabled.
     */
    public void setIpfsGatewayEnabled(boolean enabled) {
        adrbrowsielPrefServiceBridgeJni.get().setIpfsGatewayEnabled(enabled);
    }

    /**
     * @param whether google login is enabled on third party sites.
     */
    public void setThirdPartyGoogleLoginEnabled(boolean enabled) {
        adrbrowsielPrefServiceBridgeJni.get().setThirdPartyGoogleLoginEnabled(enabled);
    }

    /**
     * @param whether facebook embeds are allowed on third party sites.
     */
    public void setThirdPartyFacebookEmbedEnabled(boolean enabled) {
        adrbrowsielPrefServiceBridgeJni.get().setThirdPartyFacebookEmbedEnabled(enabled);
    }

    /**
     * @param whether twitter embeds are allowed on third party sites.
     */
    public void setThirdPartyTwitterEmbedEnabled(boolean enabled) {
        adrbrowsielPrefServiceBridgeJni.get().setThirdPartyTwitterEmbedEnabled(enabled);
    }

    /**
     * @param whether linkedin embeds are allowed on third party sites.
     */
    public void setThirdPartyLinkedinEmbedEnabled(boolean enabled) {
        adrbrowsielPrefServiceBridgeJni.get().setThirdPartyLinkedinEmbedEnabled(enabled);
    }

    /**
     * @param whether AdBlock should be enabled.
     */
    public void setAdBlockEnabled(boolean enabled) {
        adrbrowsielPrefServiceBridgeJni.get().setAdBlockEnabled(enabled);
    }

    public void setNoScriptControlType(String type) {
        adrbrowsielPrefServiceBridgeJni.get().setNoScriptControlType(type);
    }

    public String getNoScriptControlType() {
        return adrbrowsielPrefServiceBridgeJni.get().getNoScriptControlType();
    }

    public void setCookiesBlockType(String type) {
        adrbrowsielPrefServiceBridgeJni.get().setCookiesBlockType(type);
    }

    public String getCookiesBlockType() {
        return adrbrowsielPrefServiceBridgeJni.get().getCookiesBlockType();
    }

    /**
     * @param whether Fingerprinting Protection should be enabled.
     */

    public void setFingerprintingControlType(String type) {
        adrbrowsielPrefServiceBridgeJni.get().setFingerprintingControlType(type);
    }

    public String getFingerprintingControlType() {
        return adrbrowsielPrefServiceBridgeJni.get().getFingerprintingControlType();
    }

    public void setCosmeticFilteringControlType(int type) {
        adrbrowsielPrefServiceBridgeJni.get().setCosmeticFilteringControlType(type);
    }

    public String getCosmeticFilteringControlType() {
        return adrbrowsielPrefServiceBridgeJni.get().getCosmeticFilteringControlType();
    }

    public void setPlayYTVideoInBrowserEnabled(boolean enabled) {
        adrbrowsielPrefServiceBridgeJni.get().setPlayYTVideoInBrowserEnabled(enabled);
    }

    public boolean getPlayYTVideoInBrowserEnabled() {
        return adrbrowsielPrefServiceBridgeJni.get().getPlayYTVideoInBrowserEnabled();
    }

    public void setDesktopModeEnabled(boolean enabled) {
        adrbrowsielPrefServiceBridgeJni.get().setDesktopModeEnabled(enabled);
    }

    public boolean getDesktopModeEnabled() {
        return adrbrowsielPrefServiceBridgeJni.get().getDesktopModeEnabled();
    }

    public void setBackgroundVideoPlaybackEnabled(boolean enabled) {
        adrbrowsielPrefServiceBridgeJni.get().setBackgroundVideoPlaybackEnabled(enabled);
    }

    public boolean getBackgroundVideoPlaybackEnabled() {
        return adrbrowsielPrefServiceBridgeJni.get().getBackgroundVideoPlaybackEnabled();
    }

    public long getTrackersBlockedCount(Profile profile) {
        return adrbrowsielPrefServiceBridgeJni.get().getTrackersBlockedCount(profile);
    }

    public long getAdsBlockedCount(Profile profile) {
        return adrbrowsielPrefServiceBridgeJni.get().getAdsBlockedCount(profile);
    }

    public long getDataSaved(Profile profile) {
        return adrbrowsielPrefServiceBridgeJni.get().getDataSaved(profile);
    }

    /**
     * @param whether SafetyNet check failed.
     */
    public void setSafetynetCheckFailed(boolean value) {
        adrbrowsielPrefServiceBridgeJni.get().setSafetynetCheckFailed(value);
    }

    public boolean getSafetynetCheckFailed() {
        return adrbrowsielPrefServiceBridgeJni.get().getSafetynetCheckFailed();
    }

    public void setSafetynetStatus(String status) {
        adrbrowsielPrefServiceBridgeJni.get().setSafetynetStatus(status);
    }

    public void setUseRewardsStagingServer(boolean enabled) {
        adrbrowsielPrefServiceBridgeJni.get().setUseRewardsStagingServer(enabled);
    }

    public void resetPromotionLastFetchStamp() {
        adrbrowsielPrefServiceBridgeJni.get().resetPromotionLastFetchStamp();
    }

    public boolean getUseRewardsStagingServer() {
        return adrbrowsielPrefServiceBridgeJni.get().getUseRewardsStagingServer();
    }

    public void setOldTrackersBlockedCount(Profile profile, long count) {
        adrbrowsielPrefServiceBridgeJni.get().setOldTrackersBlockedCount(profile, count);
    }

    public void setOldAdsBlockedCount(Profile profile, long count) {
        adrbrowsielPrefServiceBridgeJni.get().setOldAdsBlockedCount(profile, count);
    }

    public void setOldHttpsUpgradesCount(Profile profile, long count) {
        adrbrowsielPrefServiceBridgeJni.get().setOldHttpsUpgradesCount(profile, count);
    }

    public boolean GetBooleanForContentSetting(int content_type) {
        return adrbrowsielPrefServiceBridgeJni.get().getBooleanForContentSetting(content_type);
    }

    public void setReferralAndroidFirstRunTimestamp(long time) {
        adrbrowsielPrefServiceBridgeJni.get().setReferralAndroidFirstRunTimestamp(time);
    }

    public void setReferralCheckedForPromoCodeFile(boolean value) {
        adrbrowsielPrefServiceBridgeJni.get().setReferralCheckedForPromoCodeFile(value);
    }

    public void setReferralInitialization(boolean value) {
        adrbrowsielPrefServiceBridgeJni.get().setReferralInitialization(value);
    }

    public void setReferralPromoCode(String promoCode) {
        adrbrowsielPrefServiceBridgeJni.get().setReferralPromoCode(promoCode);
    }

    public void setReferralDownloadId(String downloadId) {
        adrbrowsielPrefServiceBridgeJni.get().setReferralDownloadId(downloadId);
    }

    public void setP3AEnabled(boolean value) {
        adrbrowsielPrefServiceBridgeJni.get().setP3AEnabled(value);
    }

    public boolean getP3AEnabled() {
        return adrbrowsielPrefServiceBridgeJni.get().getP3AEnabled();
    }

    public boolean hasPathP3AEnabled() {
        return adrbrowsielPrefServiceBridgeJni.get().hasPathP3AEnabled();
    }

    public void setP3ANoticeAcknowledged(boolean value) {
        adrbrowsielPrefServiceBridgeJni.get().setP3ANoticeAcknowledged(value);
    }

    public boolean getP3ANoticeAcknowledged() {
        return adrbrowsielPrefServiceBridgeJni.get().getP3ANoticeAcknowledged();
    }

    public void setUnstoppableDomainsResolveMethod(int method) {
        adrbrowsielPrefServiceBridgeJni.get().setUnstoppableDomainsResolveMethod(method);
    }

    public int getUnstoppableDomainsResolveMethod() {
        return adrbrowsielPrefServiceBridgeJni.get().getUnstoppableDomainsResolveMethod();
    }

    public void setENSResolveMethod(int method) {
        adrbrowsielPrefServiceBridgeJni.get().setENSResolveMethod(method);
    }

    public int getENSResolveMethod() {
        return adrbrowsielPrefServiceBridgeJni.get().getENSResolveMethod();
    }

    public void setWebrtcPolicy(int policy) {
        adrbrowsielPrefServiceBridgeJni.get().setWebrtcPolicy(policy);
    }

    public int getWebrtcPolicy() {
        return adrbrowsielPrefServiceBridgeJni.get().getWebrtcPolicy();
    }

    @NativeMethods
    interface Natives {
        void setCosmeticFilteringControlType(int type);
        String getCosmeticFilteringControlType();

        void setCookiesBlockType(String type);
        String getCookiesBlockType();

        void setFingerprintingControlType(String type);
        String getFingerprintingControlType();

        void setNoScriptControlType(String type);
        String getNoScriptControlType();

        void setHTTPSEEnabled(boolean enabled);
        void setIpfsGatewayEnabled(boolean enabled);
        void setAdBlockEnabled(boolean enabled);

        void setThirdPartyGoogleLoginEnabled(boolean enabled);
        void setThirdPartyFacebookEmbedEnabled(boolean enabled);
        void setThirdPartyTwitterEmbedEnabled(boolean enabled);
        void setThirdPartyLinkedinEmbedEnabled(boolean enabled);

        void setPlayYTVideoInBrowserEnabled(boolean enabled);
        boolean getPlayYTVideoInBrowserEnabled();

        void setDesktopModeEnabled(boolean enabled);
        boolean getDesktopModeEnabled();

        void setBackgroundVideoPlaybackEnabled(boolean enabled);
        boolean getBackgroundVideoPlaybackEnabled();

        long getTrackersBlockedCount(Profile profile);
        long getAdsBlockedCount(Profile profile);
        long getDataSaved(Profile profile);

        // Used to pass total stat from upgrading old tabs based browser
        // to a new core based
        void setOldTrackersBlockedCount(Profile profile, long count);
        void setOldAdsBlockedCount(Profile profile, long count);
        void setOldHttpsUpgradesCount(Profile profile, long count);

        void setSafetynetCheckFailed(boolean value);
        boolean getSafetynetCheckFailed();

        void setSafetynetStatus(String status);

        void setUseRewardsStagingServer(boolean enabled);
        void resetPromotionLastFetchStamp();
        boolean getUseRewardsStagingServer();
        boolean getBooleanForContentSetting(int content_type);

        void setReferralAndroidFirstRunTimestamp(long time);
        void setReferralCheckedForPromoCodeFile(boolean value);
        void setReferralInitialization(boolean value);
        void setReferralPromoCode(String promoCode);
        void setReferralDownloadId(String downloadId);

        void setP3AEnabled(boolean value);
        boolean getP3AEnabled();
        boolean hasPathP3AEnabled();
        void setP3ANoticeAcknowledged(boolean value);
        boolean getP3ANoticeAcknowledged();

        void setUnstoppableDomainsResolveMethod(int method);
        void setENSResolveMethod(int method);
        int getUnstoppableDomainsResolveMethod();
        int getENSResolveMethod();

        void setWebrtcPolicy(int policy);
        int getWebrtcPolicy();
    }
}
