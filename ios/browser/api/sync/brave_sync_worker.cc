/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/ios/browser/api/sync/adrbrowsiel_sync_worker.h"

#include <string>
#include <vector>

#include "base/json/json_writer.h"
#include "base/memory/weak_ptr.h"
#include "base/scoped_observer.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/sys_string_conversions.h"
#include "adrbrowsiel/components/adrbrowsiel_sync/adrbrowsiel_sync_prefs.h"
#include "adrbrowsiel/components/adrbrowsiel_sync/crypto/crypto.h"
#include "adrbrowsiel/components/adrbrowsiel_sync/profile_sync_service_helper.h"
#include "adrbrowsiel/components/sync/driver/adrbrowsiel_sync_profile_sync_service.h"
#include "adrbrowsiel/components/sync_device_info/adrbrowsiel_device_info.h"
#include "components/sync/driver/profile_sync_service.h"
#include "components/sync/driver/sync_service.h"
#include "components/sync/driver/sync_service_observer.h"
#include "components/sync_device_info/device_info.h"
#include "components/sync_device_info/device_info_sync_service.h"
#include "components/sync_device_info/device_info_tracker.h"
#include "components/sync_device_info/local_device_info_provider.h"
#include "ios/chrome/browser/browser_state/chrome_browser_state.h"
#include "ios/chrome/browser/sync/device_info_sync_service_factory.h"
#include "ios/chrome/browser/sync/profile_sync_service_factory.h"
#include "ios/chrome/browser/sync/sync_setup_service.h"
#include "ios/chrome/browser/sync/sync_setup_service_factory.h"
#include "ios/web/public/thread/web_thread.h"

namespace {
static const size_t SEED_BYTES_COUNT = 32u;
}  // namespace

adrbrowsielSyncDeviceTracker::adrbrowsielSyncDeviceTracker(
    syncer::DeviceInfoTracker* device_info_tracker,
    std::function<void()> on_device_info_changed_callback)
    : on_device_info_changed_callback_(on_device_info_changed_callback) {
  DCHECK(device_info_tracker);
  device_info_tracker_observer_.Add(device_info_tracker);
}

adrbrowsielSyncDeviceTracker::~adrbrowsielSyncDeviceTracker() {
  // Observer will be removed by ScopedObserver
}

void adrbrowsielSyncDeviceTracker::OnDeviceInfoChange() {
  if (on_device_info_changed_callback_) {
    on_device_info_changed_callback_();
  }
}

adrbrowsielSyncServiceTracker::adrbrowsielSyncServiceTracker(
    syncer::ProfileSyncService* profile_sync_service,
    std::function<void()> on_state_changed_callback)
    : on_state_changed_callback_(on_state_changed_callback) {
  DCHECK(profile_sync_service);
  sync_service_observer_.Add(profile_sync_service);
}

adrbrowsielSyncServiceTracker::~adrbrowsielSyncServiceTracker() {
  // Observer will be removed by ScopedObserver
}

void adrbrowsielSyncServiceTracker::OnStateChanged(syncer::SyncService* sync) {
  if (on_state_changed_callback_) {
    on_state_changed_callback_();
  }
}

adrbrowsielSyncWorker::adrbrowsielSyncWorker(ChromeBrowserState* browser_state)
    : browser_state_(browser_state) {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
}

adrbrowsielSyncWorker::~adrbrowsielSyncWorker() {
  // Observer will be removed by ScopedObserver
}

bool adrbrowsielSyncWorker::SetSyncEnabled(bool enabled) {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  auto* setup_service =
      SyncSetupServiceFactory::GetForBrowserState(browser_state_);
  auto* sync_service =
      ProfileSyncServiceFactory::GetForBrowserState(browser_state_);

  if (!setup_service || !sync_service) {
    return false;
  }

  if (!sync_service_observer_.IsObserving(sync_service)) {
    sync_service_observer_.Add(sync_service);
  }

  setup_service->SetSyncEnabled(enabled);

  if (enabled && !sync_service->GetUserSettings()->IsFirstSetupComplete()) {
    // setup_service->PrepareForFirstSyncSetup();
    // setup_service->CommitSyncChanges();
    setup_service->SetFirstSetupComplete(
        syncer::SyncFirstSetupCompleteSource::ADVANCED_FLOW_CONFIRM);
  }

  return true;
}

