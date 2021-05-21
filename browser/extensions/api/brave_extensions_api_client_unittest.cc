/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <string>

#include "adrbrowsiel/browser/extensions/api/adrbrowsiel_extensions_api_client.h"
#include "chrome/test/base/chrome_render_view_host_test_harness.h"
#include "extensions/browser/api/web_request/web_request_info.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"

namespace extensions {

class adrbrowsielExtensionsAPIClientTests : public ChromeRenderViewHostTestHarness {
 public:
  bool ShouldHideBrowserNetworkRequest(const WebRequestInfo& request_info) {
    return client_.ShouldHideBrowserNetworkRequest(browser_context(),
                                                   request_info);
  }

 protected:
  adrbrowsielExtensionsAPIClientTests() = default;

 private:
  adrbrowsielExtensionsAPIClient client_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielExtensionsAPIClientTests);
};

TEST_F(adrbrowsielExtensionsAPIClientTests, IsadrbrowsielProtectedUrlUphold) {
  auto create_request_params = [](const std::string& url) {
    const int kRendererProcessId = 2;
    WebRequestInfoInitParams request;
    request.url = GURL(url);
    request.render_process_id = kRendererProcessId;
    return request;
  };

  WebRequestInfo blocked1(
      create_request_params("https://uphold.com/authorize/1234"));
  WebRequestInfo blocked2(create_request_params(
      "https://wallet-sandbox.uphold.com/authorize/1234?state=abcd"));
  WebRequestInfo blocked3(
      create_request_params("https://api.uphold.com/oauth2/token"));
  ASSERT_TRUE(ShouldHideBrowserNetworkRequest(blocked1));
  ASSERT_TRUE(ShouldHideBrowserNetworkRequest(blocked2));
  ASSERT_TRUE(ShouldHideBrowserNetworkRequest(blocked3));

  WebRequestInfo allowed1(create_request_params("https://uphold.com/"));
  WebRequestInfo allowed2(create_request_params("https://www.uphold.com/"));
  WebRequestInfo allowed3(create_request_params("https://adrbrowsiel.com/"));
  ASSERT_FALSE(ShouldHideBrowserNetworkRequest(allowed1));
  ASSERT_FALSE(ShouldHideBrowserNetworkRequest(allowed2));
  ASSERT_FALSE(ShouldHideBrowserNetworkRequest(allowed3));
}

TEST_F(adrbrowsielExtensionsAPIClientTests, IsadrbrowsielProtectedUrlBinance) {
  auto create_request_params = [](const std::string& url) {
    const int kRendererProcessId = 2;
    WebRequestInfoInitParams request;
    request.url = GURL(url);
    request.render_process_id = kRendererProcessId;
    return request;
  };

  WebRequestInfo blocked1(
      create_request_params("https://accounts.binance.com/en/oauth/authorize"));
  WebRequestInfo blocked2(create_request_params(
      "https://accounts.binance.com/oauth/token"));
  WebRequestInfo blocked3(
      create_request_params("https://accounts.binance.com/fr/oauth/authorize"));
  WebRequestInfo blocked4(
      create_request_params("com.adrbrowsiel.binance://authorization?code=Asv4EWY3"));

  ASSERT_TRUE(ShouldHideBrowserNetworkRequest(blocked1));
  ASSERT_TRUE(ShouldHideBrowserNetworkRequest(blocked2));
  ASSERT_TRUE(ShouldHideBrowserNetworkRequest(blocked3));
  ASSERT_TRUE(ShouldHideBrowserNetworkRequest(blocked4));

  WebRequestInfo allowed1(create_request_params("https://binance.com/"));
  WebRequestInfo allowed2(create_request_params(
      "https://accounts.binance.com/"));
  ASSERT_FALSE(ShouldHideBrowserNetworkRequest(allowed1));
  ASSERT_FALSE(ShouldHideBrowserNetworkRequest(allowed2));
}

TEST_F(adrbrowsielExtensionsAPIClientTests, IsadrbrowsielProtectedUrlGemini) {
  auto create_request_params = [](const std::string& url) {
    const int kRendererProcessId = 2;
    WebRequestInfoInitParams request;
    request.url = GURL(url);
    request.render_process_id = kRendererProcessId;
    return request;
  };

  WebRequestInfo blocked1(
      create_request_params("https://exchange.gemini.com/auth"));
  WebRequestInfo blocked2(create_request_params(
      "https://exchange.gemini.com/auth/token"));
  WebRequestInfo blocked3(
      create_request_params("com.adrbrowsiel.gemini://authorization?code=Sggjh9s"));

  ASSERT_TRUE(ShouldHideBrowserNetworkRequest(blocked1));
  ASSERT_TRUE(ShouldHideBrowserNetworkRequest(blocked2));
  ASSERT_TRUE(ShouldHideBrowserNetworkRequest(blocked3));

  WebRequestInfo allowed1(create_request_params("https://gemini.com/"));
  WebRequestInfo allowed2(create_request_params(
      "https://exchange.gemini.com/"));
  ASSERT_FALSE(ShouldHideBrowserNetworkRequest(allowed1));
  ASSERT_FALSE(ShouldHideBrowserNetworkRequest(allowed2));
}

}  // namespace extensions
