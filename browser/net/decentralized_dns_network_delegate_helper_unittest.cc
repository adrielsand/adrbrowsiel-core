/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/net/decentralized_dns_network_delegate_helper.h"

#include <memory>

#include "base/test/scoped_feature_list.h"
#include "adrbrowsiel/browser/net/url_context.h"
#include "adrbrowsiel/components/decentralized_dns/constants.h"
#include "adrbrowsiel/components/decentralized_dns/features.h"
#include "adrbrowsiel/components/decentralized_dns/pref_names.h"
#include "adrbrowsiel/components/decentralized_dns/utils.h"
#include "chrome/test/base/scoped_testing_local_state.h"
#include "chrome/test/base/testing_browser_process.h"
#include "chrome/test/base/testing_profile.h"
#include "components/prefs/testing_pref_service.h"
#include "content/public/test/browser_task_environment.h"
#include "net/base/net_errors.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"

using adrbrowsiel::ResponseCallback;

namespace decentralized_dns {

class DecentralizedDnsNetworkDelegateHelperTest : public testing::Test {
 public:
  DecentralizedDnsNetworkDelegateHelperTest()
      : local_state_(std::make_unique<ScopedTestingLocalState>(
            TestingBrowserProcess::GetGlobal())) {}

  ~DecentralizedDnsNetworkDelegateHelperTest() override = default;

  void SetUp() override {
    feature_list_.InitAndEnableFeature(features::kDecentralizedDns);
    profile_ = std::make_unique<TestingProfile>();
  }

  void TearDown() override {
    profile_.reset();
    local_state_.reset();
  }

  TestingProfile* profile() { return profile_.get(); }
  PrefService* local_state() { return local_state_->Get(); }

