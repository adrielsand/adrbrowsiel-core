/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <string>

#include "base/android/jni_android.h"
#include "adrbrowsiel/browser/adrbrowsiel_browser_process.h"
#include "adrbrowsiel/browser/adrbrowsiel_stats/adrbrowsiel_stats_updater.h"
#include "adrbrowsiel/build/android/jni_headers/adrbrowsielActivity_jni.h"

namespace chrome {
namespace android {

static void JNI_adrbrowsielActivity_RestartStatsUpdater(JNIEnv* env) {
  g_adrbrowsiel_browser_process->adrbrowsiel_stats_updater()->Stop();
  g_adrbrowsiel_browser_process->adrbrowsiel_stats_updater()->Start();
}

}  // namespace android
}  // namespace chrome
