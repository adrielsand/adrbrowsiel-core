/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/adrbrowsiel_ads/android/adrbrowsiel_ads_native_helper.h"

#include <string>

#include "base/android/jni_string.h"
#include "adrbrowsiel/browser/adrbrowsiel_ads/ads_service_factory.h"
#include "adrbrowsiel/browser/adrbrowsiel_ads/android/jni_headers/adrbrowsielAdsNativeHelper_jni.h"
#include "adrbrowsiel/browser/adrbrowsiel_rewards/rewards_service_factory.h"
#include "chrome/browser/profiles/profile_android.h"

namespace adrbrowsiel_ads {

// static
jboolean JNI_adrbrowsielAdsNativeHelper_IsadrbrowsielAdsEnabled(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile_android);
  AdsService* ads_service = AdsServiceFactory::GetForProfile(profile);
  if (!ads_service) {
    return false;
  }

  return ads_service->IsEnabled();
}

// static
void JNI_adrbrowsielAdsNativeHelper_SetAdsEnabled(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile_android);
  adrbrowsiel_rewards::RewardsService* rewards_service =
      adrbrowsiel_rewards::RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return;
  }

  rewards_service->SetAdsEnabled(true);
}

// static
jboolean JNI_adrbrowsielAdsNativeHelper_IsNewlySupportedLocale(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile_android);
  AdsService* ads_service = AdsServiceFactory::GetForProfile(profile);
  if (!ads_service) {
    return false;
  }

  return ads_service->IsNewlySupportedLocale();
}

// static
jboolean JNI_adrbrowsielAdsNativeHelper_IsSupportedLocale(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile_android);
  AdsService* ads_service = AdsServiceFactory::GetForProfile(profile);
  if (!ads_service) {
    return false;
  }

  return ads_service->IsSupportedLocale();
}

// static
void JNI_adrbrowsielAdsNativeHelper_OnShowAdNotification(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android,
    const base::android::JavaParamRef<jstring>& j_notification_id) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile_android);
  AdsService* ads_service = AdsServiceFactory::GetForProfile(profile);
  if (!ads_service) {
    return;
  }

  const std::string notification_id =
      base::android::ConvertJavaStringToUTF8(env, j_notification_id);
  ads_service->OnShowAdNotification(notification_id);
}

// static
void JNI_adrbrowsielAdsNativeHelper_OnCloseAdNotification(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android,
    const base::android::JavaParamRef<jstring>& j_notification_id,
    jboolean j_by_user) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile_android);
  AdsService* ads_service = AdsServiceFactory::GetForProfile(profile);
  if (!ads_service) {
    return;
  }

  const std::string notification_id =
      base::android::ConvertJavaStringToUTF8(env, j_notification_id);
  ads_service->OnCloseAdNotification(notification_id, j_by_user);
}

// static
void JNI_adrbrowsielAdsNativeHelper_OnClickAdNotification(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile_android,
    const base::android::JavaParamRef<jstring>& j_notification_id) {
  Profile* profile = ProfileAndroid::FromProfileAndroid(j_profile_android);
  AdsService* ads_service = AdsServiceFactory::GetForProfile(profile);
  if (!ads_service) {
    return;
  }

  const std::string notification_id =
      base::android::ConvertJavaStringToUTF8(env, j_notification_id);
  ads_service->OnClickAdNotification(notification_id);
}

}  // namespace adrbrowsiel_ads