const syncer::DeviceInfo* adrbrowsielSyncWorker::GetLocalDeviceInfo() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  auto* device_info_service =
      DeviceInfoSyncServiceFactory::GetForBrowserState(browser_state_);

  if (!device_info_service) {
    return nullptr;
  }

  return device_info_service->GetLocalDeviceInfoProvider()
      ->GetLocalDeviceInfo();
}

std::vector<std::unique_ptr<syncer::adrbrowsielDeviceInfo>>
adrbrowsielSyncWorker::GetDeviceList() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  auto* device_info_service =
      DeviceInfoSyncServiceFactory::GetForBrowserState(browser_state_);

  if (!device_info_service) {
    return std::vector<std::unique_ptr<syncer::adrbrowsielDeviceInfo>>();
  }

  syncer::DeviceInfoTracker* tracker =
      device_info_service->GetDeviceInfoTracker();
  return tracker->GetAlladrbrowsielDeviceInfo();
}

std::string adrbrowsielSyncWorker::GetOrCreateSyncCode() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  auto* sync_service = GetSyncService();
  std::string sync_code;
  if (sync_service) {
    sync_code = sync_service->GetOrCreateSyncCode();
  }
  return sync_code;
}

bool adrbrowsielSyncWorker::IsValidSyncCode(const std::string& sync_code) {
  std::vector<uint8_t> seed;
  if (!adrbrowsiel_sync::crypto::PassphraseToBytes32(sync_code, &seed)) {
    return false;
  }
  return seed.size() == SEED_BYTES_COUNT;
}

bool adrbrowsielSyncWorker::SetSyncCode(const std::string& sync_code) {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  if (sync_code.empty()) {
    return false;
  }

  auto* sync_service = GetSyncService();
  if (!sync_service || !sync_service->SetSyncCode(sync_code)) {
    const std::string error_msg = sync_service
                                      ? "invalid sync code:" + sync_code
                                      : "sync service is not available";
    LOG(ERROR) << error_msg;
    return false;
  }

  passphrase_ = sync_code;
  return true;
}

std::string adrbrowsielSyncWorker::GetSyncCodeFromHexSeed(
    const std::string& hex_code_seed) {
  DCHECK(!hex_code_seed.empty());

  std::vector<uint8_t> bytes;
  std::string sync_code_words;
  if (base::HexStringToBytes(hex_code_seed, &bytes)) {
    DCHECK_EQ(bytes.size(), SEED_BYTES_COUNT);
    if (bytes.size(), SEED_BYTES_COUNT) {
      sync_code_words = adrbrowsiel_sync::crypto::PassphraseFromBytes32(bytes);
      if (sync_code_words.empty()) {
        VLOG(1) << __func__
                << " PassphraseFromBytes32 failed for hex_code_seed";
      }
    } else {
      LOG(ERROR) << "wrong seed bytes " << bytes.size();
    }

    DCHECK_NE(sync_code_words, "");
  } else {
    VLOG(1) << __func__ << " HexStringToBytes failed for hex_code_seed";
  }
  return sync_code_words;
}

bool adrbrowsielSyncWorker::IsFirstSetupComplete() {
  syncer::SyncService* sync_service = GetSyncService();
  return sync_service &&
         sync_service->GetUserSettings()->IsFirstSetupComplete();
}

void adrbrowsielSyncWorker::ResetSync() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  auto* sync_service = GetSyncService();

  if (!sync_service) {
    return;
  }

  auto* device_info_service =
      DeviceInfoSyncServiceFactory::GetForBrowserState(browser_state_);
  DCHECK(device_info_service);

  adrbrowsiel_sync::ResetSync(sync_service, device_info_service,
                        base::BindOnce(&adrbrowsielSyncWorker::OnResetDone,
                                        weak_ptr_factory_.GetWeakPtr()));
}

