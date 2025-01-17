/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_SYNC_adrbrowsiel_SYNC_DEVICES_ANDROID_H_
#define adrbrowsiel_BROWSER_SYNC_adrbrowsiel_SYNC_DEVICES_ANDROID_H_

#include <jni.h>

#include "base/android/jni_weak_ref.h"
#include "base/scoped_observer.h"
#include "base/values.h"
#include "chrome/browser/sync/profile_sync_service_android.h"
#include "components/sync_device_info/device_info_tracker.h"

namespace syncer {
class adrbrowsielProfileSyncService;
}

namespace chrome {
namespace android {

class adrbrowsielSyncDevicesAndroid : public syncer::DeviceInfoTracker::Observer {
 public:
  adrbrowsielSyncDevicesAndroid(JNIEnv* env,
                          const base::android::JavaRef<jobject>& obj);
  virtual ~adrbrowsielSyncDevicesAndroid();

  void Destroy(JNIEnv* env);

  base::android::ScopedJavaLocalRef<jstring> GetSyncDeviceListJson(JNIEnv* env);

  void DeleteDevice(JNIEnv* env,
                    const base::android::JavaParamRef<jstring>& device_guid);

 private:
  // syncer::DeviceInfoTracker::Observer
  void OnDeviceInfoChange() override;

  base::Value GetSyncDeviceList();

  syncer::adrbrowsielProfileSyncService* GetSyncService() const;

  ScopedObserver<syncer::DeviceInfoTracker, syncer::DeviceInfoTracker::Observer>
      device_info_tracker_observer_{this};

  JavaObjectWeakGlobalRef weak_java_adrbrowsiel_sync_worker_;
  Profile* profile_ = nullptr;
};

}  // namespace android
}  // namespace chrome

#endif  // adrbrowsiel_BROWSER_SYNC_adrbrowsiel_SYNC_DEVICES_ANDROID_H_
