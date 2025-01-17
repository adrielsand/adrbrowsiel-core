/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_USER_ACTIVITY_USER_ACTIVITY_EVENT_INFO_H_
#define adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_USER_ACTIVITY_USER_ACTIVITY_EVENT_INFO_H_

#include <deque>

#include "base/time/time.h"
#include "bat/ads/internal/user_activity/user_activity_event_types.h"

namespace ads {

struct UserActivityEventInfo {
  UserActivityEventInfo();
  UserActivityEventInfo(const UserActivityEventInfo& info);
  ~UserActivityEventInfo();

  bool operator==(const UserActivityEventInfo& rhs) const;
  bool operator!=(const UserActivityEventInfo& rhs) const;

  UserActivityEventType type;
  base::Time time;
};

using UserActivityEvents = std::deque<UserActivityEventInfo>;

}  // namespace ads

#endif  // adrbrowsiel_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_USER_ACTIVITY_USER_ACTIVITY_EVENT_INFO_H_
