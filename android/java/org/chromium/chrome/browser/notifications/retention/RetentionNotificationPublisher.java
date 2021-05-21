/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.notifications.retention;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;

import org.chromium.base.ApplicationStatus;
import org.chromium.base.Log;
import org.chromium.base.ThreadUtils;
import org.chromium.base.task.AsyncTask;
import org.chromium.chrome.browser.adrbrowsielAdsNativeHelper;
import org.chromium.chrome.browser.adrbrowsielFeatureList;
import org.chromium.chrome.browser.app.adrbrowsielActivity;
import org.chromium.chrome.browser.adrbrowsiel_stats.adrbrowsielStatsUtil;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.chrome.browser.notifications.adrbrowsielSetDefaultBrowserNotificationService;
import org.chromium.chrome.browser.onboarding.OnboardingPrefManager;
import org.chromium.chrome.browser.preferences.adrbrowsielPref;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.tab.TabLaunchType;
import org.chromium.components.embedder_support.util.UrlConstants;
import org.chromium.components.embedder_support.util.UrlUtilities;
import org.chromium.components.user_prefs.UserPrefs;

public class RetentionNotificationPublisher extends BroadcastReceiver {
    private static final String NOTIFICATION_CHANNEL_NAME = "adrbrowsiel";
    public static final String RETENTION_NOTIFICATION_ACTION = "retention_notification_action";

    @Override
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();
        String notificationType = intent.getStringExtra(RetentionNotificationUtil.NOTIFICATION_TYPE);
        adrbrowsielActivity adrbrowsielActivity = adrbrowsielActivity.getadrbrowsielActivity();
        if (action != null && action.equals(RETENTION_NOTIFICATION_ACTION)) {
            if (adrbrowsielActivity != null) {
                Intent launchIntent = new Intent(Intent.ACTION_MAIN);
                launchIntent.setPackage(context.getPackageName());
                launchIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                context.startActivity(launchIntent);
                switch (notificationType) {
                case RetentionNotificationUtil.HOUR_3:
                case RetentionNotificationUtil.HOUR_24:
                case RetentionNotificationUtil.EVERY_SUNDAY:
                    adrbrowsielActivity.checkForadrbrowsielStats();
                    break;
                case RetentionNotificationUtil.DAY_6:
                case RetentionNotificationUtil.adrbrowsiel_STATS_ADS_TRACKERS:
                case RetentionNotificationUtil.adrbrowsiel_STATS_DATA:
                case RetentionNotificationUtil.adrbrowsiel_STATS_TIME:
                    if (adrbrowsielActivity.getActivityTab() != null 
                        && adrbrowsielActivity.getActivityTab().getUrlString() != null
                        && !UrlUtilities.isNTPUrl(adrbrowsielActivity.getActivityTab().getUrlString())) {
                        adrbrowsielActivity.getTabCreator(false).launchUrl(UrlConstants.NTP_URL, TabLaunchType.FROM_CHROME_UI);
                    }
                    break;
                case RetentionNotificationUtil.DAY_10:
                case RetentionNotificationUtil.DAY_30:
                case RetentionNotificationUtil.DAY_35:
                    adrbrowsielActivity.openRewardsPanel();
                    break;
                }
            } else {
                backgroundNotificationAction(context, intent);
            }
        } else {
            switch (notificationType) {
            case RetentionNotificationUtil.HOUR_3:
            case RetentionNotificationUtil.HOUR_24:
            case RetentionNotificationUtil.DAY_6:
            case RetentionNotificationUtil.adrbrowsiel_STATS_ADS_TRACKERS:
            case RetentionNotificationUtil.adrbrowsiel_STATS_DATA:
            case RetentionNotificationUtil.adrbrowsiel_STATS_TIME:
                createNotification(context, intent);
                break;
            case RetentionNotificationUtil.DEFAULT_BROWSER_1:
            case RetentionNotificationUtil.DEFAULT_BROWSER_2:
            case RetentionNotificationUtil.DEFAULT_BROWSER_3:
                if (!adrbrowsielSetDefaultBrowserNotificationService.isadrbrowsielSetAsDefaultBrowser(context)) {
                    createNotification(context, intent);
                }
                break;
            case RetentionNotificationUtil.DAY_10:
            case RetentionNotificationUtil.DAY_30:
            case RetentionNotificationUtil.DAY_35:
                // Can't check for rewards code in background
                if (adrbrowsielActivity != null
                        && !adrbrowsielAdsNativeHelper.nativeIsadrbrowsielAdsEnabled(
                                Profile.getLastUsedRegularProfile())
                        && ChromeFeatureList.isEnabled(adrbrowsielFeatureList.adrbrowsiel_REWARDS)) {
                    createNotification(context, intent);
                }
                break;
            case RetentionNotificationUtil.EVERY_SUNDAY:
                if (OnboardingPrefManager.getInstance().isadrbrowsielStatsNotificationEnabled()) {
                    createNotification(context, intent);
                }
                break;
            }
        }
    }

    private void createNotification(Context context, Intent intent) {
        final String notificationType =
                intent.getStringExtra(RetentionNotificationUtil.NOTIFICATION_TYPE);
        final RetentionNotification retentionNotification =
                RetentionNotificationUtil.getNotificationObject(notificationType);
        new AsyncTask<Void>() {
            String notificationText = "";
            @Override
            protected Void doInBackground() {
                notificationText =
                        RetentionNotificationUtil.getNotificationText(context, notificationType);
                return null;
            }

            @Override
            protected void onPostExecute(Void result) {
                assert ThreadUtils.runningOnUiThread();
                if (isCancelled()) return;
                NotificationManager notificationManager =
                        (NotificationManager) context.getSystemService(
                                Context.NOTIFICATION_SERVICE);
                Log.e("NTP", "Notification : " + notificationType);
                Notification notification = RetentionNotificationUtil.getNotification(
                        context, notificationType, notificationText);
                if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
                    int importance = NotificationManager.IMPORTANCE_HIGH;
                    NotificationChannel notificationChannel =
                            new NotificationChannel(retentionNotification.getChannelId(),
                                    NOTIFICATION_CHANNEL_NAME, importance);
                    assert notificationManager != null;
                    notificationManager.createNotificationChannel(notificationChannel);
                }
                assert notificationManager != null;
                notificationManager.notify(retentionNotification.getNotificationId(), notification);
            }
        }.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    public static void backgroundNotificationAction(Context context, Intent intent) {
        String notificationType = intent.getStringExtra(RetentionNotificationUtil.NOTIFICATION_TYPE);
        if (ApplicationStatus.hasVisibleActivities()) {
            return;
        }
        Intent launchIntent =
            context.getPackageManager().getLaunchIntentForPackage(context.getPackageName());
        if (launchIntent != null) {
            launchIntent.setFlags(
                Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_RESET_TASK_IF_NEEDED);
            launchIntent.putExtra(RetentionNotificationUtil.NOTIFICATION_TYPE, notificationType);
            context.startActivity(launchIntent);
        }
    }
}
