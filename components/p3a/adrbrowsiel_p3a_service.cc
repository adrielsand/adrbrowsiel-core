/* Copyright 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/p3a/adrbrowsiel_p3a_service.h"

#include <memory>
#include <string>
#include <utility>

#include "base/command_line.h"
#include "base/i18n/timezone.h"
#include "base/metrics/histogram_macros.h"
#include "base/metrics/histogram_samples.h"
#include "base/metrics/metrics_hashes.h"
#include "base/metrics/sample_vector.h"
#include "base/metrics/statistics_recorder.h"
#include "base/no_destructor.h"
#include "base/rand_util.h"
#include "base/strings/string_number_conversions.h"
#include "base/task/post_task.h"
#include "base/trace_event/trace_event.h"
#include "adrbrowsiel/components/adrbrowsiel_prochlo/prochlo_message.pb.h"
#include "adrbrowsiel/components/adrbrowsiel_referrals/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_stats/browser/adrbrowsiel_stats_updater_util.h"
#include "adrbrowsiel/components/p3a/adrbrowsiel_p2a_protocols.h"
#include "adrbrowsiel/components/p3a/adrbrowsiel_p3a_log_store.h"
#include "adrbrowsiel/components/p3a/adrbrowsiel_p3a_scheduler.h"
#include "adrbrowsiel/components/p3a/adrbrowsiel_p3a_switches.h"
#include "adrbrowsiel/components/p3a/adrbrowsiel_p3a_uploader.h"
#include "adrbrowsiel/components/p3a/pref_names.h"
#include "adrbrowsiel/components/version_info/version_info.h"
#include "adrbrowsiel/vendor/adrbrowsiel_base/random.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/browser_task_traits.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"
#include "third_party/metrics_proto/reporting_info.pb.h"

namespace adrbrowsiel {

namespace {

// Receiving this value will effectively prevent the metric from transmission
// to the backend. For now we consider this as a hack for p2a metrics, which
// should be refactored in better times.
constexpr int32_t kSuspendedMetricValue = INT_MAX - 1;
constexpr uint64_t kSuspendedMetricBucket = INT_MAX - 1;

constexpr char kLastRotationTimeStampPref[] = "p3a.last_rotation_timestamp";

constexpr char kP3AServerUrl[] = "https://p3a.adrbrowsiel.com/";
constexpr char kP2AServerUrl[] = "https://p2a.adrbrowsiel.com/";

constexpr uint64_t kDefaultUploadIntervalSeconds = 60;  // 1 minute.

// TODO(iefremov): Provide moar histograms!
// Whitelist for histograms that we collect. Will be replaced with something
// updating on the fly.
// clang-format off
constexpr const char* kCollectedHistograms[] = {
    "adrbrowsiel.Core.BookmarksCountOnProfileLoad.2",
    "adrbrowsiel.Core.CrashReportsEnabled",
    "adrbrowsiel.Core.IsDefault",
    "adrbrowsiel.Core.LastTimeIncognitoUsed",
    "adrbrowsiel.Core.NumberOfExtensions",
    "adrbrowsiel.Core.TabCount",
    "adrbrowsiel.Core.TorEverUsed",
    "adrbrowsiel.Core.WindowCount.2",
    "adrbrowsiel.Importer.ImporterSource",
    "adrbrowsiel.NTP.CustomizeUsageStatus",
    "adrbrowsiel.NTP.NewTabsCreated",
    "adrbrowsiel.NTP.SponsoredImagesEnabled",
    "adrbrowsiel.NTP.SponsoredNewTabsCreated",
    "adrbrowsiel.Omnibox.SearchCount",
    "adrbrowsiel.P3A.SentAnswersCount",
    "adrbrowsiel.Rewards.AdsState.2",
    "adrbrowsiel.Rewards.AutoContributionsState.2",
    "adrbrowsiel.Rewards.TipsState.2",
    "adrbrowsiel.Rewards.WalletBalance.2",
    "adrbrowsiel.Rewards.WalletState",
    "adrbrowsiel.Savings.BandwidthSavingsMB",
    "adrbrowsiel.Search.DefaultEngine.4",
    "adrbrowsiel.Shields.UsageStatus",
    "adrbrowsiel.SpeedReader.Enabled",
    "adrbrowsiel.SpeedReader.ToggleCount",
    "adrbrowsiel.Today.HasEverInteracted",
    "adrbrowsiel.Today.WeeklySessionCount",
    "adrbrowsiel.Today.WeeklyMaxCardViewsCount",
    "adrbrowsiel.Today.WeeklyMaxCardVisitsCount",
    "adrbrowsiel.Sync.Status",
    "adrbrowsiel.Sync.ProgressTokenEverReset",
    "adrbrowsiel.Uptime.BrowserOpenMinutes",
    "adrbrowsiel.Welcome.InteractionStatus",

    // IPFS
    "adrbrowsiel.IPFS.IPFSCompanionInstalled",
    "adrbrowsiel.IPFS.DetectionPromptCount",
    "adrbrowsiel.IPFS.GatewaySetting",
    "adrbrowsiel.IPFS.DaemonRunTime",

    // P2A
    // Ad Opportunities
    "adrbrowsiel.P2A.TotalAdOpportunities",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.architecture",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.artsentertainment",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.automotive",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.business",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.careers",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.cellphones",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.crypto",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.education",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.familyparenting",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.fashion",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.folklore",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.fooddrink",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.gaming",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.healthfitness",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.history",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.hobbiesinterests",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.home",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.law",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.military",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.other",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.personalfinance",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.pets",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.realestate",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.science",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.sports",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.technologycomputing",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.travel",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.weather",
    "adrbrowsiel.P2A.AdOpportunitiesPerSegment.untargeted",
    // Ad Impressions
    "adrbrowsiel.P2A.TotalAdImpressions",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.architecture",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.artsentertainment",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.automotive",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.business",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.careers",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.cellphones",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.crypto",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.education",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.familyparenting",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.fashion",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.folklore",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.fooddrink",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.gaming",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.healthfitness",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.history",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.hobbiesinterests",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.home",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.law",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.military",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.other",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.personalfinance",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.pets",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.realestate",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.science",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.sports",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.technologycomputing",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.travel",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.weather",
    "adrbrowsiel.P2A.AdImpressionsPerSegment.untargeted"
};
// clang-format on

bool IsSuspendedMetric(base::StringPiece metric_name,
                       uint64_t value_or_bucket) {
  return value_or_bucket == kSuspendedMetricBucket;
}

base::TimeDelta GetRandomizedUploadInterval(
    base::TimeDelta average_upload_interval) {
  const auto delta = base::TimeDelta::FromSecondsD(
      adrbrowsiel_base::random::Geometric(average_upload_interval.InSecondsF()));
  return delta;
}

base::TimeDelta TimeDeltaTillMonday(base::Time time) {
  base::Time::Exploded exploded;
  time.LocalMidnight().LocalExplode(&exploded);
  // 1 stands for Monday, 0 for Sunday
  int days_till_monday = 0;
  if (exploded.day_of_week >= 1) {
    days_till_monday = 8 - exploded.day_of_week;
  } else {
    days_till_monday = 1;
  }

  base::TimeDelta result = base::TimeDelta::FromDays(days_till_monday) -
                           (time - time.LocalMidnight());
  return result;
}

}  // namespace

adrbrowsielP3AService::adrbrowsielP3AService(PrefService* local_state,
                                 std::string channel,
                                 std::string week_of_install)
    : local_state_(std::move(local_state)),
      channel_(std::move(channel)),
      week_of_install_(week_of_install) {}

adrbrowsielP3AService::~adrbrowsielP3AService() = default;

void adrbrowsielP3AService::RegisterPrefs(PrefRegistrySimple* registry,
                                    bool first_run) {
  adrbrowsielP3ALogStore::RegisterPrefs(registry);
  registry->RegisterTimePref(kLastRotationTimeStampPref, {});
  registry->RegisterBooleanPref(kP3AEnabled, true);

  // New users are shown the P3A notice via the welcome page.
  registry->RegisterBooleanPref(kP3ANoticeAcknowledged, first_run);
}

void adrbrowsielP3AService::InitCallbacks() {
  for (const char* histogram_name : kCollectedHistograms) {
    base::StatisticsRecorder::SetCallback(
        histogram_name,
        base::BindRepeating(&adrbrowsielP3AService::OnHistogramChanged, this));
  }
}

void adrbrowsielP3AService::Init(
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory) {
  // Init basic prefs.
  initialized_ = true;

  average_upload_interval_ =
      base::TimeDelta::FromSeconds(kDefaultUploadIntervalSeconds);

  upload_server_url_ = GURL(kP3AServerUrl);
  MaybeOverrideSettingsFromCommandLine();

  VLOG(2) << "adrbrowsielP3AService::Init() Done!";
  VLOG(2) << "adrbrowsielP3AService parameters are:"
          << ", average_upload_interval_ = " << average_upload_interval_
          << ", randomize_upload_interval_ = " << randomize_upload_interval_
          << ", upload_server_url_ = " << upload_server_url_.spec()
          << ", rotation_interval_ = " << rotation_interval_;

  InitPyxisMeta();

  // Init log store.
  log_store_.reset(new adrbrowsielP3ALogStore(this, local_state_));
  log_store_->LoadPersistedUnsentLogs();
  // Store values that were recorded between calling constructor and |Init()|.
  for (const auto& entry : histogram_values_) {
    HandleHistogramChange(entry.first.as_string(), entry.second);
  }
  histogram_values_ = {};
  // Do rotation if needed.
  const base::Time last_rotation =
      local_state_->GetTime(kLastRotationTimeStampPref);
  if (last_rotation.is_null()) {
    DoRotation();
  } else {
    const base::TimeDelta last_rotation_interval =
        rotation_interval_.is_zero() ? TimeDeltaTillMonday(last_rotation)
                                     : rotation_interval_;
    if (base::Time::Now() - last_rotation > last_rotation_interval) {
      DoRotation();
    }
  }

  // Init other components.
  uploader_.reset(new adrbrowsielP3AUploader(
      url_loader_factory, upload_server_url_, GURL(kP2AServerUrl),
      base::Bind(&adrbrowsielP3AService::OnLogUploadComplete, this)));

  upload_scheduler_.reset(new adrbrowsielP3AScheduler(
      base::Bind(&adrbrowsielP3AService::StartScheduledUpload, this),
      (randomize_upload_interval_
           ? base::BindRepeating(GetRandomizedUploadInterval,
                                 average_upload_interval_)
           : base::BindRepeating([](base::TimeDelta x) { return x; },
                                 average_upload_interval_))));

  upload_scheduler_->Start();
  if (!rotation_timer_.IsRunning()) {
    UpdateRotationTimer();
  }
}

std::string adrbrowsielP3AService::Serialize(base::StringPiece histogram_name,
                                       uint64_t value) {
  // TRACE_EVENT0("adrbrowsiel_p3a", "SerializeMessage");
  // TODO(iefremov): Maybe we should store it in logs and pass here?
  // We cannot directly query |base::StatisticsRecorder::FindHistogram| because
  // the serialized value can be obtained from persisted log storage at the
  // point when the actual histogram is not ready yet.
  const uint64_t histogram_name_hash = base::HashMetricName(histogram_name);

  // TODO(iefremov): Restore when PROCHLO/PYXIS is ready.
  //  adrbrowsiel_pyxis::PyxisMessage message;
  //  prochlo::GenerateProchloMessage(histogram_name_hash, value, pyxis_meta_,
  //                                  &message);

  UpdatePyxisMeta();
  adrbrowsiel_pyxis::RawP3AValue message;
  prochlo::GenerateP3AMessage(histogram_name_hash, value, pyxis_meta_,
                              &message);
  return message.SerializeAsString();
}

bool
adrbrowsielP3AService::IsActualMetric(base::StringPiece histogram_name) const {
  static const base::NoDestructor<base::flat_set<base::StringPiece>>
      metric_names {std::begin(kCollectedHistograms),
                    std::end(kCollectedHistograms)};
  return metric_names->contains(histogram_name);
}

void adrbrowsielP3AService::MaybeOverrideSettingsFromCommandLine() {
  base::CommandLine* cmdline = base::CommandLine::ForCurrentProcess();

  if (cmdline->HasSwitch(switches::kP3AUploadIntervalSeconds)) {
    std::string seconds_str =
        cmdline->GetSwitchValueASCII(switches::kP3AUploadIntervalSeconds);
    int64_t seconds;
    if (base::StringToInt64(seconds_str, &seconds) && seconds > 0) {
      average_upload_interval_ = base::TimeDelta::FromSeconds(seconds);
    }
  }

  if (cmdline->HasSwitch(switches::kP3ADoNotRandomizeUploadInterval)) {
    randomize_upload_interval_ = false;
  }

  if (cmdline->HasSwitch(switches::kP3ARotationIntervalSeconds)) {
    std::string seconds_str =
        cmdline->GetSwitchValueASCII(switches::kP3ARotationIntervalSeconds);
    int64_t seconds;
    if (base::StringToInt64(seconds_str, &seconds) && seconds > 0) {
      rotation_interval_ = base::TimeDelta::FromSeconds(seconds);
    }
  }

  if (cmdline->HasSwitch(switches::kP3AUploadServerUrl)) {
    GURL url =
        GURL(cmdline->GetSwitchValueASCII(switches::kP3AUploadServerUrl));
    if (url.is_valid()) {
      upload_server_url_ = url;
    }
  }
}

void adrbrowsielP3AService::InitPyxisMeta() {
  pyxis_meta_.platform = adrbrowsiel_stats::GetPlatformIdentifier();
  pyxis_meta_.channel = channel_;
  pyxis_meta_.version =
      version_info::GetadrbrowsielVersionWithoutChromiumMajorVersion();

  if (!week_of_install_.empty()) {
    pyxis_meta_.date_of_install = adrbrowsiel_stats::GetYMDAsDate(week_of_install_);
  } else {
    pyxis_meta_.date_of_install = base::Time::Now();
  }
  pyxis_meta_.woi = adrbrowsiel_stats::GetIsoWeekNumber(pyxis_meta_.date_of_install);

  pyxis_meta_.country_code =
      base::ToUpperASCII(base::CountryCodeForCurrentTimezone());
  pyxis_meta_.refcode = local_state_->GetString(kReferralPromoCode);
  MaybeStripRefcodeAndCountry(&pyxis_meta_);

  UpdatePyxisMeta();

  VLOG(2) << "Pyxis meta: " << pyxis_meta_.platform << " "
          << pyxis_meta_.channel << " " << pyxis_meta_.version << " "
          << pyxis_meta_.woi << " " << pyxis_meta_.wos << " "
          << pyxis_meta_.country_code << " " << pyxis_meta_.refcode;
}

void adrbrowsielP3AService::UpdatePyxisMeta() {
  pyxis_meta_.date_of_survey = base::Time::Now();
  pyxis_meta_.wos = adrbrowsiel_stats::GetIsoWeekNumber(pyxis_meta_.date_of_survey);
}

void adrbrowsielP3AService::StartScheduledUpload() {
  VLOG(2) << "adrbrowsielP3AService::StartScheduledUpload at " << base::Time::Now();
  if (!log_store_->has_unsent_logs()) {
    // We continue to schedule next uploads since new histogram values can
    // come up at any moment. Maybe it's worth to add a method with more
    // appropriate name for this situation.
    upload_scheduler_->UploadFinished(true);
    // Nothing to stage.
    VLOG(2) << "StartScheduledUpload - Nothing to stage.";
    return;
  }
  if (!log_store_->has_staged_log()) {
    log_store_->StageNextLog();
  }

  // Only upload if service is enabled.
  bool p3a_enabled = local_state_->GetBoolean(adrbrowsiel::kP3AEnabled);
  if (p3a_enabled) {
    const std::string log = log_store_->staged_log();
    const std::string log_type = log_store_->staged_log_type();
    VLOG(2) << "StartScheduledUpload - Uploading " << log.size() << " bytes "
            << "of type " << log_type;
    uploader_->UploadLog(log, log_type);
  }
}

void adrbrowsielP3AService::OnHistogramChanged(const char* histogram_name,
                                         uint64_t name_hash,
                                         base::HistogramBase::Sample sample) {
  std::unique_ptr<base::HistogramSamples> samples =
      base::StatisticsRecorder::FindHistogram(histogram_name)->SnapshotDelta();
  DCHECK(!samples->Iterator()->Done());

  // Shortcut for the special values, see |kSuspendedMetricValue|
  // description for details.
  if (IsSuspendedMetric(histogram_name, sample)) {
    base::PostTask(FROM_HERE, {content::BrowserThread::UI},
                   base::BindOnce(&adrbrowsielP3AService::OnHistogramChangedOnUI,
                                  this,
                                  histogram_name,
                                  kSuspendedMetricValue,
                                  kSuspendedMetricBucket));
    return;
  }

  // Note that we store only buckets, not actual values.
  size_t bucket = 0u;
  const bool ok = samples->Iterator()->GetBucketIndex(&bucket);
  if (!ok) {
    LOG(ERROR) << "Only linear histograms are supported at the moment!";
    NOTREACHED();
    return;
  }

  // Special handling of P2A histograms.
  if (base::StartsWith(histogram_name, "adrbrowsiel.P2A.",
                       base::CompareCase::SENSITIVE)) {
    // We need the bucket count to make proper perturbation.
    // All P2A metrics should be implemented as linear histograms.
    base::SampleVector* vector =
        static_cast<base::SampleVector*>(samples.get());
    DCHECK(vector);
    const size_t bucket_count = vector->bucket_ranges()->bucket_count() - 1;
    VLOG(2) << "P2A metric " << histogram_name << " has bucket count "
            << bucket_count;

    // Perturb the bucket.
    bucket = DirectEncodingProtocol::Perturb(bucket_count, bucket);
  }

  base::PostTask(FROM_HERE, {content::BrowserThread::UI},
                 base::BindOnce(&adrbrowsielP3AService::OnHistogramChangedOnUI, this,
                                histogram_name, sample, bucket));
}

void adrbrowsielP3AService::OnHistogramChangedOnUI(const char* histogram_name,
                                             base::HistogramBase::Sample sample,
                                             size_t bucket) {
  VLOG(2) << "adrbrowsielP3AService::OnHistogramChanged: histogram_name = "
          << histogram_name << " Sample = " << sample << " bucket = " << bucket;
  if (!initialized_) {
    // Will handle it later when ready.
    histogram_values_[histogram_name] = bucket;
  } else {
    HandleHistogramChange(histogram_name, bucket);
  }
}

void adrbrowsielP3AService::HandleHistogramChange(base::StringPiece histogram_name,
                                            size_t bucket) {
  if (IsSuspendedMetric(histogram_name, bucket)) {
    log_store_->RemoveValueIfExists(histogram_name.as_string());
    return;
  }
  log_store_->UpdateValue(histogram_name.as_string(), bucket);
}

void adrbrowsielP3AService::OnLogUploadComplete(int response_code,
                                          int error_code,
                                          bool was_https) {
  const bool upload_succeeded = response_code == 200;
  bool ok = upload_succeeded;
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kP3AIgnoreServerErrors)) {
    ok = true;
  }
  VLOG(2) << "adrbrowsielP3AService::UploadFinished ok = " << ok
          << " HTTP response = " << response_code;
  if (ok) {
    log_store_->DiscardStagedLog();
  }
  upload_scheduler_->UploadFinished(ok);
}

void adrbrowsielP3AService::DoRotation() {
  VLOG(2) << "adrbrowsielP3AService doing rotation at " << base::Time::Now();
  log_store_->ResetUploadStamps();
  UpdateRotationTimer();

  local_state_->SetTime(kLastRotationTimeStampPref, base::Time::Now());
}

void adrbrowsielP3AService::UpdateRotationTimer() {
  base::TimeDelta next_rotation = rotation_interval_.is_zero()
                                      ? TimeDeltaTillMonday(base::Time::Now())
                                      : rotation_interval_;
  rotation_timer_.Start(FROM_HERE, next_rotation, this,
                        &adrbrowsielP3AService::DoRotation);

  VLOG(2) << "adrbrowsielP3AService new rotation timer will fire at "
          << base::Time::Now() + next_rotation << " after " << next_rotation;
}

}  // namespace adrbrowsiel
