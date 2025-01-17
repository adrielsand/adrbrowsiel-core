/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <string>

#include "adrbrowsiel/browser/net/adrbrowsiel_system_request_handler.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/net/system_network_context_manager.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "components/network_session_configurator/common/network_switches.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/simple_url_loader_test_helper.h"
#include "net/dns/mock_host_resolver.h"
#include "net/test/embedded_test_server/embedded_test_server.h"
#include "net/test/embedded_test_server/http_request.h"
#include "net/traffic_annotation/network_traffic_annotation.h"
#include "net/traffic_annotation/network_traffic_annotation_test_helper.h"
#include "testing/gtest/include/gtest/gtest.h"

// Test to check if key is added for adrbrowsiel apis
class SystemNetworkContextManagerBrowsertest : public InProcessBrowserTest {
 public:
  SystemNetworkContextManagerBrowsertest()
      : https_server_(net::test_server::EmbeddedTestServer::TYPE_HTTPS) {
    https_server_.RegisterRequestMonitor(base::BindRepeating(
        &SystemNetworkContextManagerBrowsertest::MonitorResourceRequest,
        base::Unretained(this)));
  }

  ~SystemNetworkContextManagerBrowsertest() override {}

  void SetUpCommandLine(base::CommandLine* command_line) override {
    // This is needed to load pages from "domain.com" without an interstitial.
    command_line->AppendSwitch(switches::kIgnoreCertificateErrors);
  }

  scoped_refptr<network::SharedURLLoaderFactory> loader_factory() const {
    return loader_factory_;
  }

  void SetUpOnMainThread() override {
    host_resolver()->AddRule("*", "127.0.0.1");
    ASSERT_TRUE(https_server_.Start());

    loader_factory_ = g_browser_process->system_network_context_manager()
                          ->GetSharedURLLoaderFactory();
  }

  bool LoadURL(const std::string& host) {
    auto request = std::make_unique<network::ResourceRequest>();
    request->url = https_server_.GetURL(host, "/");
    content::SimpleURLLoaderTestHelper simple_loader_helper;
    std::unique_ptr<network::SimpleURLLoader> simple_loader =
        network::SimpleURLLoader::Create(std::move(request),
                                         TRAFFIC_ANNOTATION_FOR_TESTS);

    simple_loader->DownloadToStringOfUnboundedSizeUntilCrashAndDie(
        loader_factory().get(), simple_loader_helper.GetCallback());
    simple_loader_helper.WaitForCallback();

    return service_key_present_;
  }

 private:
  void MonitorResourceRequest(const net::test_server::HttpRequest& request) {
    service_key_present_ = request.headers.count(kadrbrowsielServicesKeyHeader) > 0;
  }

  bool service_key_present_ = false;
  net::test_server::EmbeddedTestServer https_server_;
  scoped_refptr<network::SharedURLLoaderFactory> loader_factory_ = nullptr;
};

IN_PROC_BROWSER_TEST_F(SystemNetworkContextManagerBrowsertest,
                       CheckForadrbrowsielServiceKey) {
  EXPECT_TRUE(LoadURL("demo.adrbrowsiel.com"));
  EXPECT_TRUE(LoadURL("demo.adrbrowsielsoftware.com"));
  EXPECT_FALSE(LoadURL("adrbrowsiel.demo.com"));
  EXPECT_FALSE(LoadURL("randomdomain.com"));
}
