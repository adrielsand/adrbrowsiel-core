/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/sync/adrbrowsiel_sync_devices_android.h"

#include <string>
#include <utility>

#include "base/android/jni_android.h"
#include "base/android/jni_string.h"
#include "base/json/json_writer.h"

#include "adrbrowsiel/build/android/jni_headers/adrbrowsielSyncDevices_jni.h"
#include "adrbrowsiel/components/adrbrowsiel_sync/profile_sync_service_helper.h"
#include "adrbrowsiel/components/sync/driver/adrbrowsiel_sync_profile_sync_service.h"
#include "adrbrowsiel/components/sync_device_info/adrbrowsiel_device_info.h"

#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/browser/sync/device_info_sync_service_factory.h"
#include "chrome/browser/sync/profile_sync_service_factory.h"

#include "components/sync_device_info/device_info_sync_service.h"
#include "components/sync_device_info/device_info_tracker.h"
#include "components/sync_device_info/local_device_info_provider.h"

namespace chrome {
namespace android {

adrbrowsielSyncDevicesAndroid::adrbrowsielSyncDevicesAndroid(
    JNIEnv* env, const base::android::JavaRef<jobject>& obj) :
    weak_java_adrbrowsiel_sync_worker_(env, obj) {
  Java_adrbrowsielSyncDevices_setNativePtr(env, obj,
    reinterpret_cast<intptr_t>(this));

  profile_ =
      ProfileManager::GetActiveUserProfile()->GetOriginalProfile();
  DCHECK_NE(profile_, nullptr);

  syncer::DeviceInfoTracker* tracker =
    DeviceInfoSyncServiceFactory::GetForProfile(profile_)
       ->GetDeviceInfoTracker();
  DCHECK(tracker);
  if (tracker) {
    device_info_tracker_observer_.Add(tracker);
  }
}

adrbrowsielSyncDevicesAndroid::~adrbrowsielSyncDevicesAndroid() {
  // Observer will be removed by ScopedObserver
}

void adrbrowsielSyncDevicesAndroid::Destroy(JNIEnv* env) {
  delete this;
}

void adrbrowsielSyncDevicesAndroid::OnDeviceInfoChange() {
  // Notify Java code
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_adrbrowsielSyncDevices_deviceInfoChanged(env,
      weak_java_adrbrowsiel_sync_worker_.get(env));
}

base::Value adrbrowsielSyncDevicesAndroid::GetSyncDeviceList() {
  auto* device_info_service =
      DeviceInfoSyncServiceFactory::GetForProfile(profile_);
  syncer::DeviceInfoTracker* tracker =
      device_info_service->GetDeviceInfoTracker();
  DCHECK(tracker);
  const syncer::DeviceInfo* local_device_info = device_info_service
     ->GetLocalDeviceInfoProvider()->GetLocalDeviceInfo();

  base::Value device_list(base::Value::Type::LIST);

  for (const auto& device : tracker->GetAlladrbrowsielDeviceInfo()) {
    auto device_value = base::Value::FromUniquePtrValue(device->ToValue());
    bool is_current_device = local_device_info
        ? local_device_info->guid() == device->guid()
        : false;
    device_value.SetBoolKey("isCurrentDevice", is_current_device);
    // DeviceInfo::ToValue doesn't put guid
    device_value.SetStringKey("guid", device->guid());
    device_value.SetBoolKey("supportsSelfDelete",
                            device->is_self_delete_supported());
    device_list.Append(std::move(device_value));
  }

  return device_list;
}

base::android::ScopedJavaLocalRef<jstring>
adrbrowsielSyncDevicesAndroid::GetSyncDeviceListJson(JNIEnv* env) {
  base::Value device_list = GetSyncDeviceList();
  std::string json_string;
  if (!base::JSONWriter::Write(device_list, &json_string)) {
    VLOG(1) << "Writing as JSON failed. Passing empty string to Java code.";
    json_string = std::string();
  }

  return base::android::ConvertUTF8ToJavaString(env, json_string);
}

// TODO(AlexeyBarabash): duplicate with adrbrowsielSyncWorker?
syncer::adrbrowsielProfileSyncService* adrbrowsielSyncDevicesAndroid::GetSyncService()
    const {
  return ProfileSyncServiceFactory::IsSyncAllowed(profile_)
             ? static_cast<syncer::adrbrowsielProfileSyncService*>(
                   ProfileSyncServiceFactory::GetForProfile(profile_))
             : nullptr;
}

void adrbrowsielSyncDevicesAndroid::DeleteDevice(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& device_guid) {
  std::string str_device_guid =
      base::android::ConvertJavaStringToUTF8(device_guid);
  auto* sync_service = GetSyncService();
  DCHECK(sync_service);

  auto* device_info_sync_service =
      DeviceInfoSyncServiceFactory::GetForProfile(profile_);
  DCHECK(device_info_sync_service);

  adrbrowsiel_sync::DeleteDevice(sync_service, device_info_sync_service,
                           str_device_guid);
}

static void JNI_adrbrowsielSyncDevices_Init(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& jcaller) {
  new adrbrowsielSyncDevicesAndroid(env, jcaller);
}

}  // namespace android
}  // namespace chrome
