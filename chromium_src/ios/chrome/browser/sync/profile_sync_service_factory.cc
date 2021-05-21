/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/sync/adrbrowsiel_profile_sync_service_delegate.h"
#include "adrbrowsiel/components/sync/driver/adrbrowsiel_sync_profile_sync_service.h"
#include "ios/chrome/browser/sync/device_info_sync_service_factory.h"

#define adrbrowsiel_BUILD_SERVICE_INSTANCE_FOR             \
  std::make_unique<syncer::adrbrowsielProfileSyncService>( \
      std::move(init_params),                        \
      std::make_unique<syncer::adrbrowsielProfileSyncServiceDelegate>( \
          DeviceInfoSyncServiceFactory::GetForBrowserState(browser_state)));

#include "../../../../../../ios/chrome/browser/sync/profile_sync_service_factory.cc"

#undef adrbrowsiel_BUILD_SERVICE_INSTANCE_FOR
