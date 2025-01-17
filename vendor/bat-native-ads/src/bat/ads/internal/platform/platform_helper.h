/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_PLATFORM_PLATFORM_HELPER_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_PLATFORM_PLATFORM_HELPER_H_

#include <string>

#include "base/memory/singleton.h"

namespace ads {

enum class PlatformType { kUnknown, kAndroid, kIOS, kLinux, kMacOS, kWindows };

class PlatformHelper {
 public:
  PlatformHelper(const PlatformHelper&) = delete;
  PlatformHelper& operator=(const PlatformHelper&) = delete;

  static PlatformHelper* GetInstance();

  void set_for_testing(PlatformHelper* platform_helper);

  virtual bool IsMobile() const;
  virtual std::string GetPlatformName() const;
  virtual PlatformType GetPlatform() const;

 protected:
  friend struct base::DefaultSingletonTraits<PlatformHelper>;

  PlatformHelper();
  virtual ~PlatformHelper();

  static PlatformHelper* GetInstanceImpl();
};

}  // namespace ads

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_PLATFORM_PLATFORM_HELPER_H_
