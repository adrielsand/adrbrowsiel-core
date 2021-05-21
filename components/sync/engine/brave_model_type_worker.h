/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_SYNC_ENGINE_adrbrowsiel_MODEL_TYPE_WORKER_H_
#define adrbrowsiel_COMPONENTS_SYNC_ENGINE_adrbrowsiel_MODEL_TYPE_WORKER_H_

#include <memory>

#include "base/feature_list.h"
#include "components/sync/base/model_type.h"
#include "components/sync/base/passphrase_enums.h"
#include "components/sync/engine/commit_and_get_updates_types.h"
#include "components/sync/engine/model_type_worker.h"

namespace syncer {

class CancelationSignal;
class Cryptographer;
class NudgeHandler;
class ModelTypeProcessor;

namespace features {

extern const base::Feature kadrbrowsielSyncResetProgressMarker;

}  // namespace features

FORWARD_DECLARE_TEST(adrbrowsielModelTypeWorkerTest, ResetProgressMarker);
FORWARD_DECLARE_TEST(adrbrowsielModelTypeWorkerTest, ResetProgressMarkerMaxPeriod);
FORWARD_DECLARE_TEST(adrbrowsielModelTypeWorkerTest,
                     ResetProgressMarkerDisabledFeature);

class adrbrowsielModelTypeWorker : public ModelTypeWorker {
 public:
  adrbrowsielModelTypeWorker(ModelType type,
                       const sync_pb::ModelTypeState& initial_state,
                       bool trigger_initial_sync,
                       Cryptographer* cryptographer,
                       PassphraseType passphrase_type,
                       NudgeHandler* nudge_handler,
                       std::unique_ptr<ModelTypeProcessor> model_type_processor,
                       CancelationSignal* cancelation_signal);
  ~adrbrowsielModelTypeWorker() override;
  adrbrowsielModelTypeWorker(const adrbrowsielModelTypeWorker&) = delete;
  adrbrowsielModelTypeWorker& operator=(const adrbrowsielModelTypeWorker&) = delete;

 private:
  FRIEND_TEST_ALL_PREFIXES(adrbrowsielModelTypeWorkerTest, ResetProgressMarker);
  FRIEND_TEST_ALL_PREFIXES(adrbrowsielModelTypeWorkerTest,
                           ResetProgressMarkerMaxPeriod);
  FRIEND_TEST_ALL_PREFIXES(adrbrowsielModelTypeWorkerTest,
                           ResetProgressMarkerDisabledFeature);

  void OnCommitResponse(
      const CommitResponseDataList& committed_response_list,
      const FailedCommitResponseDataList& error_response_list) override;

  bool IsResetProgressMarkerRequired(
      const FailedCommitResponseDataList& error_response_list);
  void ResetProgressMarker();

  size_t failed_commit_times_ = 0;
  base::Time last_reset_marker_time_;
  static size_t GetFailuresToResetMarkerForTests();
  static base::TimeDelta MinimalTimeBetweenResetForTests();
};

}  // namespace syncer

#endif  // adrbrowsiel_COMPONENTS_SYNC_ENGINE_adrbrowsiel_MODEL_TYPE_WORKER_H_
