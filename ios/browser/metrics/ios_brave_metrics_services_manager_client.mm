/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/ios/browser/metrics/ios_adrbrowsiel_metrics_services_manager_client.h"

#include "components/metrics/metrics_service_client.h"
#include "components/prefs/pref_service.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

IOSadrbrowsielMetricsServicesManagerClient::IOSadrbrowsielMetricsServicesManagerClient(
    PrefService* local_state)
    : IOSChromeMetricsServicesManagerClient(local_state) {}

IOSadrbrowsielMetricsServicesManagerClient::
    ~IOSadrbrowsielMetricsServicesManagerClient() = default;

std::unique_ptr<metrics::MetricsServiceClient>
IOSadrbrowsielMetricsServicesManagerClient::CreateMetricsServiceClient() {
  return nullptr;
}

bool IOSadrbrowsielMetricsServicesManagerClient::IsMetricsReportingEnabled() {
  return false;
}

bool IOSadrbrowsielMetricsServicesManagerClient::IsMetricsConsentGiven() {
  return false;
}

bool IOSadrbrowsielMetricsServicesManagerClient::IsOffTheRecordSessionActive() {
  return false;
}
