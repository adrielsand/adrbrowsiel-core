/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_SHIELDS_BROWSER_DOMAIN_BLOCK_CONTROLLER_CLIENT_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_SHIELDS_BROWSER_DOMAIN_BLOCK_CONTROLLER_CLIENT_H_

#include <memory>
#include <string>

#include "components/security_interstitials/content/security_interstitial_controller_client.h"
#include "url/gurl.h"

namespace content {
class WebContents;
}  // namespace content

namespace security_interstitials {
class MetricsHelper;
}  // namespace security_interstitials

namespace adrbrowsiel_shields {

class AdBlockCustomFiltersService;

class DomainBlockControllerClient
    : public security_interstitials::SecurityInterstitialControllerClient {
 public:
  static std::unique_ptr<security_interstitials::MetricsHelper>
  GetMetricsHelper(const GURL& url);

  DomainBlockControllerClient(
      content::WebContents* web_contents,
      const GURL& request_url,
      AdBlockCustomFiltersService* ad_block_custom_filters_service,
      PrefService* prefs,
      const std::string& locale);
  ~DomainBlockControllerClient() override = default;

  DomainBlockControllerClient(const DomainBlockControllerClient&) = delete;
  DomainBlockControllerClient& operator=(const DomainBlockControllerClient&) =
      delete;

  void SetDontWarnAgain(bool value);

  // security_interstitials::SecurityInterstitialControllerClient:
  void GoBack() override;
  void Proceed() override;

 private:
  const GURL request_url_;
  AdBlockCustomFiltersService* ad_block_custom_filters_service_;
  bool dont_warn_again_;
};

}  // namespace adrbrowsiel_shields

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_SHIELDS_BROWSER_DOMAIN_BLOCK_CONTROLLER_CLIENT_H_
