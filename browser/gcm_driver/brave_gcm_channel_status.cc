/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/gcm_driver/adrbrowsiel_gcm_channel_status.h"

#include <memory>

#include "adrbrowsiel/common/pref_names.h"
#include "chrome/browser/gcm/gcm_profile_service_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "components/gcm_driver/gcm_driver_desktop.h"
#include "components/gcm_driver/gcm_profile_service.h"
#include "components/prefs/pref_service.h"

namespace gcm {

const char kadrbrowsielGCMStatusKey[] = "adrbrowsiel_gcm_channel_status";

adrbrowsielGCMChannelStatus::adrbrowsielGCMChannelStatus(Profile* profile, bool enabled)
    : profile_(profile), gcm_enabled_(enabled) {}

// static
adrbrowsielGCMChannelStatus* adrbrowsielGCMChannelStatus::GetForProfile(
    Profile* profile) {
  adrbrowsielGCMChannelStatus* status = static_cast<adrbrowsielGCMChannelStatus*>(
      profile->GetUserData(kadrbrowsielGCMStatusKey));

  if (!status) {
    bool enabled = profile->GetPrefs()->GetBoolean(kadrbrowsielGCMChannelStatus);
    // Object cleanup is handled by SupportsUserData
    profile->SetUserData(
        kadrbrowsielGCMStatusKey,
        std::make_unique<adrbrowsielGCMChannelStatus>(profile, enabled));
    status = static_cast<adrbrowsielGCMChannelStatus*>(
        profile->GetUserData(kadrbrowsielGCMStatusKey));
  }
  return status;
}

bool adrbrowsielGCMChannelStatus::IsGCMEnabled() const {
  return gcm_enabled_;
}

void adrbrowsielGCMChannelStatus::UpdateGCMDriverStatus() {
  if (!profile_)
    return;
  gcm::GCMProfileService* gcm_profile_service =
      gcm::GCMProfileServiceFactory::GetForProfile(profile_);
  if (!gcm_profile_service)
    return;
  gcm::GCMDriver* gcm_driver = gcm_profile_service->driver();
  if (!gcm_driver)
    return;
  (static_cast<gcm::adrbrowsielGCMDriverDesktop*>(gcm_driver))
      ->SetEnabled(IsGCMEnabled());
}

}  // namespace gcm
