/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_P3A_adrbrowsiel_P3A_SCHEDULER_H_
#define adrbrowsiel_COMPONENTS_P3A_adrbrowsiel_P3A_SCHEDULER_H_

#include "base/callback_forward.h"
#include "components/metrics/metrics_scheduler.h"

namespace adrbrowsiel {

class adrbrowsielP3AScheduler : public metrics::MetricsScheduler {
 public:
  explicit adrbrowsielP3AScheduler(
      const base::Closure& upload_callback,
      const base::Callback<base::TimeDelta(void)>& get_interval_callback);
  ~adrbrowsielP3AScheduler() override;

  void UploadFinished(bool ok);

 private:
  // Provides us with the interval between successful uploads.
  base::Callback<base::TimeDelta(void)> get_interval_callback_;

  // Initial time to wait between upload retry attempts.
  const base::TimeDelta initial_backoff_interval_;

  // Time to wait for the next upload attempt if the next one fails.
  base::TimeDelta backoff_interval_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielP3AScheduler);
};

}  // namespace adrbrowsiel

#endif  // adrbrowsiel_COMPONENTS_P3A_adrbrowsiel_P3A_SCHEDULER_H_
