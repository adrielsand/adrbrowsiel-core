/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <vector>

#include "base/path_service.h"
#include "base/strings/utf_string_conversions.h"
#include "adrbrowsiel/browser/adrbrowsiel_content_browser_client.h"
#include "adrbrowsiel/common/adrbrowsiel_paths.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/common/adrbrowsiel_shield_constants.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/extensions/component_loader.h"
#include "chrome/browser/extensions/extension_service.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/common/buildflags.h"
#include "chrome/common/chrome_content_client.h"
#include "chrome/common/webui_url_constants.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/content_settings/core/common/content_settings_types.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/navigation_entry.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"
#include "content/public/test/test_navigation_observer.h"
#include "extensions/browser/extension_registry.h"
#include "extensions/browser/extension_system.h"
#include "extensions/common/constants.h"
#include "net/dns/mock_host_resolver.h"

class adrbrowsielContentBrowserClientTest : public InProcessBrowserTest {
 public:
  void SetUp() override {
    // This is needed because component extensions are not added by default
    // without it.  Theyu are found to interfere with tests otherwise. It's
    // needed for loading the hangouts extension of which there are tests for
    // below.
    extensions::ComponentLoader::EnableBackgroundExtensionsForTesting();
    InProcessBrowserTest::SetUp();
  }
  void SetUpOnMainThread() override {
    InProcessBrowserTest::SetUpOnMainThread();

    content_client_.reset(new ChromeContentClient);
    content::SetContentClient(content_client_.get());
    browser_content_client_.reset(new adrbrowsielContentBrowserClient());
    content::SetBrowserClientForTesting(browser_content_client_.get());

    host_resolver()->AddRule("*", "127.0.0.1");
    content::SetupCrossSiteRedirector(embedded_test_server());

    adrbrowsiel::RegisterPathProvider();
    base::FilePath test_data_dir;
    base::PathService::Get(adrbrowsiel::DIR_TEST_DATA, &test_data_dir);
    embedded_test_server()->ServeFilesFromDirectory(test_data_dir);

    ASSERT_TRUE(embedded_test_server()->Start());

    magnet_html_url_ = embedded_test_server()->GetURL("a.com", "/magnet.html");
    magnet_url_ = GURL(
        "magnet:?xt=urn:btih:dd8255ecdc7ca55fb0bbf81323d87062db1f6d1c&dn=Big+"
        "Buck+Bunny&tr=udp%3A%2F%2Fexplodie.org%3A6969&tr=udp%3A%2F%2Ftracker."
        "coppersurfer.tk%3A6969&tr=udp%3A%2F%2Ftracker.empire-js.us%3A1337&tr="
        "udp%3A%2F%2Ftracker.leechers-paradise.org%3A6969&tr=udp%3A%2F%"
        "2Ftracker.opentrackr.org%3A1337&tr=wss%3A%2F%2Ftracker.btorrent.xyz&"
        "tr=wss%3A%2F%2Ftracker.fastcast.nz&tr=wss%3A%2F%2Ftracker."
        "openwebtorrent.com&ws=https%3A%2F%2Fwebtorrent.io%2Ftorrents%2F&xs="
        "https%3A%2F%2Fwebtorrent.io%2Ftorrents%2Fbig-buck-bunny.torrent");
    extension_url_ = GURL(
        "chrome-extension://lgjmpdmojkpocjcopdikifhejkkjglho/extension/"
        "adrbrowsiel_webtorrent.html?magnet%3A%3Fxt%3Durn%3Abtih%"
        "3Add8255ecdc7ca55fb0bbf81323d87062db1f6d1c%26dn%3DBig%2BBuck%2BBunny%"
        "26tr%3Dudp%253A%252F%252Fexplodie.org%253A6969%26tr%3Dudp%253A%252F%"
        "252Ftracker.coppersurfer.tk%253A6969%26tr%3Dudp%253A%252F%252Ftracker."
        "empire-js.us%253A1337%26tr%3Dudp%253A%252F%252Ftracker.leechers-"
        "paradise.org%253A6969%26tr%3Dudp%253A%252F%252Ftracker.opentrackr.org%"
        "253A1337%26tr%3Dwss%253A%252F%252Ftracker.btorrent.xyz%26tr%3Dwss%"
        "253A%252F%252Ftracker.fastcast.nz%26tr%3Dwss%253A%252F%252Ftracker."
        "openwebtorrent.com%26ws%3Dhttps%253A%252F%252Fwebtorrent.io%"
        "252Ftorrents%252F%26xs%3Dhttps%253A%252F%252Fwebtorrent.io%"
        "252Ftorrents%252Fbig-buck-bunny.torrent");
    torrent_url_ = GURL("https://webtorrent.io/torrents/sintel.torrent#ix=5");
    torrent_extension_url_ = GURL(
        "chrome-extension://lgjmpdmojkpocjcopdikifhejkkjglho/extension/"
        "adrbrowsiel_webtorrent.html?https://webtorrent.io/torrents/"
        "sintel.torrent#ix=5");
    torrent_invalid_query_extension_url_ = GURL(
        "chrome-extension://lgjmpdmojkpocjcopdikifhejkkjglho/extension/"
        "adrbrowsiel_webtorrent.html?chrome://settings");
  }

