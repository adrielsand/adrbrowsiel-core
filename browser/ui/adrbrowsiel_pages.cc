/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/adrbrowsiel_pages.h"

#include "adrbrowsiel/browser/webcompat_reporter/webcompat_reporter_dialog.h"
#include "adrbrowsiel/common/webui_url_constants.h"
#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/chrome_pages.h"
#include "chrome/browser/ui/singleton_tabs.h"
#include "chrome/common/webui_url_constants.h"
#include "url/gurl.h"

#if BUILDFLAG(IPFS_ENABLED)
#include "adrbrowsiel/browser/ipfs/ipfs_tab_helper.h"
#endif

namespace adrbrowsiel {

void ShowadrbrowsielRewards(Browser* browser) {
  NavigateParams params(
      GetSingletonTabNavigateParams(browser, GURL(kadrbrowsielUIRewardsURL)));
  ShowSingletonTabOverwritingNTP(browser, &params);
}

void ShowadrbrowsielAdblock(Browser* browser) {
  NavigateParams params(
      GetSingletonTabNavigateParams(browser, GURL(kadrbrowsielUIAdblockURL)));
  ShowSingletonTabOverwritingNTP(browser, &params);
}

void ShowSync(Browser* browser) {
  auto url = chrome::GetSettingsUrl(chrome::kSyncSetupSubPage);
  NavigateParams params(GetSingletonTabNavigateParams(browser, url));
  ShowSingletonTabOverwritingNTP(browser, &params);
}

void ShowWebcompatReporter(Browser* browser) {
  content::WebContents* web_contents =
      browser->tab_strip_model()->GetActiveWebContents();
  if (!web_contents)
    return;

  OpenWebcompatReporterDialog(web_contents);
}

void ShowadrbrowsielWallet(Browser* browser) {
  NavigateParams params(
      GetSingletonTabNavigateParams(browser, GURL(kadrbrowsielUIWalletURL)));
  ShowSingletonTabOverwritingNTP(browser, &params);
}

void ShowExtensionSettings(Browser* browser) {
  NavigateParams params(
      GetSingletonTabNavigateParams(browser, GURL(kExtensionSettingsURL)));
  ShowSingletonTabOverwritingNTP(browser, &params);
}

void ShowWalletSettings(Browser* browser) {
  NavigateParams params(
      GetSingletonTabNavigateParams(browser, GURL(kWalletSettingsURL)));
  ShowSingletonTabOverwritingNTP(browser, &params);
}

void ShowIPFS(Browser* browser) {
  NavigateParams params(
      GetSingletonTabNavigateParams(browser, GURL(kIPFSWebUIURL)));
  ShowSingletonTabOverwritingNTP(browser, &params);
}

}  // namespace adrbrowsiel