 private:
  content::BrowserTaskEnvironment task_environment_;
  std::unique_ptr<TestingProfile> profile_;
  std::unique_ptr<ScopedTestingLocalState> local_state_;
  base::test::ScopedFeatureList feature_list_;
};

TEST_F(DecentralizedDnsNetworkDelegateHelperTest,
       DecentralizedDnsPreRedirectWork) {
  GURL url("http://adrbrowsiel.crypto");
  auto adrbrowsiel_request_info = std::make_shared<adrbrowsiel::adrbrowsielRequestInfo>(url);
  adrbrowsiel_request_info->browser_context = profile();

  // No redirect if resolve method is not set to Ethereum.
  EXPECT_FALSE(IsUnstoppableDomainsResolveMethodEthereum(local_state()));
  int rc = OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(
      ResponseCallback(), adrbrowsiel_request_info);
  EXPECT_EQ(rc, net::OK);
  EXPECT_TRUE(adrbrowsiel_request_info->new_url_spec.empty());

  local_state()->SetInteger(kUnstoppableDomainsResolveMethod,
                            static_cast<int>(ResolveMethodTypes::ETHEREUM));
  EXPECT_TRUE(IsUnstoppableDomainsResolveMethodEthereum(local_state()));

  // No redirect for OTR context.
  adrbrowsiel_request_info->browser_context = profile()->GetPrimaryOTRProfile();
  rc = OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(ResponseCallback(),
                                                          adrbrowsiel_request_info);
  EXPECT_EQ(rc, net::OK);
  EXPECT_TRUE(adrbrowsiel_request_info->new_url_spec.empty());
  adrbrowsiel_request_info->browser_context = profile();

  // TLD is not .crypto
  adrbrowsiel_request_info->request_url = GURL("http://test.com");
  rc = OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(ResponseCallback(),
                                                          adrbrowsiel_request_info);
  EXPECT_EQ(rc, net::OK);
  EXPECT_TRUE(adrbrowsiel_request_info->new_url_spec.empty());
  adrbrowsiel_request_info->request_url = url;

  rc = OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(ResponseCallback(),
                                                          adrbrowsiel_request_info);
  EXPECT_EQ(rc, net::ERR_IO_PENDING);
}

TEST_F(DecentralizedDnsNetworkDelegateHelperTest,
       DecentralizedDnsRedirectWork) {
  GURL url("http://adrbrowsiel.crypto");
  auto adrbrowsiel_request_info = std::make_shared<adrbrowsiel::adrbrowsielRequestInfo>(url);

  // No redirect for failed requests.
  OnBeforeURLRequest_DecentralizedDnsRedirectWork(
      ResponseCallback(), adrbrowsiel_request_info, false, "");
  EXPECT_TRUE(adrbrowsiel_request_info->new_url_spec.empty());

  OnBeforeURLRequest_DecentralizedDnsRedirectWork(ResponseCallback(),
                                                  adrbrowsiel_request_info, true, "");
  EXPECT_TRUE(adrbrowsiel_request_info->new_url_spec.empty());

  // Has both IPFS URI & fallback URL.
  std::string result =
      // offset for array
      "0x0000000000000000000000000000000000000000000000000000000000000020"
      // count for array
      "0000000000000000000000000000000000000000000000000000000000000006"
      // offsets for array elements
      "00000000000000000000000000000000000000000000000000000000000000c0"
      "0000000000000000000000000000000000000000000000000000000000000120"
      "0000000000000000000000000000000000000000000000000000000000000180"
      "00000000000000000000000000000000000000000000000000000000000001a0"
      "00000000000000000000000000000000000000000000000000000000000001c0"
      "0000000000000000000000000000000000000000000000000000000000000200"
      // count for "QmWrdNJWMbvRxxzLhojVKaBDswS4KNVM7LvjsN7QbDrvka"
      "000000000000000000000000000000000000000000000000000000000000002e"
      // encoding for "QmWrdNJWMbvRxxzLhojVKaBDswS4KNVM7LvjsN7QbDrvka"
      "516d5772644e4a574d62765278787a4c686f6a564b614244737753344b4e564d"
      "374c766a734e3751624472766b61000000000000000000000000000000000000"
      // count for "QmbWqxBEKC3P8tqsKc98xmWNzrzDtRLMiMPL8wBuTGsMnR"
      "000000000000000000000000000000000000000000000000000000000000002e"
      // encoding for "QmbWqxBEKC3P8tqsKc98xmWNzrzDtRLMiMPL8wBuTGsMnR"
      "516d6257717842454b433350387471734b633938786d574e7a727a4474524c4d"
      "694d504c387742755447734d6e52000000000000000000000000000000000000"
      // count for empty dns.A
      "0000000000000000000000000000000000000000000000000000000000000000"
      // count for empty dns.AAAA
      "0000000000000000000000000000000000000000000000000000000000000000"
      // count for "https://fallback1.test.com"
      "000000000000000000000000000000000000000000000000000000000000001a"
      // encoding for "https://fallback1.test.com"
      "68747470733a2f2f66616c6c6261636b312e746573742e636f6d000000000000"
      // count for "https://fallback2.test.com"
      "000000000000000000000000000000000000000000000000000000000000001a"
      // encoding for "https://fallback2.test.com"
      "68747470733a2f2f66616c6c6261636b322e746573742e636f6d000000000000";

  OnBeforeURLRequest_DecentralizedDnsRedirectWork(
      ResponseCallback(), adrbrowsiel_request_info, true, result);
  EXPECT_EQ("ipfs://QmWrdNJWMbvRxxzLhojVKaBDswS4KNVM7LvjsN7QbDrvka",
            adrbrowsiel_request_info->new_url_spec);

  // Has legacy IPFS URI & fallback URL
  result =
      // offset for array
      "0x0000000000000000000000000000000000000000000000000000000000000020"
      // count for array
      "0000000000000000000000000000000000000000000000000000000000000006"
      // offsets for array elements
      "00000000000000000000000000000000000000000000000000000000000000c0"
      "00000000000000000000000000000000000000000000000000000000000000e0"
      "0000000000000000000000000000000000000000000000000000000000000140"
      "0000000000000000000000000000000000000000000000000000000000000160"
      "0000000000000000000000000000000000000000000000000000000000000180"
      "00000000000000000000000000000000000000000000000000000000000001c0"
      // count for empty dweb.ipfs.hash
      "0000000000000000000000000000000000000000000000000000000000000000"
      // count for "QmbWqxBEKC3P8tqsKc98xmWNzrzDtRLMiMPL8wBuTGsMnR"
      "000000000000000000000000000000000000000000000000000000000000002e"
      // encoding for "QmbWqxBEKC3P8tqsKc98xmWNzrzDtRLMiMPL8wBuTGsMnR"
      "516d6257717842454b433350387471734b633938786d574e7a727a4474524c4d"
      "694d504c387742755447734d6e52000000000000000000000000000000000000"
      // count for empty dns.A
      "0000000000000000000000000000000000000000000000000000000000000000"
      // count for empty dns.AAAA
      "0000000000000000000000000000000000000000000000000000000000000000"
      // count for "https://fallback1.test.com"
      "000000000000000000000000000000000000000000000000000000000000001a"
      // encoding for "https://fallback1.test.com"
      "68747470733a2f2f66616c6c6261636b312e746573742e636f6d000000000000"
      // count for "https://fallback2.test.com"
      "000000000000000000000000000000000000000000000000000000000000001a"
      // encoding for "https://fallback2.test.com"
      "68747470733a2f2f66616c6c6261636b322e746573742e636f6d000000000000";
  OnBeforeURLRequest_DecentralizedDnsRedirectWork(
      ResponseCallback(), adrbrowsiel_request_info, true, result);
  EXPECT_EQ("ipfs://QmbWqxBEKC3P8tqsKc98xmWNzrzDtRLMiMPL8wBuTGsMnR",
            adrbrowsiel_request_info->new_url_spec);

  // Has both fallback URL
  result =
      // offset for array
      "0x0000000000000000000000000000000000000000000000000000000000000020"
      // count for array
      "0000000000000000000000000000000000000000000000000000000000000006"
      // offsets for array elements
      "00000000000000000000000000000000000000000000000000000000000000c0"
      "00000000000000000000000000000000000000000000000000000000000000e0"
      "0000000000000000000000000000000000000000000000000000000000000100"
      "0000000000000000000000000000000000000000000000000000000000000120"
      "0000000000000000000000000000000000000000000000000000000000000140"
      "0000000000000000000000000000000000000000000000000000000000000180"
      // count for empty dweb.ipfs.hash
      "0000000000000000000000000000000000000000000000000000000000000000"
      // count for empty ipfs.html.value
      "0000000000000000000000000000000000000000000000000000000000000000"
      // count for empty dns.A
      "0000000000000000000000000000000000000000000000000000000000000000"
      // count for empty dns.AAAA
      "0000000000000000000000000000000000000000000000000000000000000000"
      // count for "https://fallback1.test.com"
      "000000000000000000000000000000000000000000000000000000000000001a"
      // encoding for "https://fallback1.test.com"
      "68747470733a2f2f66616c6c6261636b312e746573742e636f6d000000000000"
      // count for "https://fallback2.test.com"
      "000000000000000000000000000000000000000000000000000000000000001a"
      // encoding for "https://fallback2.test.com"
      "68747470733a2f2f66616c6c6261636b322e746573742e636f6d000000000000";
  OnBeforeURLRequest_DecentralizedDnsRedirectWork(
      ResponseCallback(), adrbrowsiel_request_info, true, result);
  EXPECT_EQ("https://fallback1.test.com/", adrbrowsiel_request_info->new_url_spec);

  // Has legacy URL
  result =
      // offset for array
      "0x0000000000000000000000000000000000000000000000000000000000000020"
      // count for array
      "0000000000000000000000000000000000000000000000000000000000000006"
      // offsets for array elements
      "00000000000000000000000000000000000000000000000000000000000000c0"
      "00000000000000000000000000000000000000000000000000000000000000e0"
      "0000000000000000000000000000000000000000000000000000000000000100"
      "0000000000000000000000000000000000000000000000000000000000000120"
      "0000000000000000000000000000000000000000000000000000000000000140"
      "0000000000000000000000000000000000000000000000000000000000000160"
      // count for empty dweb.ipfs.hash
      "0000000000000000000000000000000000000000000000000000000000000000"
      // count for empty ipfs.html.value
      "0000000000000000000000000000000000000000000000000000000000000000"
      // count for empty dns.A
      "0000000000000000000000000000000000000000000000000000000000000000"
      // count for empty dns.AAAA
      "0000000000000000000000000000000000000000000000000000000000000000"
      // count for empty browser.redirect_url
      "0000000000000000000000000000000000000000000000000000000000000000"
      // count for "https://fallback2.test.com"
      "000000000000000000000000000000000000000000000000000000000000001a"
      // encoding for "https://fallback2.test.com"
      "68747470733a2f2f66616c6c6261636b322e746573742e636f6d000000000000";
  OnBeforeURLRequest_DecentralizedDnsRedirectWork(
      ResponseCallback(), adrbrowsiel_request_info, true, result);
  EXPECT_EQ("https://fallback2.test.com/", adrbrowsiel_request_info->new_url_spec);
}

}  // namespace decentralized_dns
