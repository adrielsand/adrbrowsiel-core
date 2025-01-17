/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/adrbrowsiel_ads/notifications/ad_notification_platform_bridge.h"

#include "base/android/jni_android.h"
#include "base/android/jni_string.h"
#include "adrbrowsiel/browser/adrbrowsiel_ads/android/jni_headers/adrbrowsielAdsNotificationDialog_jni.h"

namespace adrbrowsiel_ads {

AdNotificationPlatformBridge::AdNotificationPlatformBridge(Profile* profile)
    : profile_(profile) {
  DCHECK(profile_);
}

AdNotificationPlatformBridge::~AdNotificationPlatformBridge() = default;

void AdNotificationPlatformBridge::ShowAdNotification(
    AdNotification ad_notification) {
  JNIEnv* env = base::android::AttachCurrentThread();

  const base::android::ScopedJavaLocalRef<jstring> j_notification_id =
      base::android::ConvertUTF8ToJavaString(env, ad_notification.id());
  const base::android::ScopedJavaLocalRef<jstring> j_origin =
      base::android::ConvertUTF8ToJavaString(env, "");
  const base::android::ScopedJavaLocalRef<jstring> title =
      base::android::ConvertUTF16ToJavaString(env, ad_notification.title());
  const base::android::ScopedJavaLocalRef<jstring> body =
      base::android::ConvertUTF16ToJavaString(env, ad_notification.body());

  Java_adrbrowsielAdsNotificationDialog_showAdNotification(env, j_notification_id,
                                                     j_origin, title, body);
}

void AdNotificationPlatformBridge::CloseAdNotification(
    const std::string& notification_id) {
  JNIEnv* env = base::android::AttachCurrentThread();

  base::android::ScopedJavaLocalRef<jstring> j_notification_id =
      base::android::ConvertUTF8ToJavaString(env, notification_id);

  Java_adrbrowsielAdsNotificationDialog_closeAdsNotification(env, j_notification_id);
}

}  // namespace adrbrowsiel_ads
