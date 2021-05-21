/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/android/adrbrowsiel_shields_content_settings.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/adrbrowsiel_shields_util.h"

#include <string>

#include "base/android/jni_android.h"
#include "base/android/jni_string.h"
#include "adrbrowsiel/build/android/jni_headers/adrbrowsielShieldsContentSettings_jni.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_android.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "components/content_settings/core/browser/content_settings_utils.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "url/gurl.h"

namespace chrome {
namespace android {


// That class is linked to a global toolbar. It's a one instance on Android
adrbrowsielShieldsContentSettings* g_adrbrowsiel_shields_content_settings = nullptr;

static void JNI_adrbrowsielShieldsContentSettings_Init(JNIEnv* env,
    const base::android::JavaParamRef<jobject>& jcaller) {
  g_adrbrowsiel_shields_content_settings =
      new adrbrowsielShieldsContentSettings(env, jcaller);
}

adrbrowsielShieldsContentSettings::adrbrowsielShieldsContentSettings(
    JNIEnv* env,
    const base::android::JavaRef<jobject>& obj)
    : jobj_(base::android::ScopedJavaGlobalRef<jobject>(obj)) {
  Java_adrbrowsielShieldsContentSettings_setNativePtr(env, obj,
      reinterpret_cast<intptr_t>(this));
}

adrbrowsielShieldsContentSettings::~adrbrowsielShieldsContentSettings() {
}

void adrbrowsielShieldsContentSettings::Destroy(JNIEnv* env) {
  g_adrbrowsiel_shields_content_settings = nullptr;
  delete this;
}

void adrbrowsielShieldsContentSettings::DispatchBlockedEventToJava(int tab_id,
        const std::string& block_type, const std::string& subresource) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_adrbrowsielShieldsContentSettings_blockedEvent(
      env, jobj_, tab_id,
      base::android::ConvertUTF8ToJavaString(env, block_type),
      base::android::ConvertUTF8ToJavaString(env, subresource));
}

void adrbrowsielShieldsContentSettings::DispatchSavedBandwidthToJava(
  uint64_t savings) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_adrbrowsielShieldsContentSettings_savedBandwidth(env, jobj_, savings);
}

void adrbrowsielShieldsContentSettings::DispatchSavedBandwidth(uint64_t savings) {
  DCHECK(g_adrbrowsiel_shields_content_settings);
  if (!g_adrbrowsiel_shields_content_settings) {
    return;
  }
  g_adrbrowsiel_shields_content_settings->DispatchSavedBandwidthToJava(savings);
}

// static
void adrbrowsielShieldsContentSettings::DispatchBlockedEvent(int tab_id,
  const std::string& block_type, const std::string& subresource) {
  DCHECK(g_adrbrowsiel_shields_content_settings);
  if (!g_adrbrowsiel_shields_content_settings) {
    return;
  }
  g_adrbrowsiel_shields_content_settings->DispatchBlockedEventToJava(tab_id,
      block_type, subresource);
}

void JNI_adrbrowsielShieldsContentSettings_SetadrbrowsielShieldsEnabled(JNIEnv* env,
    jboolean enabled,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  adrbrowsiel_shields::SetadrbrowsielShieldsEnabled(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      enabled,
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

jboolean JNI_adrbrowsielShieldsContentSettings_GetadrbrowsielShieldsEnabled(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  return adrbrowsiel_shields::GetadrbrowsielShieldsEnabled(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)));
}

void JNI_adrbrowsielShieldsContentSettings_SetAdControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  adrbrowsiel_shields::SetAdControlType(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      adrbrowsiel_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

base::android::ScopedJavaLocalRef<jstring>
    JNI_adrbrowsielShieldsContentSettings_GetAdControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  adrbrowsiel_shields::ControlType control_type =
      adrbrowsiel_shields::GetAdControlType(
          HostContentSettingsMapFactory::GetForProfile(
              ProfileAndroid::FromProfileAndroid(j_profile)),
          GURL(base::android::ConvertJavaStringToUTF8(env, url)));

  return base::android::ConvertUTF8ToJavaString(env,
      adrbrowsiel_shields::ControlTypeToString(control_type));
}

void JNI_adrbrowsielShieldsContentSettings_SetCookieControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  adrbrowsiel_shields::SetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      adrbrowsiel_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

base::android::ScopedJavaLocalRef<jstring>
    JNI_adrbrowsielShieldsContentSettings_GetCookieControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  adrbrowsiel_shields::ControlType control_type =
      adrbrowsiel_shields::GetCookieControlType(
          HostContentSettingsMapFactory::GetForProfile(
              ProfileAndroid::FromProfileAndroid(j_profile)),
          GURL(base::android::ConvertJavaStringToUTF8(env, url)));

  return base::android::ConvertUTF8ToJavaString(env,
      adrbrowsiel_shields::ControlTypeToString(control_type));
}

void JNI_adrbrowsielShieldsContentSettings_SetFingerprintingControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  adrbrowsiel_shields::SetFingerprintingControlType(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      adrbrowsiel_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

base::android::ScopedJavaLocalRef<jstring>
    JNI_adrbrowsielShieldsContentSettings_GetFingerprintingControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  adrbrowsiel_shields::ControlType control_type =
      adrbrowsiel_shields::GetFingerprintingControlType(
          HostContentSettingsMapFactory::GetForProfile(
              ProfileAndroid::FromProfileAndroid(j_profile)),
          GURL(base::android::ConvertJavaStringToUTF8(env, url)));

  return base::android::ConvertUTF8ToJavaString(env,
      adrbrowsiel_shields::ControlTypeToString(control_type));
}

void JNI_adrbrowsielShieldsContentSettings_SetHTTPSEverywhereEnabled(JNIEnv* env,
    jboolean enabled,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  adrbrowsiel_shields::SetHTTPSEverywhereEnabled(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      enabled,
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

jboolean JNI_adrbrowsielShieldsContentSettings_GetHTTPSEverywhereEnabled(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  return adrbrowsiel_shields::GetHTTPSEverywhereEnabled(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)));
}

void JNI_adrbrowsielShieldsContentSettings_SetNoScriptControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  adrbrowsiel_shields::SetNoScriptControlType(
      HostContentSettingsMapFactory::GetForProfile(
          ProfileAndroid::FromProfileAndroid(j_profile)),
      adrbrowsiel_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

base::android::ScopedJavaLocalRef<jstring>
    JNI_adrbrowsielShieldsContentSettings_GetNoScriptControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  adrbrowsiel_shields::ControlType control_type =
      adrbrowsiel_shields::GetNoScriptControlType(
          HostContentSettingsMapFactory::GetForProfile(
              ProfileAndroid::FromProfileAndroid(j_profile)),
          GURL(base::android::ConvertJavaStringToUTF8(env, url)));

  return base::android::ConvertUTF8ToJavaString(env,
      adrbrowsiel_shields::ControlTypeToString(control_type));
}

}  // namespace android
}  // namespace chrome