  void TearDown() override {
    browser_content_client_.reset();
    content_client_.reset();
  }

  const GURL& magnet_html_url() { return magnet_html_url_; }
  const GURL& magnet_url() { return magnet_url_; }
  const GURL& extension_url() { return extension_url_; }
  const GURL& torrent_url() { return torrent_url_; }
  const GURL& torrent_extension_url() { return torrent_extension_url_; }
  const GURL& torrent_invalid_query_extension_url() {
    return torrent_invalid_query_extension_url_;
  }

  content::ContentBrowserClient* client() {
    return browser_content_client_.get();
  }

 private:
  GURL magnet_html_url_;
  GURL magnet_url_;
  GURL extension_url_;
  GURL torrent_url_;
  GURL torrent_extension_url_;
  GURL torrent_invalid_query_extension_url_;
  std::unique_ptr<ChromeContentClient> content_client_;
  std::unique_ptr<adrbrowsielContentBrowserClient> browser_content_client_;
};

IN_PROC_BROWSER_TEST_F(adrbrowsielContentBrowserClientTest, CanLoadChromeURL) {
  std::vector<std::string> pages{
      chrome::kChromeUIWelcomeHost,
  };

  std::vector<std::string> schemes{
      "adrbrowsiel://",
      "chrome://",
  };

  for (const std::string& page : pages) {
    for (const std::string& scheme : schemes) {
      content::WebContents* contents =
          browser()->tab_strip_model()->GetActiveWebContents();
      ui_test_utils::NavigateToURL(browser(), GURL(scheme + page + "/"));
      ASSERT_TRUE(WaitForLoadStop(contents));

      EXPECT_STREQ(base::UTF16ToUTF8(
                       browser()->location_bar_model()->GetFormattedFullURL())
                       .c_str(),
                   ("adrbrowsiel://" + page).c_str());
      EXPECT_STREQ(contents->GetController()
                       .GetLastCommittedEntry()
                       ->GetVirtualURL()
                       .spec()
                       .c_str(),
                   ("chrome://" + page + "/").c_str());
      EXPECT_STREQ(contents->GetController()
                       .GetLastCommittedEntry()
                       ->GetURL()
                       .spec()
                       .c_str(),
                   ("chrome://" + page + "/").c_str());
    }
  }
}

IN_PROC_BROWSER_TEST_F(adrbrowsielContentBrowserClientTest, CanLoadCustomadrbrowsielPages) {
  std::vector<std::string> pages {
    "adblock",
#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
        "rewards",
#endif
  };

  std::vector<std::string> schemes{
      "adrbrowsiel://",
      "chrome://",
  };

  for (const std::string& page : pages) {
    for (const std::string& scheme : schemes) {
      content::WebContents* contents =
          browser()->tab_strip_model()->GetActiveWebContents();
      ui_test_utils::NavigateToURL(browser(), GURL(scheme + page + "/"));
      ASSERT_TRUE(WaitForLoadStop(contents));

      EXPECT_STREQ(base::UTF16ToUTF8(
                       browser()->location_bar_model()->GetFormattedFullURL())
                       .c_str(),
                   ("adrbrowsiel://" + page).c_str());
      EXPECT_STREQ(contents->GetController()
                       .GetLastCommittedEntry()
                       ->GetVirtualURL()
                       .spec()
                       .c_str(),
                   ("chrome://" + page + "/").c_str());
      EXPECT_STREQ(contents->GetController()
                       .GetLastCommittedEntry()
                       ->GetURL()
                       .spec()
                       .c_str(),
                   ("chrome://" + page + "/").c_str());
    }
  }
}

