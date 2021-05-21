/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_GCM_DRIVER_adrbrowsiel_GCM_CHANNEL_STATUS_H_
#define adrbrowsiel_BROWSER_GCM_DRIVER_adrbrowsiel_GCM_CHANNEL_STATUS_H_

#include "base/supports_user_data.h"

class Profile;

namespace gcm {

class adrbrowsielGCMChannelStatus : public base::SupportsUserData::Data {
 public:
  explicit adrbrowsielGCMChannelStatus(Profile* profile, bool enabled);
  static adrbrowsielGCMChannelStatus* GetForProfile(Profile *profile);

  bool IsGCMEnabled() const;
  void UpdateGCMDriverStatus();

 private:
  Profile* profile_;
  bool gcm_enabled_;
};

}  // namespace gcm

#endif  // adrbrowsiel_BROWSER_GCM_DRIVER_adrbrowsiel_GCM_CHANNEL_STATUS_H_
