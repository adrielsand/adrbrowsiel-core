/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/android/signin/signin_manager_android.h"

#include "adrbrowsiel/build/android/jni_headers/adrbrowsielSigninManager_jni.h"
#include "chrome/android/chrome_jni_headers/SigninManagerImpl_jni.h"

namespace {
// For preventing "unused Java_SigninManager_create method" compile error.
class UnusedClass {
 private:
  void test() {
    Java_SigninManagerImpl_create(nullptr, 0ll, nullptr, nullptr, nullptr);
    Java_SigninManagerImpl_destroy(nullptr, 0ll);
  }
};
}  // namespace

#define Java_SigninManagerImpl_create Java_adrbrowsielSigninManager_create
#define Java_SigninManagerImpl_destroy Java_adrbrowsielSigninManager_destroy
#include "../../../../../../chrome/browser/android/signin/signin_manager_android.cc"  // NOLINT
#undef Java_SigninManagerImpl_create
#undef Java_SigninManagerImpl_destroy
