/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_SAFETYNET_SAFETYNET_CHECK_H_
#define adrbrowsiel_COMPONENTS_SAFETYNET_SAFETYNET_CHECK_H_

#include <jni.h>
#include <memory>
#include <string>
#include <vector>

#include "base/android/scoped_java_ref.h"
#include "net/base/completion_once_callback.h"

namespace safetynet_check {

class SafetyNetCheckRunner;
using ClientAttestationCallback =
  base::OnceCallback<void(const bool, const std::string&, const bool)>;

class SafetyNetCheck {
 public:
    explicit SafetyNetCheck(SafetyNetCheckRunner* runner);
    ~SafetyNetCheck();
    // Performs client attestation, called from C++
    bool clientAttestation(const std::string& nonce,
      ClientAttestationCallback attest_callback,
      const bool perform_attestation_on_client);
    // Callback returns client attestation final result, called from Java
    void ClientAttestationResult(
        JNIEnv* env,
        jboolean token_received,
        const base::android::JavaParamRef<jstring>& jresult_string,
        jboolean attestation_passed);
    friend class SafetyNetCheckRunner;
 private:
    base::android::ScopedJavaGlobalRef<jobject> java_obj_;
    ClientAttestationCallback attest_callback_;
    SafetyNetCheckRunner* runner_;

    DISALLOW_COPY_AND_ASSIGN(SafetyNetCheck);
};

class SafetyNetCheckRunner {
 public:
    SafetyNetCheckRunner();
    ~SafetyNetCheckRunner();
    void performSafetynetCheck(const std::string& nonce,
      ClientAttestationCallback attest_callback,
      const bool perform_attestation_on_client = false);
    void jobFinished(SafetyNetCheck* finished_job);
 private:
    std::vector<std::unique_ptr<SafetyNetCheck>> jobs_;

    DISALLOW_COPY_AND_ASSIGN(SafetyNetCheckRunner);
};

}  // namespace safetynet_check

#endif  // adrbrowsiel_COMPONENTS_SAFETYNET_SAFETYNET_CHECK_H_
