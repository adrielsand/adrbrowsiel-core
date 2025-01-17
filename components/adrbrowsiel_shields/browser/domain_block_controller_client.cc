/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/adrbrowsiel_shields/browser/domain_block_controller_client.h"

#include "adrbrowsiel/components/adrbrowsiel_shields/browser/ad_block_custom_filters_service.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/domain_block_tab_storage.h"
#include "components/prefs/pref_service.h"
#include "components/security_interstitials/content/settings_page_helper.h"
#include "components/security_interstitials/core/metrics_helper.h"
#include "content/public/browser/page_navigator.h"
#include "content/public/browser/web_contents.h"
#include "content/public/common/referrer.h"
#include "ui/base/page_transition_types.h"
#include "ui/base/window_open_disposition.h"

namespace adrbrowsiel_shields {

// static
std::unique_ptr<security_interstitials::MetricsHelper>
DomainBlockControllerClient::GetMetricsHelper(const GURL& url) {
  security_interstitials::MetricsHelper::ReportDetails report_details;
  report_details.metric_prefix = "domain_block";

  return std::make_unique<security_interstitials::MetricsHelper>(
      url, report_details, nullptr);
}

DomainBlockControllerClient::DomainBlockControllerClient(
    content::WebContents* web_contents,
    const GURL& request_url,
    AdBlockCustomFiltersService* ad_block_custom_filters_service,
    PrefService* prefs,
    const std::string& locale)
    : security_interstitials::SecurityInterstitialControllerClient(
          web_contents,
          GetMetricsHelper(request_url),
          prefs,
          locale,
          GURL("about:blank") /* default_safe_page */,
          nullptr /* settings_page_helper */),
      request_url_(request_url),
      ad_block_custom_filters_service_(ad_block_custom_filters_service),
      dont_warn_again_(false) {}

void DomainBlockControllerClient::GoBack() {
  SecurityInterstitialControllerClient::GoBackAfterNavigationCommitted();
}

void DomainBlockControllerClient::Proceed() {
  DomainBlockTabStorage* tab_storage =
      DomainBlockTabStorage::GetOrCreate(web_contents_);
  tab_storage->SetIsProceeding(true);
  if (dont_warn_again_) {
    std::string custom_filters =
        ad_block_custom_filters_service_->GetCustomFilters();
    ad_block_custom_filters_service_->UpdateCustomFilters(
        "@@||" + request_url_.host() + "^\n" + custom_filters);
  }
  web_contents_->GetController().Reload(content::ReloadType::NORMAL, false);
}

void DomainBlockControllerClient::SetDontWarnAgain(bool value) {
  dont_warn_again_ = value;
}

}  // namespace adrbrowsiel_shields
