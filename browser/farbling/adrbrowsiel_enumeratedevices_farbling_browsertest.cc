/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/path_service.h"
#include "base/strings/stringprintf.h"
#include "base/task/post_task.h"
#include "base/test/thread_test_helper.h"
#include "adrbrowsiel/browser/adrbrowsiel_content_browser_client.h"
#include "adrbrowsiel/browser/extensions/adrbrowsiel_base_local_data_files_browsertest.h"
#include "adrbrowsiel/common/adrbrowsiel_paths.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_component_updater/browser/local_data_files_service.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/adrbrowsiel_shields_util.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/extensions/extension_browsertest.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/common/chrome_content_client.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "components/network_session_configurator/common/network_switches.h"
#include "components/permissions/permission_request.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"
#include "net/dns/mock_host_resolver.h"

using adrbrowsiel_shields::ControlType;

const char kEnumerateDevicesScript[] =
    "navigator.mediaDevices.enumerateDevices()"
    ".then(function(devices) {"
    "  var devicekinds = '';"
    "  devices.forEach(function(device) {"
    "    devicekinds = devicekinds + device.kind + '|';"
    "  });"
    "  domAutomationController.send(devicekinds);"
    "})";

class adrbrowsielEnumerateDevicesFarblingBrowserTest : public InProcessBrowserTest {
 public:
  adrbrowsielEnumerateDevicesFarblingBrowserTest()
      : https_server_(net::EmbeddedTestServer::TYPE_HTTPS) {
    adrbrowsiel::RegisterPathProvider();
    base::FilePath test_data_dir;
    base::PathService::Get(adrbrowsiel::DIR_TEST_DATA, &test_data_dir);
    https_server_.SetSSLConfig(net::EmbeddedTestServer::CERT_OK);
    https_server_.ServeFilesFromDirectory(test_data_dir);
    EXPECT_TRUE(https_server_.Start());
    top_level_page_url_ = https_server_.GetURL("a.com", "/");
    farbling_url_ = https_server_.GetURL("a.com", "/simple.html");
  }

  adrbrowsielEnumerateDevicesFarblingBrowserTest(
      const adrbrowsielEnumerateDevicesFarblingBrowserTest&) = delete;
  adrbrowsielEnumerateDevicesFarblingBrowserTest& operator=(
      const adrbrowsielEnumerateDevicesFarblingBrowserTest&) = delete;

  ~adrbrowsielEnumerateDevicesFarblingBrowserTest() override {}

  void SetUpCommandLine(base::CommandLine* command_line) override {
    // HTTPS server only serves a valid cert for localhost, so this is needed
    // to load pages from other hosts without an error
    command_line->AppendSwitch(switches::kIgnoreCertificateErrors);
  }

  void SetUpOnMainThread() override {
    InProcessBrowserTest::SetUpOnMainThread();

    content_client_.reset(new ChromeContentClient);
    content::SetContentClient(content_client_.get());
    browser_content_client_.reset(new adrbrowsielContentBrowserClient());
    content::SetBrowserClientForTesting(browser_content_client_.get());

    host_resolver()->AddRule("*", "127.0.0.1");
  }

  void TearDown() override {
    browser_content_client_.reset();
    content_client_.reset();
  }

 protected:
  base::test::ScopedFeatureList feature_list_;
  net::EmbeddedTestServer https_server_;

  const GURL& farbling_url() { return farbling_url_; }

  HostContentSettingsMap* content_settings() {
    return HostContentSettingsMapFactory::GetForProfile(browser()->profile());
  }

  void AllowFingerprinting() {
    adrbrowsiel_shields::SetFingerprintingControlType(
        content_settings(), ControlType::ALLOW, top_level_page_url_);
  }

  void BlockFingerprinting() {
    adrbrowsiel_shields::SetFingerprintingControlType(
        content_settings(), ControlType::BLOCK, top_level_page_url_);
  }

  void SetFingerprintingDefault() {
    adrbrowsiel_shields::SetFingerprintingControlType(
        content_settings(), ControlType::DEFAULT, top_level_page_url_);
  }

  template <typename T>
  int ExecScriptGetInt(const std::string& script, T* frame) {
    int value;
    EXPECT_TRUE(ExecuteScriptAndExtractInt(frame, script, &value));
    return value;
  }

  template <typename T>
  std::string ExecScriptGetStr(const std::string& script, T* frame) {
    std::string value;
    EXPECT_TRUE(ExecuteScriptAndExtractString(frame, script, &value));
    return value;
  }

  content::WebContents* contents() {
    return browser()->tab_strip_model()->GetActiveWebContents();
  }

  bool NavigateToURLUntilLoadStop(const GURL& url) {
    ui_test_utils::NavigateToURL(browser(), url);
    return WaitForLoadStop(contents());
  }

 private:
  GURL top_level_page_url_;
  GURL farbling_url_;
  std::unique_ptr<ChromeContentClient> content_client_;
  std::unique_ptr<adrbrowsielContentBrowserClient> browser_content_client_;
};

// Tests results of farbling known values
IN_PROC_BROWSER_TEST_F(adrbrowsielEnumerateDevicesFarblingBrowserTest,
                       FarbleEnumerateDevices) {
  // Farbling level: off
  // get real navigator.mediaDevices.enumerateDevices array
  AllowFingerprinting();
  NavigateToURLUntilLoadStop(farbling_url());
  std::string real_value =
      ExecScriptGetStr(kEnumerateDevicesScript, contents());
  ASSERT_NE(real_value, "");

  // Farbling level: balanced (default)
  // navigator.mediaDevices.enumerateDevices array is shuffled
  // pseudo-randomly based on domain+session key
  SetFingerprintingDefault();
  NavigateToURLUntilLoadStop(farbling_url());
  std::string balanced_value =
      ExecScriptGetStr(kEnumerateDevicesScript, contents());
  EXPECT_NE(balanced_value, real_value);

  // Farbling level: maximum
  // same as farbling level: balanced
  BlockFingerprinting();
  NavigateToURLUntilLoadStop(farbling_url());
  std::string maximum_value =
      ExecScriptGetStr(kEnumerateDevicesScript, contents());
  EXPECT_EQ(balanced_value, maximum_value);
}
