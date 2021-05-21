/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/adrbrowsiel_ads/browser/notification_helper_android.h"

#include <string>

#include "base/android/jni_string.h"
#include "base/system/sys_info.h"
#include "bat/ads/pref_names.h"
#include "adrbrowsiel/browser/adrbrowsiel_ads/android/jni_headers/adrbrowsielAdsSignupDialog_jni.h"
#include "adrbrowsiel/browser/adrbrowsiel_ads/android/jni_headers/adrbrowsielAds_jni.h"
#include "adrbrowsiel/build/android/jni_headers/adrbrowsielNotificationSettingsBridge_jni.h"
#include "adrbrowsiel/components/adrbrowsiel_ads/browser/background_helper.h"
#include "adrbrowsiel/components/adrbrowsiel_ads/browser/features.h"
#include "chrome/android/chrome_jni_headers/NotificationSystemStatusUtil_jni.h"
#include "chrome/browser/notifications/notification_channels_provider_android.h"

namespace adrbrowsiel_ads {

namespace {

const int kMinimumVersionForNotificationChannels = 8;

const int kAppNotificationStatusUndeterminable = 0;
const int kAppNotificationsStatusEnabled = 2;

}  // namespace

NotificationHelperAndroid::NotificationHelperAndroid() = default;

NotificationHelperAndroid::~NotificationHelperAndroid() = default;

bool NotificationHelperAndroid::ShouldShowNotifications() {
  if (features::ShouldShowCustomAdNotifications()) {
    return true;
  }

  JNIEnv* env = base::android::AttachCurrentThread();
  int status = Java_NotificationSystemStatusUtil_getAppNotificationStatus(env);
  bool is_notifications_enabled =
      (status == kAppNotificationsStatusEnabled ||
       status == kAppNotificationStatusUndeterminable);

  bool is_foreground = BackgroundHelper::GetInstance()->IsForeground();
  bool is_notification_channel_enabled =
      IsadrbrowsielAdsNotificationChannelEnabled(is_foreground);
  bool result = is_notifications_enabled && is_notification_channel_enabled;
  if (!is_foreground) {
    result = result && CanShowBackgroundNotifications();
  }
  return result;
}

bool NotificationHelperAndroid::ShowMyFirstAdNotification() {
  if (!ShouldShowNotifications()) {
    return false;
  }

  const bool should_show_custom_notifications =
      features::ShouldShowCustomAdNotifications();

  JNIEnv* env = base::android::AttachCurrentThread();
  Java_adrbrowsielAdsSignupDialog_enqueueOnboardingNotificationNative(
      env, should_show_custom_notifications);

  return true;
}

bool NotificationHelperAndroid::CanShowBackgroundNotifications() const {
  if (features::ShouldShowCustomAdNotifications()) {
    return true;
  }

  JNIEnv* env = base::android::AttachCurrentThread();
  return Java_adrbrowsielAdsSignupDialog_showAdsInBackground(env);
}

NotificationHelperAndroid* NotificationHelperAndroid::GetInstanceImpl() {
  return base::Singleton<NotificationHelperAndroid>::get();
}

NotificationHelper* NotificationHelper::GetInstanceImpl() {
  return NotificationHelperAndroid::GetInstanceImpl();
}

///////////////////////////////////////////////////////////////////////////////

bool NotificationHelperAndroid::IsadrbrowsielAdsNotificationChannelEnabled(
    bool foreground_channel) const {
  if (GetOperatingSystemVersion() < kMinimumVersionForNotificationChannels) {
    return true;
  }

  JNIEnv* env = base::android::AttachCurrentThread();
  auto j_channel_id = (foreground_channel)
                          ? Java_adrbrowsielAds_getadrbrowsielAdsChannelId(env)
                          : Java_adrbrowsielAds_getadrbrowsielAdsBackgroundChannelId(env);
  auto status = static_cast<NotificationChannelStatus>(
      Java_adrbrowsielNotificationSettingsBridge_getChannelStatus(env, j_channel_id));

  return (status == NotificationChannelStatus::ENABLED ||
          status == NotificationChannelStatus::UNAVAILABLE);
}

int NotificationHelperAndroid::GetOperatingSystemVersion() const {
  int32_t major_version = 0;
  int32_t minor_version = 0;
  int32_t bugfix_version = 0;

  base::SysInfo::OperatingSystemVersionNumbers(&major_version, &minor_version,
                                               &bugfix_version);

  return major_version;
}

}  // namespace adrbrowsiel_ads