IN_PROC_BROWSER_TEST_F(adrbrowsielContentBrowserClientTest, CanLoadAboutHost) {
  std::vector<std::string> schemes{
      "chrome://",
      "adrbrowsiel://",
  };

  for (const std::string& scheme : schemes) {
    content::WebContents* contents =
        browser()->tab_strip_model()->GetActiveWebContents();
    ui_test_utils::NavigateToURL(browser(), GURL(scheme + "about/"));
    ASSERT_TRUE(WaitForLoadStop(contents));

    EXPECT_STREQ(base::UTF16ToUTF8(
                     browser()->location_bar_model()->GetFormattedFullURL())
                     .c_str(),
                 "adrbrowsiel://about");
    EXPECT_STREQ(contents->GetController()
                     .GetLastCommittedEntry()
                     ->GetVirtualURL()
                     .spec()
                     .c_str(),
                 "chrome://about/");
    EXPECT_STREQ(contents->GetController()
                     .GetLastCommittedEntry()
                     ->GetURL()
                     .spec()
                     .c_str(),
                 "chrome://chrome-urls/");
  }
}
IN_PROC_BROWSER_TEST_F(adrbrowsielContentBrowserClientTest, RewriteChromeSync) {
  std::vector<std::string> schemes{
      "adrbrowsiel://",
      "chrome://",
  };

  for (const std::string& scheme : schemes) {
    content::WebContents* contents =
        browser()->tab_strip_model()->GetActiveWebContents();
    ui_test_utils::NavigateToURL(browser(),
                                 GURL(scheme + chrome::kChromeUISyncHost));
    ASSERT_TRUE(WaitForLoadStop(contents));

    EXPECT_STREQ(base::UTF16ToUTF8(
                     browser()->location_bar_model()->GetFormattedFullURL())
                     .c_str(),
                 "adrbrowsiel://sync");
    EXPECT_STREQ(contents->GetController()
                     .GetLastCommittedEntry()
                     ->GetVirtualURL()
                     .spec()
                     .c_str(),
                 "chrome://sync/");
    EXPECT_STREQ(contents->GetController()
                     .GetLastCommittedEntry()
                     ->GetURL()
                     .spec()
                     .c_str(),
                 "chrome://settings/adrbrowsielSync");
  }
}

IN_PROC_BROWSER_TEST_F(adrbrowsielContentBrowserClientTest, RewriteMagnetURLURLBar) {
  content::WebContents* contents =
      browser()->tab_strip_model()->GetActiveWebContents();

  ui_test_utils::NavigateToURL(browser(), magnet_url());
  ASSERT_TRUE(WaitForLoadStop(contents));
  EXPECT_STREQ(contents->GetLastCommittedURL().spec().c_str(),
               magnet_url().spec().c_str())
      << "URL visible to users should stay as the magnet URL";
  content::NavigationEntry* entry =
      contents->GetController().GetLastCommittedEntry();
  EXPECT_STREQ(entry->GetURL().spec().c_str(), extension_url().spec().c_str())
      << "Real URL should be extension URL";
}

IN_PROC_BROWSER_TEST_F(adrbrowsielContentBrowserClientTest, RewriteMagnetURLLink) {
  content::WebContents* contents =
      browser()->tab_strip_model()->GetActiveWebContents();
  ui_test_utils::NavigateToURL(browser(), magnet_html_url());
  ASSERT_TRUE(WaitForLoadStop(contents));
  bool value;
  EXPECT_TRUE(
      ExecuteScriptAndExtractBool(contents, "clickMagnetLink();", &value));
  EXPECT_TRUE(value);
  ASSERT_TRUE(WaitForLoadStop(contents));

  EXPECT_STREQ(contents->GetLastCommittedURL().spec().c_str(),
               magnet_url().spec().c_str())
      << "URL visible to users should stay as the magnet URL";
  content::NavigationEntry* entry =
      contents->GetController().GetLastCommittedEntry();
  EXPECT_STREQ(entry->GetURL().spec().c_str(), extension_url().spec().c_str())
      << "Real URL should be extension URL";
}

