/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/path_service.h"
#include "base/strings/pattern.h"
#include "base/strings/utf_string_conversions.h"
#include "adrbrowsiel/common/adrbrowsiel_paths.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "content/public/browser/notification_service.h"
#include "content/public/browser/notification_types.h"
#include "content/public/common/url_constants.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"
#include "net/dns/mock_host_resolver.h"

class adrbrowsielSchemeLoadBrowserTest : public InProcessBrowserTest,
                                   public TabStripModelObserver {
 public:
  void SetUpOnMainThread() override {
    InProcessBrowserTest::SetUpOnMainThread();
    host_resolver()->AddRule("*", "127.0.0.1");

    adrbrowsiel::RegisterPathProvider();
    base::FilePath test_data_dir;
    base::PathService::Get(adrbrowsiel::DIR_TEST_DATA, &test_data_dir);
    embedded_test_server()->ServeFilesFromDirectory(test_data_dir);

    ASSERT_TRUE(embedded_test_server()->Start());
  }

  // TabStripModelObserver overrides:
  void OnTabStripModelChanged(
      TabStripModel* tab_strip_model,
      const TabStripModelChange& change,
      const TabStripSelectionChange& selection) override {
    if (change.type() == TabStripModelChange::kInserted) {
      quit_closure_.Run();
    }
  }

  content::WebContents* active_contents() {
    return browser()->tab_strip_model()->GetActiveWebContents();
  }

  bool NavigateToURLUntilLoadStop(const std::string& origin,
                                  const std::string& path) {
    ui_test_utils::NavigateToURL(browser(),
                                 embedded_test_server()->GetURL(origin, path));
    return WaitForLoadStop(active_contents());
  }

  // Check loading |url| in private window is redirected to normal
  // window.
  void TestURLIsNotLoadedInPrivateWindow(const std::string& url) {
    Browser* private_browser = CreateIncognitoBrowser(nullptr);
    TabStripModel* private_model = private_browser->tab_strip_model();

    // Check normal & private window have one blank tab.
    EXPECT_EQ("about:blank",
              private_model->GetActiveWebContents()->GetVisibleURL().spec());
    EXPECT_EQ(1, private_model->count());
    EXPECT_EQ("about:blank", active_contents()->GetVisibleURL().spec());
    EXPECT_EQ(1, browser()->tab_strip_model()->count());

    browser()->tab_strip_model()->AddObserver(this);

    base::RunLoop run_loop;
    quit_closure_ = run_loop.QuitClosure();

    // Load url to private window.
    NavigateParams params(
        private_browser, GURL(url), ui::PAGE_TRANSITION_TYPED);
    Navigate(&params);

    run_loop.Run();

    browser()->tab_strip_model()->RemoveObserver(this);

    EXPECT_STREQ(url.c_str(),
                 base::UTF16ToUTF8(browser()->location_bar_model()
                      ->GetFormattedFullURL()).c_str());
    EXPECT_EQ(2, browser()->tab_strip_model()->count());
    // Private window stays as initial state.
    EXPECT_EQ("about:blank",
              private_model->GetActiveWebContents()->GetVisibleURL().spec());
    EXPECT_EQ(1, private_browser->tab_strip_model()->count());
  }

  base::RepeatingClosure quit_closure_;
};

// Test whether adrbrowsiel page is not loaded from different host by window.open().
IN_PROC_BROWSER_TEST_F(adrbrowsielSchemeLoadBrowserTest, NotAllowedToLoadTest) {
  EXPECT_TRUE(
      NavigateToURLUntilLoadStop("example.com", "/adrbrowsiel_scheme_load.html"));
  content::WebContentsConsoleObserver console_observer(active_contents());
  console_observer.SetPattern(
      "Not allowed to load local resource: adrbrowsiel://settings/");

  ASSERT_TRUE(ExecuteScript(
      active_contents(),
      "window.domAutomationController.send(openadrbrowsielSettings())"));
  console_observer.Wait();
}

// Test whether adrbrowsiel page is not loaded from different host by window.open().
IN_PROC_BROWSER_TEST_F(adrbrowsielSchemeLoadBrowserTest,
                       NotAllowedToLoadTestByWindowOpenWithNoOpener) {
  EXPECT_TRUE(
      NavigateToURLUntilLoadStop("example.com", "/adrbrowsiel_scheme_load.html"));
  content::WebContentsConsoleObserver console_observer(active_contents());
  console_observer.SetPattern(
      "Not allowed to load local resource: adrbrowsiel://settings/");

  ASSERT_TRUE(ExecuteScript(
      active_contents(),
      "window.domAutomationController.send(openadrbrowsielSettingsWithNoOpener())"));
  console_observer.Wait();
}

// Test whether adrbrowsiel page is not loaded from different host directly by
// location.replace().
IN_PROC_BROWSER_TEST_F(adrbrowsielSchemeLoadBrowserTest,
                       NotAllowedToDirectReplaceTest) {
  EXPECT_TRUE(
      NavigateToURLUntilLoadStop("example.com", "/adrbrowsiel_scheme_load.html"));
  content::WebContentsConsoleObserver console_observer(active_contents());
  console_observer.SetPattern(
      "Not allowed to load local resource: adrbrowsiel://settings/");

  ASSERT_TRUE(ExecuteScript(
      active_contents(),
      "window.domAutomationController.send(replaceToadrbrowsielSettingsDirectly())"));
  console_observer.Wait();
}

