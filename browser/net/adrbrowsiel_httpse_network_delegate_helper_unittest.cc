/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>

#include "adrbrowsiel/browser/net/adrbrowsiel_httpse_network_delegate_helper.h"

#include "adrbrowsiel/browser/net/url_context.h"
#include "adrbrowsiel/common/network_constants.h"
#include "chrome/test/base/chrome_render_view_host_test_harness.h"
#include "content/public/test/browser_task_environment.h"
#include "net/cookies/site_for_cookies.h"
#include "net/traffic_annotation/network_traffic_annotation_test_helper.h"
#include "net/url_request/url_request_test_util.h"

namespace {

class adrbrowsielHTTPSENetworkDelegateHelperTest: public testing::Test {
 public:
  adrbrowsielHTTPSENetworkDelegateHelperTest()
      : task_environment_(content::BrowserTaskEnvironment::IO_MAINLOOP),
        context_(new net::TestURLRequestContext(true)) {
  }
  ~adrbrowsielHTTPSENetworkDelegateHelperTest() override {}
  void SetUp() override {
    context_->Init();
  }
  net::TestURLRequestContext* context() { return context_.get(); }

 private:
  content::BrowserTaskEnvironment task_environment_;
  std::unique_ptr<net::TestURLRequestContext> context_;
};


TEST_F(adrbrowsielHTTPSENetworkDelegateHelperTest, AlreadySetNewURLNoOp) {
  net::TestDelegate test_delegate;
  GURL url("http://bradhatesprimes.adrbrowsiel.com/composite_numbers_ftw");
  std::unique_ptr<net::URLRequest> request =
      context()->CreateRequest(url, net::IDLE, &test_delegate,
                               TRAFFIC_ANNOTATION_FOR_TESTS);
  std::shared_ptr<adrbrowsiel::adrbrowsielRequestInfo>
      adrbrowsiel_request_info(new adrbrowsiel::adrbrowsielRequestInfo());
  request->set_site_for_cookies(net::SiteForCookies::FromUrl(
      GURL("http://brad.adrbrowsiel.com/hide_all_primes_in_ui/composites_forever")));
  adrbrowsiel_request_info->new_url_spec = "data:image/png;base64,iVB";
  adrbrowsiel::ResponseCallback callback;
  int ret =
    OnBeforeURLRequest_HttpsePreFileWork(callback, adrbrowsiel_request_info);
  EXPECT_EQ(adrbrowsiel_request_info->new_url_spec, adrbrowsiel_request_info->new_url_spec);
  EXPECT_EQ(ret, net::OK);
}

}  // namespace
