/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/ios/app/adrbrowsiel_main_delegate.h"

#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/mac/bundle_locations.h"
#include "base/base_paths.h"
#include "base/path_service.h"
#include "components/sync/driver/sync_driver_switches.h"
#include "components/sync/base/model_type.h"
#include "components/browser_sync/browser_sync_switches.h"
#include "components/sync/base/sync_base_switches.h"
#include "ios/chrome/browser/chrome_switches.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

namespace {
const char kadrbrowsielSyncServiceURL[] = adrbrowsiel_SYNC_ENDPOINT;
}  // namespace

adrbrowsielMainDelegate::adrbrowsielMainDelegate() : adrbrowsiel_sync_service_url_(kadrbrowsielSyncServiceURL) {
  base::FilePath path;
  base::PathService::Get(base::DIR_MODULE, &path);
  base::mac::SetOverrideFrameworkBundlePath(path);
  base::mac::SetOverrideOuterBundlePath(path);
}

adrbrowsielMainDelegate::~adrbrowsielMainDelegate() {}

void adrbrowsielMainDelegate::SetSyncServiceURL(const std::string& url) {
  adrbrowsiel_sync_service_url_ = url.empty() ? std::string(kadrbrowsielSyncServiceURL) : url;
}

void adrbrowsielMainDelegate::BasicStartupComplete() {
  auto* command_line(base::CommandLine::ForCurrentProcess());

  syncer::ModelTypeSet disabledTypes = syncer::ModelTypeSet(
    syncer::TYPED_URLS,
    // syncer::PASSWORDS,
    syncer::PROXY_TABS,
    syncer::AUTOFILL,
    // syncer::PREFERENCES,
    syncer::READING_LIST,
    syncer::USER_CONSENTS);

  command_line->RemoveSwitch(switches::kDisableSyncTypes);
  command_line->AppendSwitchASCII(switches::kDisableSyncTypes, syncer::ModelTypeSetToString(disabledTypes));
  command_line->AppendSwitch(switches::kDisableEnterprisePolicy);

  // adrbrowsiel's sync protocol does not use the sync service url
  command_line->AppendSwitchASCII(switches::kSyncServiceURL,
                                 adrbrowsiel_sync_service_url_.c_str());

  IOSChromeMainDelegate::BasicStartupComplete();
}
