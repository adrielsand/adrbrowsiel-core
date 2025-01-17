/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/sync/engine/adrbrowsiel_model_type_worker.h"

#include <utility>

#include "base/feature_list.h"
#include "base/logging.h"
#include "base/metrics/histogram_functions.h"
#include "components/sync/engine/model_type_processor.h"

namespace syncer {

namespace features {

// Enables the option of resetting progress marker.
const base::Feature kadrbrowsielSyncResetProgressMarker{
    "ResetProgressMarkerOnCommitFailures", base::FEATURE_ENABLED_BY_DEFAULT};

}  // namespace features

namespace {
// Between each failed commit the timeout is randomly increased,
// see |BackoffDelayProvider|.
// 7 attemps gives near 2..5 minutes before resetting progress marker
// and new get updates

size_t kFailuresToResetMarker = 7;
// Allow reset progress marker for type not often than once in 30 minutes
base::TimeDelta kMinimalTimeBetweenResetMarker =
    base::TimeDelta::FromMinutes(30);
}  // namespace

adrbrowsielModelTypeWorker::adrbrowsielModelTypeWorker(
    ModelType type,
    const sync_pb::ModelTypeState& initial_state,
    bool trigger_initial_sync,
    Cryptographer* cryptographer,
    PassphraseType passphrase_type,
    NudgeHandler* nudge_handler,
    std::unique_ptr<ModelTypeProcessor> model_type_processor,
    CancelationSignal* cancelation_signal)
    : ModelTypeWorker(type,
                      initial_state,
                      trigger_initial_sync,
                      cryptographer,
                      passphrase_type,
                      nudge_handler,
                      std::move(model_type_processor),
                      cancelation_signal) {}

adrbrowsielModelTypeWorker::~adrbrowsielModelTypeWorker() {}

void adrbrowsielModelTypeWorker::OnCommitResponse(
    const CommitResponseDataList& committed_response_list,
    const FailedCommitResponseDataList& error_response_list) {
  ModelTypeWorker::OnCommitResponse(committed_response_list,
                                    error_response_list);

  if (!base::FeatureList::IsEnabled(features::kadrbrowsielSyncResetProgressMarker)) {
    return;
  }

  if (IsResetProgressMarkerRequired(error_response_list)) {
    ResetProgressMarker();
  }
}

// static
size_t adrbrowsielModelTypeWorker::GetFailuresToResetMarkerForTests() {
  return kFailuresToResetMarker;
}

// static
base::TimeDelta adrbrowsielModelTypeWorker::MinimalTimeBetweenResetForTests() {
  return kMinimalTimeBetweenResetMarker;
}

bool adrbrowsielModelTypeWorker::IsResetProgressMarkerRequired(
    const FailedCommitResponseDataList& error_response_list) {
  if (!last_reset_marker_time_.is_null() &&
      base::Time::Now() - last_reset_marker_time_ <
          kMinimalTimeBetweenResetMarker) {
    // Reset progress marker due to 7th failure happening twice in a row
    // in less than 30mins
    // P3A sample is 1
    base::UmaHistogramExactLinear("adrbrowsiel.Sync.ProgressTokenEverReset", 1, 1);
    return false;
  }

  bool found_conflict_or_transient = false;
  for (const syncer::FailedCommitResponseData& failed_response_entry :
       error_response_list) {
    if (failed_response_entry.response_type ==
            sync_pb::CommitResponse_ResponseType_CONFLICT ||
        failed_response_entry.response_type ==
            sync_pb::CommitResponse_ResponseType_TRANSIENT_ERROR) {
      found_conflict_or_transient = true;
      break;
    }
  }

  if (found_conflict_or_transient) {
    ++failed_commit_times_;
  } else {
    failed_commit_times_ = 0;
  }

  return failed_commit_times_ >= kFailuresToResetMarker;
}

void adrbrowsielModelTypeWorker::ResetProgressMarker() {
  VLOG(1) << "Reset progress marker for type " << ModelTypeToString(type_);
  // Normal reset of progress marker due to 7th failure
  // P3A sample is 0
  base::UmaHistogramExactLinear("adrbrowsiel.Sync.ProgressTokenEverReset", 0, 1);
  last_reset_marker_time_ = base::Time::Now();
  model_type_state_.mutable_progress_marker()->clear_token();
}

}  // namespace syncer