void adrbrowsielSyncWorker::DeleteDevice(const std::string& device_guid) {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  auto* sync_service = GetSyncService();

  if (!sync_service) {
    return;
  }

  auto* device_info_service =
      DeviceInfoSyncServiceFactory::GetForBrowserState(browser_state_);
  DCHECK(device_info_service);

  adrbrowsiel_sync::DeleteDevice(sync_service, device_info_service, device_guid);
}

syncer::adrbrowsielProfileSyncService* adrbrowsielSyncWorker::GetSyncService() const {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  return static_cast<syncer::adrbrowsielProfileSyncService*>(
      ProfileSyncServiceFactory::GetForBrowserState(browser_state_));
}

void adrbrowsielSyncWorker::SetEncryptionPassphrase(syncer::SyncService* service) {
  DCHECK(service);
  DCHECK(service->IsEngineInitialized());
  DCHECK(!this->passphrase_.empty());

  syncer::SyncUserSettings* sync_user_settings = service->GetUserSettings();
  DCHECK(!sync_user_settings->IsPassphraseRequired());

  if (sync_user_settings->IsEncryptEverythingAllowed() &&
      !sync_user_settings->IsUsingSecondaryPassphrase() &&
      !sync_user_settings->IsTrustedVaultKeyRequired()) {
    sync_user_settings->SetEncryptionPassphrase(this->passphrase_);

    VLOG(3) << "[adrbrowsielSync] " << __func__ << " SYNC_CREATED_NEW_PASSPHRASE";
  }
}

void adrbrowsielSyncWorker::SetDecryptionPassphrase(syncer::SyncService* service) {
  DCHECK(service);
  DCHECK(service->IsEngineInitialized());
  DCHECK(!this->passphrase_.empty());

  syncer::SyncUserSettings* sync_user_settings = service->GetUserSettings();
  DCHECK(sync_user_settings->IsPassphraseRequired());

  if (sync_user_settings->SetDecryptionPassphrase(this->passphrase_)) {
    VLOG(3) << "[adrbrowsielSync] " << __func__
            << " SYNC_ENTERED_EXISTING_PASSPHRASE";
  }
}

void adrbrowsielSyncWorker::OnStateChanged(syncer::SyncService* service) {
  // If the sync engine has shutdown for some reason, just give up
  if (!service || !service->IsEngineInitialized()) {
    VLOG(3) << "[adrbrowsielSync] " << __func__ << " sync engine is not initialized";
    return;
  }

  if (this->passphrase_.empty()) {
    VLOG(3) << "[adrbrowsielSync] " << __func__ << " empty passphrase";
    return;
  }

  adrbrowsiel_sync::Prefs adrbrowsiel_sync_prefs(browser_state_->GetPrefs());
  std::string sync_code = adrbrowsiel_sync_prefs.GetSeed();
  DCHECK_NE(sync_code.size(), 0u);

  if (service->GetUserSettings()->IsPassphraseRequired()) {
    SetDecryptionPassphrase(service);
  } else {
    SetEncryptionPassphrase(service);
  }
}

void adrbrowsielSyncWorker::OnSyncShutdown(syncer::SyncService* service) {
  if (sync_service_observer_.IsObserving(service)) {
    sync_service_observer_.Remove(service);
  }
}

void adrbrowsielSyncWorker::OnResetDone() {
  syncer::SyncService* sync_service = GetSyncService();
  if (sync_service) {
    if (sync_service_observer_.IsObserving(sync_service)) {
      sync_service_observer_.Remove(sync_service);
    }
  }
}

bool adrbrowsielSyncWorker::IsSyncEnabled() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  auto* setup_service =
      SyncSetupServiceFactory::GetForBrowserState(browser_state_);

  if (!setup_service) {
    return false;
  }

  return setup_service->IsSyncEnabled();
}

bool adrbrowsielSyncWorker::IsSyncFeatureActive() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  auto* sync_service =
      ProfileSyncServiceFactory::GetForBrowserState(browser_state_);

  if (!sync_service) {
    return false;
  }

  return sync_service->IsSyncFeatureActive();
}