IN_PROC_BROWSER_TEST_F(adrbrowsielContentBrowserClientTest, TypedMagnetURL) {
  content::WebContents* web_contents =
      browser()->tab_strip_model()->GetActiveWebContents();
  content::TestNavigationObserver observer(web_contents);
  ui_test_utils::SendToOmniboxAndSubmit(browser(), magnet_url().spec());
  observer.Wait();
  EXPECT_EQ(magnet_url(), web_contents->GetLastCommittedURL().spec());
}

IN_PROC_BROWSER_TEST_F(adrbrowsielContentBrowserClientTest,
                       ReverseRewriteTorrentURL) {
  content::WebContents* contents =
      browser()->tab_strip_model()->GetActiveWebContents();

  // Used to add the extension
  ui_test_utils::NavigateToURL(browser(), magnet_url());
  ASSERT_TRUE(WaitForLoadStop(contents));

  ui_test_utils::NavigateToURL(browser(), torrent_extension_url());
  ASSERT_TRUE(WaitForLoadStop(contents));

  EXPECT_STREQ(contents->GetLastCommittedURL().spec().c_str(),
               torrent_url().spec().c_str())
      << "URL visible to users should stay as the torrent URL";
  content::NavigationEntry* entry =
      contents->GetController().GetLastCommittedEntry();
  EXPECT_STREQ(entry->GetURL().spec().c_str(),
               torrent_extension_url().spec().c_str())
      << "Real URL should be extension URL";
}

IN_PROC_BROWSER_TEST_F(adrbrowsielContentBrowserClientTest,
                       NoReverseRewriteTorrentURLForInvalidQuery) {
  content::WebContents* contents =
      browser()->tab_strip_model()->GetActiveWebContents();

  // Used to add the extension
  ui_test_utils::NavigateToURL(browser(), magnet_url());
  ASSERT_TRUE(WaitForLoadStop(contents));

  ui_test_utils::NavigateToURL(browser(),
                               torrent_invalid_query_extension_url());
  ASSERT_TRUE(WaitForLoadStop(contents));
  EXPECT_STREQ(contents->GetLastCommittedURL().spec().c_str(),
               torrent_invalid_query_extension_url().spec().c_str())
      << "URL visible to users should stay as extension URL for invalid query";
  content::NavigationEntry* entry =
      contents->GetController().GetLastCommittedEntry();
  EXPECT_STREQ(entry->GetURL().spec().c_str(),
               torrent_invalid_query_extension_url().spec().c_str())
      << "Real URL should be extension URL";
}

IN_PROC_BROWSER_TEST_F(adrbrowsielContentBrowserClientTest,
                       WebTorrentExtensionEnabledAfterLoad) {
  ASSERT_TRUE(browser()->profile()->GetPrefs()->GetBoolean(kWebTorrentEnabled));

  extensions::ExtensionRegistry* registry =
      extensions::ExtensionRegistry::Get(browser()->profile());
  ASSERT_FALSE(
      registry->enabled_extensions().Contains(adrbrowsiel_webtorrent_extension_id));

  content::WebContents* contents =
      browser()->tab_strip_model()->GetActiveWebContents();
  ui_test_utils::NavigateToURL(browser(), magnet_url());
  WaitForLoadStop(contents);

  ASSERT_TRUE(
      registry->enabled_extensions().Contains(adrbrowsiel_webtorrent_extension_id));
}

IN_PROC_BROWSER_TEST_F(adrbrowsielContentBrowserClientTest,
                       PRE_NoRewriteMagnetURLURLBarWebTorrentDisabled) {
  browser()->profile()->GetPrefs()->SetBoolean(kWebTorrentEnabled, false);
}

