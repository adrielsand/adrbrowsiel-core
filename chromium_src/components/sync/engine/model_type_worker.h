/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_CHROMIUM_SRC_COMPONENTS_SYNC_ENGINE_MODEL_TYPE_WORKER_H_
#define adrbrowsiel_CHROMIUM_SRC_COMPONENTS_SYNC_ENGINE_MODEL_TYPE_WORKER_H_

#include "base/gtest_prod_util.h"

namespace syncer {

FORWARD_DECLARE_TEST(adrbrowsielModelTypeWorkerTest, ResetProgressMarker);
FORWARD_DECLARE_TEST(adrbrowsielModelTypeWorkerTest, ResetProgressMarkerMaxPeriod);

}  // namespace syncer

#define adrbrowsiel_MODEL_TYPE_WORKER_H_                                         \
 private:                                                                  \
  friend class adrbrowsielModelTypeWorker;                                       \
  friend class adrbrowsielModelTypeWorkerTest;                                   \
  FRIEND_TEST_ALL_PREFIXES(adrbrowsielModelTypeWorkerTest, ResetProgressMarker); \
  FRIEND_TEST_ALL_PREFIXES(adrbrowsielModelTypeWorkerTest,                       \
                           ResetProgressMarkerMaxPeriod);

#define OnCommitResponse virtual OnCommitResponse

#include "../../../../../components/sync/engine/model_type_worker.h"

#undef OnCommitResponse
#undef adrbrowsiel_MODEL_TYPE_WORKER_H_

#endif  // adrbrowsiel_CHROMIUM_SRC_COMPONENTS_SYNC_ENGINE_MODEL_TYPE_WORKER_H_
