/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_adrbrowsiel_ADS_ANDROID_adrbrowsiel_ADS_NATIVE_HELPER_H_
#define adrbrowsiel_BROWSER_adrbrowsiel_ADS_ANDROID_adrbrowsiel_ADS_NATIVE_HELPER_H_

#include "base/android/scoped_java_ref.h"

namespace adrbrowsiel_ads {

static jboolean JNI_adrbrowsielAdsNativeHelper_IsadrbrowsielAdsEnabled(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android);

static void JNI_adrbrowsielAdsNativeHelper_SetAdsEnabled(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android);

static jboolean JNI_adrbrowsielAdsNativeHelper_IsNewlySupportedLocale(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android);

static jboolean JNI_adrbrowsielAdsNativeHelper_IsSupportedLocale(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android);

static void JNI_adrbrowsielAdsNativeHelper_OnShowAdNotification(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android,
    const base::android::JavaParamRef<jstring>& j_notification_id);

static void JNI_adrbrowsielAdsNativeHelper_OnCloseAdNotification(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android,
    const base::android::JavaParamRef<jstring>& j_notification_id,
    jboolean j_by_user);

static void JNI_adrbrowsielAdsNativeHelper_OnClickAdNotification(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android,
    const base::android::JavaParamRef<jstring>& j_notification_id);

}  // namespace adrbrowsiel_ads

#endif  // adrbrowsiel_BROWSER_adrbrowsiel_ADS_ANDROID_adrbrowsiel_ADS_NATIVE_HELPER_H_
