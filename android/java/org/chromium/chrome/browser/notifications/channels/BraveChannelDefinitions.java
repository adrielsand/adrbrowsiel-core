/**
 * Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.notifications.channels;

import android.app.NotificationManager;

import org.chromium.chrome.R;
import org.chromium.components.browser_ui.notifications.channels.ChannelDefinitions;
import org.chromium.components.browser_ui.notifications.channels.ChannelDefinitions.PredefinedChannel;

import java.util.Map;
import java.util.Set;

public class adrbrowsielChannelDefinitions {
    public class ChannelId {
        public static final String adrbrowsiel_ADS = "com.adrbrowsiel.browser.ads";
        public static final String adrbrowsiel_ADS_BACKGROUND = "com.adrbrowsiel.browser.ads.background";
        public static final String adrbrowsiel_BROWSER = "com.adrbrowsiel.browser";
    }

    public class ChannelGroupId {
        public static final String adrbrowsiel_ADS = "com.adrbrowsiel.browser.ads";
    }

    static protected void addadrbrowsielChannels(
        Map<String, PredefinedChannel> map, Set<String> startup) {
        map.put(ChannelId.adrbrowsiel_ADS,
                PredefinedChannel.create(ChannelId.adrbrowsiel_ADS,
                                         R.string.notification_category_adrbrowsiel_ads,
                                         NotificationManager.IMPORTANCE_HIGH, ChannelGroupId.adrbrowsiel_ADS));
        startup.add(ChannelId.adrbrowsiel_ADS);

        map.put(ChannelId.adrbrowsiel_ADS_BACKGROUND,
                PredefinedChannel.create(ChannelId.adrbrowsiel_ADS_BACKGROUND,
                                         R.string.notification_category_adrbrowsiel_ads_background,
                                         NotificationManager.IMPORTANCE_LOW, ChannelGroupId.adrbrowsiel_ADS));
        startup.add(ChannelId.adrbrowsiel_ADS_BACKGROUND);
    }

    static protected void addadrbrowsielChannelGroups(
        Map<String, ChannelDefinitions.PredefinedChannelGroup> map) {
        map.put(ChannelGroupId.adrbrowsiel_ADS,
                new ChannelDefinitions.PredefinedChannelGroup(
                    ChannelGroupId.adrbrowsiel_ADS, R.string.notification_category_group_adrbrowsiel_ads));
    }
}