IN_PROC_BROWSER_TEST_F(adrbrowsielContentBrowserClientTest,
                       NoRewriteMagnetURLURLBarWebTorrentDisabled) {
  ASSERT_FALSE(
      browser()->profile()->GetPrefs()->GetBoolean(kWebTorrentEnabled));
  extensions::ExtensionRegistry* registry =
      extensions::ExtensionRegistry::Get(browser()->profile());
  ASSERT_FALSE(
      registry->enabled_extensions().Contains(adrbrowsiel_webtorrent_extension_id));

  content::WebContents* contents =
      browser()->tab_strip_model()->GetActiveWebContents();
  ui_test_utils::NavigateToURL(browser(), magnet_url());
  ASSERT_TRUE(WaitForLoadStop(contents));
  EXPECT_STREQ(contents->GetLastCommittedURL().spec().c_str(), "about:blank");
  content::NavigationEntry* entry =
      contents->GetController().GetLastCommittedEntry();
  EXPECT_STREQ(entry->GetURL().spec().c_str(), "about:blank");
}

IN_PROC_BROWSER_TEST_F(adrbrowsielContentBrowserClientTest,
                       PRE_NoRewriteMagnetURLLinkWebTorrentDisabled) {
  browser()->profile()->GetPrefs()->SetBoolean(kWebTorrentEnabled, false);
}

IN_PROC_BROWSER_TEST_F(adrbrowsielContentBrowserClientTest,
                       NoRewriteMagnetURLLinkWebTorrentDisabled) {
  ASSERT_FALSE(
      browser()->profile()->GetPrefs()->GetBoolean(kWebTorrentEnabled));
  extensions::ExtensionRegistry* registry =
      extensions::ExtensionRegistry::Get(browser()->profile());
  ASSERT_FALSE(
      registry->enabled_extensions().Contains(adrbrowsiel_webtorrent_extension_id));

  content::WebContents* contents =
      browser()->tab_strip_model()->GetActiveWebContents();
  ui_test_utils::NavigateToURL(browser(), magnet_html_url());
  ASSERT_TRUE(WaitForLoadStop(contents));
  bool value;
  EXPECT_TRUE(
      ExecuteScriptAndExtractBool(contents, "clickMagnetLink();", &value));
  EXPECT_TRUE(value);
  ASSERT_TRUE(WaitForLoadStop(contents));

  EXPECT_STREQ(contents->GetLastCommittedURL().spec().c_str(),
               magnet_html_url().spec().c_str());
  content::NavigationEntry* entry =
      contents->GetController().GetLastCommittedEntry();
  EXPECT_STREQ(entry->GetURL().spec().c_str(),
               magnet_html_url().spec().c_str());
}

IN_PROC_BROWSER_TEST_F(adrbrowsielContentBrowserClientTest,
                       PRE_NoReverseRewriteTorrentURLWebTorrentDisabled) {
  browser()->profile()->GetPrefs()->SetBoolean(kWebTorrentEnabled, false);
}

IN_PROC_BROWSER_TEST_F(adrbrowsielContentBrowserClientTest,
                       NoReverseRewriteTorrentURLWebTorrentDisabled) {
  ASSERT_FALSE(
      browser()->profile()->GetPrefs()->GetBoolean(kWebTorrentEnabled));
  extensions::ExtensionRegistry* registry =
      extensions::ExtensionRegistry::Get(browser()->profile());
  ASSERT_FALSE(
      registry->enabled_extensions().Contains(adrbrowsiel_webtorrent_extension_id));

  content::WebContents* contents =
      browser()->tab_strip_model()->GetActiveWebContents();
  ui_test_utils::NavigateToURL(browser(), torrent_extension_url());
  WaitForLoadStop(contents);

  EXPECT_STREQ(contents->GetLastCommittedURL().spec().c_str(),
               torrent_extension_url().spec().c_str())
      << "No changes on the visible URL";
  content::NavigationEntry* entry =
      contents->GetController().GetLastCommittedEntry();
  EXPECT_STREQ(entry->GetURL().spec().c_str(),
               torrent_extension_url().spec().c_str())
      << "No changes on the real URL";
}

#if BUILDFLAG(ENABLE_HANGOUT_SERVICES_EXTENSION)
IN_PROC_BROWSER_TEST_F(adrbrowsielContentBrowserClientTest,
                       HangoutsEnabledByDefault) {
  ASSERT_TRUE(browser()->profile()->GetPrefs()->GetBoolean(kHangoutsEnabled));
  extensions::ExtensionRegistry* registry =
      extensions::ExtensionRegistry::Get(browser()->profile());
  ASSERT_TRUE(registry->enabled_extensions().Contains(hangouts_extension_id));
}