// Test whether adrbrowsiel page is not loaded from different host indirectly by
// location.replace().
IN_PROC_BROWSER_TEST_F(adrbrowsielSchemeLoadBrowserTest,
                       NotAllowedToIndirectReplaceTest) {
  EXPECT_TRUE(
      NavigateToURLUntilLoadStop("example.com", "/adrbrowsiel_scheme_load.html"));
  auto* initial_active_tab = active_contents();
  content::WebContentsConsoleObserver console_observer(initial_active_tab);
  console_observer.SetPattern(
      "Not allowed to load local resource: adrbrowsiel://settings/");

  ASSERT_TRUE(ExecuteScript(initial_active_tab,
                            "window.domAutomationController.send("
                            "replaceToadrbrowsielSettingsIndirectly())"));
  console_observer.Wait();
}

// Test whether adrbrowsiel page is not loaded from chrome page.
IN_PROC_BROWSER_TEST_F(adrbrowsielSchemeLoadBrowserTest,
                       NotAllowedToadrbrowsielFromChrome) {
  NavigateToURLBlockUntilNavigationsComplete(active_contents(),
                                             GURL("chrome://newtab/"), 1);

  content::WebContentsConsoleObserver console_observer(active_contents());
  console_observer.SetPattern(
      "Not allowed to load local resource: adrbrowsiel://settings/");

  ASSERT_TRUE(
      ExecuteScript(active_contents(), "window.open(\"adrbrowsiel://settings\")"));
  console_observer.Wait();
}

// Test whether adrbrowsiel page is not loaded by click.
IN_PROC_BROWSER_TEST_F(adrbrowsielSchemeLoadBrowserTest, NotAllowedToadrbrowsielByClick) {
  EXPECT_TRUE(
      NavigateToURLUntilLoadStop("example.com", "/adrbrowsiel_scheme_load.html"));
  content::WebContentsConsoleObserver console_observer(active_contents());
  console_observer.SetPattern(
      "Not allowed to load local resource: adrbrowsiel://settings/");

  ASSERT_TRUE(ExecuteScript(
      active_contents(),
      "window.domAutomationController.send(gotoadrbrowsielSettingsByClick())"));
  console_observer.Wait();
}

// Test whether adrbrowsiel page is not loaded by middle click.
IN_PROC_BROWSER_TEST_F(adrbrowsielSchemeLoadBrowserTest,
                       NotAllowedToadrbrowsielByMiddleClick) {
  EXPECT_TRUE(
      NavigateToURLUntilLoadStop("example.com", "/adrbrowsiel_scheme_load.html"));
  content::WebContentsConsoleObserver console_observer(active_contents());
  console_observer.SetPattern(
      "Not allowed to load local resource: adrbrowsiel://settings/");

  ASSERT_TRUE(ExecuteScript(
      active_contents(),
      "window.domAutomationController.send(gotoadrbrowsielSettingsByMiddleClick())"));
  console_observer.Wait();
}

// Check renderer crash happened by observing related notification.
IN_PROC_BROWSER_TEST_F(adrbrowsielSchemeLoadBrowserTest, CrashURLTest) {
  content::RenderProcessHostWatcher crash_observer(
      browser()->tab_strip_model()->GetActiveWebContents(),
      content::RenderProcessHostWatcher::WATCH_FOR_PROCESS_EXIT);
  content::ScopedAllowRendererCrashes allow_renderer_crashes(active_contents());
  browser()->OpenURL(
      content::OpenURLParams(GURL("adrbrowsiel://crash/"), content::Referrer(),
                             WindowOpenDisposition::CURRENT_TAB,
                             ui::PAGE_TRANSITION_TYPED, false));
  crash_observer.Wait();
}

// Some webuis are not allowed to load in private window.
// Allowed url list are checked by IsURLAllowedInIncognito().
// So, corresponding adrbrowsiel scheme url should be filtered as chrome scheme.
// Ex, adrbrowsiel://settings should be loaded only in normal window because
// chrome://settings is not allowed. When tyring to loading adrbrowsiel://settings in
// private window, it should be loaded in normal window instead of private
// window.
IN_PROC_BROWSER_TEST_F(adrbrowsielSchemeLoadBrowserTest,
                       SettingsPageIsNotAllowedInPrivateWindow) {
  TestURLIsNotLoadedInPrivateWindow("adrbrowsiel://settings");
}

IN_PROC_BROWSER_TEST_F(adrbrowsielSchemeLoadBrowserTest,
                       RewardsPageIsNotAllowedInPrivateWindow) {
  TestURLIsNotLoadedInPrivateWindow("adrbrowsiel://rewards");
}

IN_PROC_BROWSER_TEST_F(adrbrowsielSchemeLoadBrowserTest,
                       WalletPageIsNotAllowedInPrivateWindow) {
  TestURLIsNotLoadedInPrivateWindow("adrbrowsiel://wallet");
}

IN_PROC_BROWSER_TEST_F(adrbrowsielSchemeLoadBrowserTest,
                       adrbrowsielSyncPageIsNotAllowedInPrivateWindow) {
  TestURLIsNotLoadedInPrivateWindow("adrbrowsiel://sync");
}
