/**
 * Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.notifications;

import android.app.Notification;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;

import org.chromium.base.ContextUtils;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.app.adrbrowsielActivity;
import org.chromium.chrome.browser.dialogs.adrbrowsielAdsNotificationDialog;
import org.chromium.chrome.browser.notifications.adrbrowsielAdsNotificationBuilder;
import org.chromium.chrome.browser.notifications.NotificationBuilderBase;
import org.chromium.chrome.browser.notifications.NotificationUmaTracker;
import org.chromium.chrome.browser.notifications.retention.RetentionNotificationPublisher;
import org.chromium.chrome.browser.notifications.retention.RetentionNotificationUtil;
import org.chromium.components.browser_ui.notifications.NotificationManagerProxyImpl;
import org.chromium.components.browser_ui.notifications.NotificationMetadata;
import org.chromium.components.browser_ui.notifications.NotificationWrapper;
import org.chromium.components.browser_ui.notifications.PendingIntentProvider;

import java.util.Locale;

public class adrbrowsielOnboardingNotification extends BroadcastReceiver {
    public Context mContext;
    private Intent mIntent;

    private static final int adrbrowsiel_ONBOARDING_NOTIFICATION_ID = -2;
    public static String adrbrowsiel_ONBOARDING_NOTIFICATION_TAG = "adrbrowsiel_onboarding_notification_tag";
    private static String adrbrowsiel_ONBOARDING_ORIGIN_EN = "https://adrbrowsiel.com/my-first-ad/";
    private static String adrbrowsiel_ONBOARDING_ORIGIN_DE = "https://adrbrowsiel.com/de/my-first-ad/";
    private static String adrbrowsiel_ONBOARDING_ORIGIN_FR = "https://adrbrowsiel.com/fr/my-first-ad/";
    public static final String DEEP_LINK = "deep_link";
    public static final String USE_CUSTOM_NOTIFICATION = "use_custom_notification";

    private static final String COUNTRY_CODE_DE = "de_DE";
    private static final String COUNTRY_CODE_FR = "fr_FR";

    public static void showOnboardingDialog() {
        adrbrowsielActivity adrbrowsielActivity = adrbrowsielActivity.getadrbrowsielActivity();
        if (adrbrowsielActivity != null) {
            adrbrowsielAdsNotificationDialog.showAdNotification(adrbrowsielActivity,
                    adrbrowsiel_ONBOARDING_NOTIFICATION_TAG, getNotificationUrl(),
                    adrbrowsielActivity.getString(R.string.adrbrowsiel_ui_adrbrowsiel_rewards),
                    adrbrowsielActivity.getString(R.string.this_is_your_first_ad));
        }
    }

    public static void showOnboardingNotification() {
        Context context = ContextUtils.getApplicationContext();
        if (context == null) return;
        NotificationManagerProxyImpl notificationManager =
            new NotificationManagerProxyImpl(context);

        NotificationBuilderBase notificationBuilder =
            new adrbrowsielAdsNotificationBuilder(context)
        .setTitle(context.getString(R.string.adrbrowsiel_ui_adrbrowsiel_rewards))
        .setBody(context.getString(R.string.this_is_your_first_ad))
        .setSmallIconId(R.drawable.ic_chrome)
        .setPriority(Notification.PRIORITY_HIGH)
        .setDefaults(Notification.DEFAULT_ALL)
        .setContentIntent(getDeepLinkIntent(context))
        .setOrigin(getNotificationUrl());

        NotificationWrapper notification = notificationBuilder.build(new NotificationMetadata(
                                              NotificationUmaTracker.SystemNotificationType
                                              .UNKNOWN /* Underlying code doesn't track UNKNOWN */,
                                              adrbrowsiel_ONBOARDING_NOTIFICATION_TAG /* notificationTag */,
                                              adrbrowsiel_ONBOARDING_NOTIFICATION_ID /* notificationId */
                                          ));
        notificationManager.notify(notification);
    }

    public static PendingIntentProvider getDeepLinkIntent(Context context) {
        Intent intent = new Intent(context, adrbrowsielOnboardingNotification.class);
        intent.setAction(DEEP_LINK);
        return new PendingIntentProvider(
                PendingIntent.getBroadcast(context, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT),
                0, 0);
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();
        adrbrowsielActivity adrbrowsielActivity = adrbrowsielActivity.getadrbrowsielActivity();
        if (action != null && action.equals(DEEP_LINK)) {
            if (adrbrowsielActivity != null) {
                adrbrowsielActivity.openRewardsPanel();
                Intent launchIntent = new Intent(Intent.ACTION_MAIN);
                launchIntent.setPackage(context.getPackageName());
                launchIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                context.startActivity(launchIntent);
            } else {
                intent.putExtra(RetentionNotificationUtil.NOTIFICATION_TYPE, RetentionNotificationUtil.DAY_10);
                RetentionNotificationPublisher.backgroundNotificationAction(context, intent);
            }
        } else {
            if (intent.getBooleanExtra(USE_CUSTOM_NOTIFICATION, false)) {
                showOnboardingDialog();
            } else {
                showOnboardingNotification();
            }
            if (adrbrowsielActivity != null) {
                adrbrowsielActivity.hideRewardsOnboardingIcon();
            }
        }
    }

    private static String getNotificationUrl() {
        Locale locale = Locale.getDefault();
        switch (locale.toString()) {
        case COUNTRY_CODE_DE:
            return adrbrowsiel_ONBOARDING_ORIGIN_DE;
        case COUNTRY_CODE_FR:
            return adrbrowsiel_ONBOARDING_ORIGIN_FR;
        default:
            return adrbrowsiel_ONBOARDING_ORIGIN_EN;
        }
    }

    public static void cancelOnboardingNotification(Context context) {
        NotificationManagerProxyImpl notificationManager =
            new NotificationManagerProxyImpl(context);
        notificationManager.cancel(
            adrbrowsiel_ONBOARDING_NOTIFICATION_TAG, adrbrowsiel_ONBOARDING_NOTIFICATION_ID);
    }
}