IN_PROC_BROWSER_TEST_F(adrbrowsielContentBrowserClientTest,
                       PRE_HangoutsDisabledDoesNotLoadComponent) {
  browser()->profile()->GetPrefs()->SetBoolean(kHangoutsEnabled, false);
}

IN_PROC_BROWSER_TEST_F(adrbrowsielContentBrowserClientTest,
                       HangoutsDisabledDoesNotLoadComponent) {
  ASSERT_FALSE(browser()->profile()->GetPrefs()->GetBoolean(kHangoutsEnabled));
  extensions::ExtensionRegistry* registry =
      extensions::ExtensionRegistry::Get(browser()->profile());
  ASSERT_FALSE(registry->enabled_extensions().Contains(hangouts_extension_id));
}
#endif

class adrbrowsielContentBrowserClientReferrerTest
    : public adrbrowsielContentBrowserClientTest {
 public:
  HostContentSettingsMap* content_settings() {
    return HostContentSettingsMapFactory::GetForProfile(browser()->profile());
  }
};

IN_PROC_BROWSER_TEST_F(adrbrowsielContentBrowserClientReferrerTest,
                       DefaultBehaviour) {
  const GURL kRequestUrl("http://request.com/path?query");
  const GURL kDocumentUrl("http://document.com/path?query");
  const GURL kSameSiteRequestUrl("http://sub.document.com/sub/path");
  const GURL kSameOriginRequestUrl("http://document.com/different/path");

  blink::mojom::ReferrerPtr kReferrer = blink::mojom::Referrer::New(
      kDocumentUrl, network::mojom::ReferrerPolicy::kDefault);

  // Cross-origin navigations get an origin.
  blink::mojom::ReferrerPtr referrer = kReferrer.Clone();
  client()->MaybeHideReferrer(browser()->profile(), kRequestUrl, kDocumentUrl,
                              &referrer);
  EXPECT_EQ(referrer->url, kDocumentUrl.GetOrigin());

  // Same-origin navigations get full referrers.
  referrer = kReferrer.Clone();
  client()->MaybeHideReferrer(browser()->profile(), kSameOriginRequestUrl,
                              kDocumentUrl, &referrer);
  EXPECT_EQ(referrer->url, kDocumentUrl);

  // Same-site navigations get truncated referrers.
  referrer = kReferrer.Clone();
  client()->MaybeHideReferrer(browser()->profile(), kSameSiteRequestUrl,
                              kDocumentUrl, &referrer);
  EXPECT_EQ(referrer->url, kDocumentUrl.GetOrigin());

  // Cross-origin iframe navigations get origins.
  referrer = kReferrer.Clone();
  client()->MaybeHideReferrer(browser()->profile(), kRequestUrl, kDocumentUrl,
                              &referrer);
  EXPECT_EQ(referrer->url, kDocumentUrl.GetOrigin().spec());

  // Same-origin iframe navigations get full referrers.
  referrer = kReferrer.Clone();
  client()->MaybeHideReferrer(browser()->profile(), kSameOriginRequestUrl,
                              kDocumentUrl, &referrer);
  EXPECT_EQ(referrer->url, kDocumentUrl);

  // Special rule for extensions.
  const GURL kExtensionUrl("chrome-extension://abc/path?query");
  referrer = kReferrer.Clone();
  referrer->url = kExtensionUrl;
  client()->MaybeHideReferrer(browser()->profile(), kRequestUrl, kExtensionUrl,
                              &referrer);
  EXPECT_EQ(referrer->url, kExtensionUrl);

  // Allow referrers for certain URL.
  content_settings()->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString(kDocumentUrl.GetOrigin().spec() + "*"),
      ContentSettingsPattern::Wildcard(), ContentSettingsType::adrbrowsiel_REFERRERS,
      CONTENT_SETTING_ALLOW);
  referrer = kReferrer.Clone();
  client()->MaybeHideReferrer(browser()->profile(), kRequestUrl, kDocumentUrl,
                              &referrer);
  EXPECT_EQ(referrer->url, kDocumentUrl);
}
