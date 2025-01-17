/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/webui/adrbrowsiel_web_ui_controller_factory.h"

#include <memory>

#include "base/feature_list.h"
#include "base/memory/ptr_util.h"
#include "adrbrowsiel/browser/ui/webui/adrbrowsiel_adblock_ui.h"
#include "adrbrowsiel/browser/ui/webui/webcompat_reporter_ui.h"
#include "adrbrowsiel/common/adrbrowsiel_features.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/common/webui_url_constants.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/buildflags/buildflags.h"
#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"
#include "adrbrowsiel/components/ipfs/features.h"
#include "adrbrowsiel/components/tor/buildflags/buildflags.h"
#include "build/build_config.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/url_constants.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_contents.h"
#include "url/gurl.h"

#if !defined(OS_ANDROID)
#include "adrbrowsiel/browser/ui/webui/adrbrowsiel_settings_ui.h"
#include "adrbrowsiel/browser/ui/webui/adrbrowsiel_welcome_ui.h"
#include "adrbrowsiel/browser/ui/webui/new_tab_page/adrbrowsiel_new_tab_ui.h"
#endif

#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
#include "adrbrowsiel/browser/ui/webui/adrbrowsiel_tip_ui.h"
#include "adrbrowsiel/browser/ui/webui/adrbrowsiel_rewards_internals_ui.h"
#include "adrbrowsiel/browser/ui/webui/adrbrowsiel_rewards_page_ui.h"
#endif

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED) && !defined(OS_ANDROID)
#include "adrbrowsiel/browser/ui/webui/adrbrowsiel_wallet/wallet_panel_ui.h"
#include "adrbrowsiel/browser/ui/webui/adrbrowsiel_wallet_ui.h"
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "adrbrowsiel/browser/ui/webui/ipfs_ui.h"
#include "adrbrowsiel/components/ipfs/ipfs_utils.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "adrbrowsiel/browser/ui/webui/tor_internals_ui.h"
#endif

using content::WebUI;
using content::WebUIController;

