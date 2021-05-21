/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_SYNC_DRIVER_adrbrowsiel_SYNC_STOPPED_REPORTER_H_
#define adrbrowsiel_COMPONENTS_SYNC_DRIVER_adrbrowsiel_SYNC_STOPPED_REPORTER_H_

#include <string>

#include "components/sync/driver/sync_stopped_reporter.h"

namespace syncer {

class adrbrowsielSyncStoppedReporter : public SyncStoppedReporter {
 public:
  adrbrowsielSyncStoppedReporter(
      const GURL& sync_service_url,
      const std::string& user_agent,
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
      ResultCallback callback);
  ~adrbrowsielSyncStoppedReporter() override;

  void ReportSyncStopped(const std::string& access_token,
                         const std::string& cache_guid,
                         const std::string& birthday) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(adrbrowsielSyncStoppedReporter);
};

}  // namespace syncer

#endif  // adrbrowsiel_COMPONENTS_SYNC_DRIVER_adrbrowsiel_SYNC_STOPPED_REPORTER_H_
