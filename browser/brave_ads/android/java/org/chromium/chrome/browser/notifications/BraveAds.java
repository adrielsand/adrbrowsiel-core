/** Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
  * This Source Code Form is subject to the terms of the Mozilla Public
  * License, v. 2.0. If a copy of the MPL was not distributed with this file,
  * You can obtain one at http://mozilla.org/MPL/2.0/.
  */

package org.chromium.chrome.browser.notifications;

import org.chromium.base.annotations.CalledByNative;
import org.chromium.chrome.browser.notifications.channels.adrbrowsielChannelDefinitions;

/**
 * This class provides the adrbrowsiel Ads related methods for the native library
 * (adrbrowsiel/components/adrbrowsiel_ads/browser/notification_helper_android)
 */
public abstract class adrbrowsielAds {
    @CalledByNative
    public static String getadrbrowsielAdsChannelId() {
        return adrbrowsielChannelDefinitions.ChannelId.adrbrowsiel_ADS;
    }

    @CalledByNative
    public static String getadrbrowsielAdsBackgroundChannelId() {
        return adrbrowsielChannelDefinitions.ChannelId.adrbrowsiel_ADS_BACKGROUND;
    }
}