namespace {

// A function for creating a new WebUI. The caller owns the return value, which
// may be NULL (for example, if the URL refers to an non-existent extension).
typedef WebUIController* (*WebUIFactoryFunction)(WebUI* web_ui,
                                                 const GURL& url);

WebUIController* NewWebUI(WebUI* web_ui, const GURL& url) {
  auto host = url.host_piece();
  if (host == kAdblockHost) {
    return new adrbrowsielAdblockUI(web_ui, url.host());
  } else if (host == kWebcompatReporterHost) {
    return new WebcompatReporterUI(web_ui, url.host());
#if BUILDFLAG(IPFS_ENABLED)
  } else if (host == kIPFSWebUIHost &&
             ipfs::IsIpfsEnabled(
                 web_ui->GetWebContents()->GetBrowserContext())) {
    return new IPFSUI(web_ui, url.host());
#endif  // BUILDFLAG(IPFS_ENABLED)
#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED) && !defined(OS_ANDROID)
  } else if (host == kWalletHost) {
    return new adrbrowsielWalletUI(web_ui, url.host());
  } else if (host == kWalletPanelHost) {
    return new WalletPanelUI(web_ui);
#endif  // BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
  } else if (host == kRewardsPageHost) {
    return new adrbrowsielRewardsPageUI(web_ui, url.host());
  } else if (host == kRewardsInternalsHost) {
    return new adrbrowsielRewardsInternalsUI(web_ui, url.host());
#if !defined(OS_ANDROID)
  } else if (host == kTipHost) {
    return new adrbrowsielTipUI(web_ui, url.host());
#endif  // !defined(OS_ANDROID)
#endif  // BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
#if !defined(OS_ANDROID)
  } else if (host == kWelcomeHost) {
    return new adrbrowsielWelcomeUI(web_ui, url.host());
  } else if (host == chrome::kChromeUISettingsHost) {
    return new adrbrowsielSettingsUI(web_ui, url.host());
  } else if (host == chrome::kChromeUINewTabHost) {
    return new adrbrowsielNewTabUI(web_ui, url.host());
#endif  // !defined(OS_ANDROID)
#if BUILDFLAG(ENABLE_TOR)
  } else if (host == kTorInternalsHost) {
    return new TorInternalsUI(web_ui, url.host());
#endif
  }
  return nullptr;
}

// Returns a function that can be used to create the right type of WebUI for a
// tab, based on its URL. Returns NULL if the URL doesn't have WebUI associated
// with it.
WebUIFactoryFunction GetWebUIFactoryFunction(WebUI* web_ui,
                                             const GURL& url) {
  if (url.host_piece() == kAdblockHost ||
      url.host_piece() == kWebcompatReporterHost ||
#if BUILDFLAG(IPFS_ENABLED)
      (url.host_piece() == kIPFSWebUIHost &&
       base::FeatureList::IsEnabled(ipfs::features::kIpfsFeature)) ||
#endif  // BUILDFLAG(IPFS_ENABLED)
#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED) && !defined(OS_ANDROID)
      url.host_piece() == kWalletHost || url.host_piece() == kWalletPanelHost ||
#endif
#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
      url.host_piece() == kRewardsPageHost ||
      url.host_piece() == kRewardsInternalsHost ||
      url.host_piece() == kTipHost ||
#endif
#if BUILDFLAG(ENABLE_TOR)
      url.host_piece() == kTorInternalsHost ||
#endif
      url.host_piece() == kWelcomeHost ||
      url.host_piece() == chrome::kChromeUIWelcomeURL ||
#if !defined(OS_ANDROID)
      // On Android New Tab is a native page implemented in Java, so no need in
      // WebUI.
      url.host_piece() == chrome::kChromeUINewTabHost ||
#endif  // !defined(OS_ANDROID)
      url.host_piece() == chrome::kChromeUISettingsHost) {
    return &NewWebUI;
  }

  return nullptr;
}

#if defined(OS_ANDROID)
bool ShouldBlockRewardsWebUI(
      content::BrowserContext* browser_context, const GURL& url) {
  if (url.host_piece() != kRewardsPageHost &&
      url.host_piece() != kRewardsInternalsHost) {
    return false;
  }
  if (!base::FeatureList::IsEnabled(features::kadrbrowsielRewards)) {
    return true;
  }
  Profile* profile = Profile::FromBrowserContext(browser_context);
  if (profile &&
      profile->GetPrefs() &&
      profile->GetPrefs()->GetBoolean(kSafetynetCheckFailed)) {
    return true;
  }
  return false;
}
#endif  // defined(OS_ANDROID)

}  // namespace

WebUI::TypeID adrbrowsielWebUIControllerFactory::GetWebUIType(
      content::BrowserContext* browser_context, const GURL& url) {
#if defined(OS_ANDROID)
  if (ShouldBlockRewardsWebUI(browser_context, url)) {
    return WebUI::kNoWebUI;
  }
#endif  // defined(OS_ANDROID)
  WebUIFactoryFunction function = GetWebUIFactoryFunction(NULL, url);
  if (function) {
    return reinterpret_cast<WebUI::TypeID>(function);
  }
  return ChromeWebUIControllerFactory::GetWebUIType(browser_context, url);
}

std::unique_ptr<WebUIController>
adrbrowsielWebUIControllerFactory::CreateWebUIControllerForURL(WebUI* web_ui,
                                                         const GURL& url) {
  WebUIFactoryFunction function = GetWebUIFactoryFunction(web_ui, url);
  if (!function) {
    return ChromeWebUIControllerFactory::CreateWebUIControllerForURL(
        web_ui, url);
  }

  return base::WrapUnique((*function)(web_ui, url));
}


// static
adrbrowsielWebUIControllerFactory* adrbrowsielWebUIControllerFactory::GetInstance() {
  return base::Singleton<adrbrowsielWebUIControllerFactory>::get();
}

adrbrowsielWebUIControllerFactory::adrbrowsielWebUIControllerFactory() {
}

adrbrowsielWebUIControllerFactory::~adrbrowsielWebUIControllerFactory() {
}
