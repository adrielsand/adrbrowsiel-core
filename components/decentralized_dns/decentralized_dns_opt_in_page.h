/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_DECENTRALIZED_DNS_DECENTRALIZED_DNS_OPT_IN_PAGE_H_
#define adrbrowsiel_COMPONENTS_DECENTRALIZED_DNS_DECENTRALIZED_DNS_OPT_IN_PAGE_H_

#include <memory>
#include <string>

#include "components/security_interstitials/content/security_interstitial_page.h"

namespace content {
class WebContents;
}  // namespace content

namespace security_interstitials {
class SecurityInterstitialControllerClient;
}  // namespace security_interstitials

namespace decentralized_dns {

class DecentralizedDnsOptInPage
    : public security_interstitials::SecurityInterstitialPage {
 public:
  // Interstitial type, used in tests.
  static const security_interstitials::SecurityInterstitialPage::TypeID
      kTypeForTesting;

  DecentralizedDnsOptInPage(
      content::WebContents* web_contents,
      const GURL& request_url,
      std::unique_ptr<
          security_interstitials::SecurityInterstitialControllerClient>
          controller);
  ~DecentralizedDnsOptInPage() override;

  DecentralizedDnsOptInPage(const DecentralizedDnsOptInPage&) = delete;
  DecentralizedDnsOptInPage& operator=(const DecentralizedDnsOptInPage&) =
      delete;

  // SecurityInterstitialPage::
  void OnInterstitialClosing() override {}
  void CommandReceived(const std::string& command) override;
  security_interstitials::SecurityInterstitialPage::TypeID GetTypeForTesting()
      override;

 protected:
  // SecurityInterstitialPage::
  void PopulateInterstitialStrings(
      base::DictionaryValue* load_time_data) override;
  int GetHTMLTemplateId() override;

 private:
  const GURL request_url_;
};

}  // namespace decentralized_dns

#endif  // adrbrowsiel_COMPONENTS_DECENTRALIZED_DNS_DECENTRALIZED_DNS_OPT_IN_PAGE_H_
