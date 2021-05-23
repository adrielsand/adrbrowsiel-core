/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/adrbrowsiel_content_browser_client.h"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "base/bind.h"
#include "base/json/json_reader.h"
#include "base/rand_util.h"
#include "base/system/sys_info.h"
#include "base/task/post_task.h"
#include "adrbrowsiel/browser/adrbrowsiel_browser_main_extra_parts.h"
#include "adrbrowsiel/browser/adrbrowsiel_browser_process.h"
#include "adrbrowsiel/browser/adrbrowsiel_shields/adrbrowsiel_shields_web_contents_observer.h"
#include "adrbrowsiel/browser/net/adrbrowsiel_proxying_url_loader_factory.h"
#include "adrbrowsiel/browser/net/adrbrowsiel_proxying_web_socket.h"
#include "adrbrowsiel/browser/profiles/adrbrowsiel_renderer_updater.h"
#include "adrbrowsiel/browser/profiles/adrbrowsiel_renderer_updater_factory.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/common/webui_url_constants.h"
#include "adrbrowsiel/components/binance/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_search/browser/adrbrowsiel_search_host.h"
#include "adrbrowsiel/components/adrbrowsiel_search/common/adrbrowsiel_search.mojom.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/adrbrowsiel_shields_util.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/domain_block_navigation_throttle.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/common/adrbrowsiel_shield_constants.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_webtorrent/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/cosmetic_filters/browser/cosmetic_filters_resources.h"
#include "adrbrowsiel/components/cosmetic_filters/common/cosmetic_filters.mojom.h"
#include "adrbrowsiel/components/decentralized_dns/buildflags/buildflags.h"
#include "adrbrowsiel/components/ftx/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/gemini/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"
#include "adrbrowsiel/components/speedreader/buildflags.h"
#include "adrbrowsiel/components/tor/buildflags/buildflags.h"
#include "adrbrowsiel/grit/adrbrowsiel_generated_resources.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/chrome_browser_interface_binders.h"
#include "chrome/browser/chrome_content_browser_client.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_io_data.h"
#include "chrome/common/url_constants.h"
#include "components/content_settings/browser/page_specific_content_settings.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/prefs/pref_service.h"
#include "components/services/heap_profiling/public/mojom/heap_profiling_client.mojom.h"
#include "components/version_info/version_info.h"
#include "content/browser/renderer_host/render_frame_host_impl.h"
#include "content/browser/service_worker/service_worker_host.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/browser_url_handler.h"
#include "content/public/browser/navigation_handle.h"
#include "content/public/browser/storage_partition.h"
#include "content/public/common/content_client.h"
#include "content/public/common/content_switches.h"
#include "extensions/buildflags/buildflags.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "net/base/registry_controlled_domains/registry_controlled_domain.h"
#include "net/cookies/site_for_cookies.h"
#include "third_party/blink/public/common/associated_interfaces/associated_interface_registry.h"
#include "third_party/blink/public/common/loader/url_loader_throttle.h"
#include "ui/base/l10n/l10n_util.h"

using adrbrowsiel_shields::adrbrowsielShieldsWebContentsObserver;
using adrbrowsiel_shields::ControlType;
using adrbrowsiel_shields::GetadrbrowsielShieldsEnabled;
using adrbrowsiel_shields::GetFingerprintingControlType;
using content::BrowserThread;
using content::ContentBrowserClient;
using content::RenderFrameHost;
using content::WebContents;

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "chrome/browser/extensions/chrome_content_browser_client_extensions_part.h"
#include "extensions/browser/extension_registry.h"
using extensions::ChromeContentBrowserClientExtensionsPart;
#endif

#if BUILDFLAG(ENABLE_adrbrowsiel_WEBTORRENT)
#include "adrbrowsiel/browser/extensions/adrbrowsiel_webtorrent_navigation_throttle.h"
#include "adrbrowsiel/components/adrbrowsiel_webtorrent/browser/content_browser_client_helper.h"
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "adrbrowsiel/browser/ipfs/content_browser_client_helper.h"
#include "adrbrowsiel/browser/ipfs/ipfs_service_factory.h"
#include "adrbrowsiel/components/ipfs/ipfs_constants.h"
#include "adrbrowsiel/components/ipfs/ipfs_navigation_throttle.h"
#endif

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
#include "adrbrowsiel/components/decentralized_dns/decentralized_dns_navigation_throttle.h"
#endif

#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/rewards_protocol_handler.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "adrbrowsiel/browser/tor/onion_location_navigation_throttle_delegate.h"
#include "adrbrowsiel/browser/tor/tor_profile_service_factory.h"
#include "adrbrowsiel/components/tor/onion_location_navigation_throttle.h"
#include "adrbrowsiel/components/tor/tor_navigation_throttle.h"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "adrbrowsiel/browser/speedreader/speedreader_tab_helper.h"
#include "adrbrowsiel/components/speedreader/speedreader_throttle.h"
#include "third_party/blink/public/mojom/loader/resource_load_info.mojom-shared.h"
#endif

#if BUILDFLAG(BINANCE_ENABLED)
#include "adrbrowsiel/browser/binance/binance_protocol_handler.h"
#endif

#if BUILDFLAG(GEMINI_ENABLED)
#include "adrbrowsiel/browser/gemini/gemini_protocol_handler.h"
#endif

#if BUILDFLAG(ENABLE_FTX)
#include "adrbrowsiel/browser/ftx/ftx_protocol_handler.h"
#endif

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
#include "adrbrowsiel/browser/adrbrowsiel_wallet/adrbrowsiel_wallet_context_utils.h"
#include "adrbrowsiel/browser/adrbrowsiel_wallet/adrbrowsiel_wallet_service_factory.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/adrbrowsiel_wallet_constants.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/adrbrowsiel_wallet_provider_impl.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/adrbrowsiel_wallet_service.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/adrbrowsiel_wallet_utils.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/adrbrowsiel_wallet.mojom.h"
#if !defined(OS_ANDROID)
#include "adrbrowsiel/browser/ui/webui/adrbrowsiel_wallet/wallet_panel_ui.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet_ui/wallet_panel.mojom.h"
#endif
#endif

#if !defined(OS_ANDROID)
#include "adrbrowsiel/browser/new_tab/new_tab_shows_navigation_throttle.h"
#endif

namespace {

bool HandleURLReverseOverrideRewrite(GURL* url,
                                     content::BrowserContext* browser_context) {
  if (adrbrowsielContentBrowserClient::HandleURLOverrideRewrite(url, browser_context))
    return true;

  return false;
}

bool HandleURLRewrite(GURL* url, content::BrowserContext* browser_context) {
  if (adrbrowsielContentBrowserClient::HandleURLOverrideRewrite(url, browser_context))
    return true;

  return false;
}

void BindCosmeticFiltersResources(
    content::RenderFrameHost* const frame_host,
    mojo::PendingReceiver<cosmetic_filters::mojom::CosmeticFiltersResources>
        receiver) {
  auto* web_contents = content::WebContents::FromRenderFrameHost(frame_host);
  if (!web_contents)
    return;

  auto* profile =
      Profile::FromBrowserContext(web_contents->GetBrowserContext());
  auto* settings_map = HostContentSettingsMapFactory::GetForProfile(profile);

  mojo::MakeSelfOwnedReceiver(
      std::make_unique<cosmetic_filters::CosmeticFiltersResources>(
          settings_map, g_adrbrowsiel_browser_process->ad_block_service()),
      std::move(receiver));
}

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
void MaybeBindadrbrowsielWalletProvider(
    content::RenderFrameHost* const frame_host,
    mojo::PendingReceiver<adrbrowsiel_wallet::mojom::adrbrowsielWalletProvider> receiver) {
  auto* context = frame_host->GetBrowserContext();
  if (!adrbrowsiel_wallet::IsAllowedForContext(context))
    return;

  adrbrowsielWalletService* service =
      adrbrowsielWalletServiceFactory::GetInstance()->GetForContext(context);

  mojo::MakeSelfOwnedReceiver(
      std::make_unique<adrbrowsiel_wallet::adrbrowsielWalletProviderImpl>(
          service->AsWeakPtr()),
      std::move(receiver));
}
#endif

void BindadrbrowsielSearchHost(
    int process_id,
    mojo::PendingReceiver<adrbrowsiel_search::mojom::adrbrowsielSearchFallback> receiver) {
  content::RenderProcessHost* render_process_host =
      content::RenderProcessHost::FromID(process_id);
  if (!render_process_host)
    return;

  content::BrowserContext* context = render_process_host->GetBrowserContext();
  mojo::MakeSelfOwnedReceiver(
      std::make_unique<adrbrowsiel_search::adrbrowsielSearchHost>(
          content::BrowserContext::GetDefaultStoragePartition(context)
              ->GetURLLoaderFactoryForBrowserProcess()),
      std::move(receiver));
}
}  // namespace

adrbrowsielContentBrowserClient::adrbrowsielContentBrowserClient()
    : session_token_(base::RandUint64()),
      incognito_session_token_(base::RandUint64()) {}

adrbrowsielContentBrowserClient::~adrbrowsielContentBrowserClient() {}

std::unique_ptr<content::BrowserMainParts>
adrbrowsielContentBrowserClient::CreateBrowserMainParts(
    const content::MainFunctionParams& parameters) {
  std::unique_ptr<content::BrowserMainParts> main_parts =
      ChromeContentBrowserClient::CreateBrowserMainParts(parameters);
  ChromeBrowserMainParts* chrome_main_parts =
      static_cast<ChromeBrowserMainParts*>(main_parts.get());
  chrome_main_parts->AddParts(std::make_unique<adrbrowsielBrowserMainExtraParts>());
  return main_parts;
}

void adrbrowsielContentBrowserClient::BrowserURLHandlerCreated(
    content::BrowserURLHandler* handler) {
#if BUILDFLAG(ENABLE_adrbrowsiel_WEBTORRENT)
  handler->AddHandlerPair(&webtorrent::HandleMagnetURLRewrite,
                          content::BrowserURLHandler::null_handler());
  handler->AddHandlerPair(&webtorrent::HandleTorrentURLRewrite,
                          &webtorrent::HandleTorrentURLReverseRewrite);
#endif
#if BUILDFLAG(IPFS_ENABLED)
  handler->AddHandlerPair(&ipfs::HandleIPFSURLRewrite,
                          &ipfs::HandleIPFSURLReverseRewrite);
#endif
  handler->AddHandlerPair(&HandleURLRewrite, &HandleURLReverseOverrideRewrite);
  ChromeContentBrowserClient::BrowserURLHandlerCreated(handler);
}

void adrbrowsielContentBrowserClient::RenderProcessWillLaunch(
    content::RenderProcessHost* host) {
  Profile* profile = Profile::FromBrowserContext(host->GetBrowserContext());
  adrbrowsielRendererUpdaterFactory::GetForProfile(profile)
      ->InitializeRenderer(host);

  ChromeContentBrowserClient::RenderProcessWillLaunch(host);
}

content::ContentBrowserClient::AllowWebBluetoothResult
adrbrowsielContentBrowserClient::AllowWebBluetooth(
    content::BrowserContext* browser_context,
    const url::Origin& requesting_origin,
    const url::Origin& embedding_origin) {
  return ContentBrowserClient::AllowWebBluetoothResult::BLOCK_GLOBALLY_DISABLED;
}

void adrbrowsielContentBrowserClient::ExposeInterfacesToRenderer(
    service_manager::BinderRegistry* registry,
    blink::AssociatedInterfaceRegistry* associated_registry,
    content::RenderProcessHost* render_process_host) {
  ChromeContentBrowserClient::ExposeInterfacesToRenderer(
      registry, associated_registry, render_process_host);
  registry->AddInterface(
      base::BindRepeating(&BindadrbrowsielSearchHost, render_process_host->GetID()),
      content::GetUIThreadTaskRunner({}));
}

void adrbrowsielContentBrowserClient::RegisterBrowserInterfaceBindersForFrame(
    content::RenderFrameHost* render_frame_host,
    mojo::BinderMapWithContext<content::RenderFrameHost*>* map) {
  ChromeContentBrowserClient::RegisterBrowserInterfaceBindersForFrame(
      render_frame_host, map);
  map->Add<cosmetic_filters::mojom::CosmeticFiltersResources>(
      base::BindRepeating(&BindCosmeticFiltersResources));

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
  if (adrbrowsiel_wallet::IsNativeWalletEnabled()) {
    map->Add<adrbrowsiel_wallet::mojom::adrbrowsielWalletProvider>(
        base::BindRepeating(&MaybeBindadrbrowsielWalletProvider));
  }
#if !defined(OS_ANDROID)
  chrome::internal::RegisterWebUIControllerInterfaceBinder<
      wallet_panel::mojom::PageHandlerFactory, WalletPanelUI>(map);
#endif
#endif
}

bool adrbrowsielContentBrowserClient::HandleExternalProtocol(
    const GURL& url,
    content::WebContents::OnceGetter web_contents_getter,
    int child_id,
    int frame_tree_node_id,
    content::NavigationUIData* navigation_data,
    bool is_main_frame,
    ui::PageTransition page_transition,
    bool has_user_gesture,
    const base::Optional<url::Origin>& initiating_origin,
    mojo::PendingRemote<network::mojom::URLLoaderFactory>* out_factory) {
#if BUILDFLAG(ENABLE_adrbrowsiel_WEBTORRENT)
  if (webtorrent::IsMagnetProtocol(url)) {
    webtorrent::HandleMagnetProtocol(url, std::move(web_contents_getter),
                                     page_transition, has_user_gesture,
                                     initiating_origin);
    return true;
  }
#endif

#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
  if (adrbrowsiel_rewards::IsRewardsProtocol(url)) {
    adrbrowsiel_rewards::HandleRewardsProtocol(url, std::move(web_contents_getter),
                                         page_transition, has_user_gesture);
    return true;
  }
#endif

#if BUILDFLAG(BINANCE_ENABLED)
  if (binance::IsBinanceProtocol(url)) {
    binance::HandleBinanceProtocol(url, std::move(web_contents_getter),
                                   page_transition, has_user_gesture,
                                   initiating_origin);
    return true;
  }
#endif

#if BUILDFLAG(GEMINI_ENABLED)
  if (gemini::IsGeminiProtocol(url)) {
    gemini::HandleGeminiProtocol(url, std::move(web_contents_getter),
                                 page_transition, has_user_gesture,
                                 initiating_origin);
    return true;
  }
#endif

#if BUILDFLAG(ENABLE_FTX)
  if (ftx::IsFTXProtocol(url)) {
    ftx::HandleFTXProtocol(url, std::move(web_contents_getter), page_transition,
                           has_user_gesture, initiating_origin);
    return true;
  }
#endif

  return ChromeContentBrowserClient::HandleExternalProtocol(
      url, std::move(web_contents_getter), child_id, frame_tree_node_id,
      navigation_data, is_main_frame, page_transition, has_user_gesture,
      initiating_origin, out_factory);
}

void adrbrowsielContentBrowserClient::AppendExtraCommandLineSwitches(
    base::CommandLine* command_line,
    int child_process_id) {
  ChromeContentBrowserClient::AppendExtraCommandLineSwitches(command_line,
                                                             child_process_id);
  std::string process_type =
      command_line->GetSwitchValueASCII(switches::kProcessType);
  if (process_type == switches::kRendererProcess) {
    uint64_t session_token =
        12345;  // the kinda thing an idiot would have on his luggage
    if (!command_line->HasSwitch(switches::kTestType)) {
      content::RenderProcessHost* process =
          content::RenderProcessHost::FromID(child_process_id);
      Profile* profile =
          process ? Profile::FromBrowserContext(process->GetBrowserContext())
                  : nullptr;
      if (profile && !profile->IsOffTheRecord()) {
        session_token = session_token_;
      } else {
        session_token = incognito_session_token_;
      }
    }
    command_line->AppendSwitchASCII("adrbrowsiel_session_token",
                                    base::NumberToString(session_token));
  }
}

std::vector<std::unique_ptr<blink::URLLoaderThrottle>>
adrbrowsielContentBrowserClient::CreateURLLoaderThrottles(
    const network::ResourceRequest& request,
    content::BrowserContext* browser_context,
    const base::RepeatingCallback<content::WebContents*()>& wc_getter,
    content::NavigationUIData* navigation_ui_data,
    int frame_tree_node_id) {
  auto result = ChromeContentBrowserClient::CreateURLLoaderThrottles(
      request, browser_context, wc_getter, navigation_ui_data,
      frame_tree_node_id);
#if BUILDFLAG(ENABLE_SPEEDREADER)
  content::WebContents* contents = wc_getter.Run();
  if (!contents) {
    return result;
  }
  auto* tab_helper =
      speedreader::SpeedreaderTabHelper::FromWebContents(contents);
  if (tab_helper && tab_helper->IsActiveForMainFrame() &&
      request.resource_type ==
          static_cast<int>(blink::mojom::ResourceType::kMainFrame)) {
    result.push_back(std::make_unique<speedreader::SpeedReaderThrottle>(
        g_adrbrowsiel_browser_process->speedreader_rewriter_service(),
        base::ThreadTaskRunnerHandle::Get()));
  }
#endif  // ENABLE_SPEEDREADER
  return result;
}

bool adrbrowsielContentBrowserClient::WillCreateURLLoaderFactory(
    content::BrowserContext* browser_context,
    content::RenderFrameHost* frame,
    int render_process_id,
    URLLoaderFactoryType type,
    const url::Origin& request_initiator,
    base::Optional<int64_t> navigation_id,
    ukm::SourceIdObj ukm_source_id,
    mojo::PendingReceiver<network::mojom::URLLoaderFactory>* factory_receiver,
    mojo::PendingRemote<network::mojom::TrustedURLLoaderHeaderClient>*
        header_client,
    bool* bypass_redirect_checks,
    bool* disable_secure_dns,
    network::mojom::URLLoaderFactoryOverridePtr* factory_override) {
  bool use_proxy = false;
  // TODO(iefremov): Skip proxying for certain requests?
  use_proxy = adrbrowsielProxyingURLLoaderFactory::MaybeProxyRequest(
      browser_context, frame,
      type == URLLoaderFactoryType::kNavigation ? -1 : render_process_id,
      factory_receiver);

  use_proxy |= ChromeContentBrowserClient::WillCreateURLLoaderFactory(
      browser_context, frame, render_process_id, type, request_initiator,
      std::move(navigation_id), ukm_source_id, factory_receiver, header_client,
      bypass_redirect_checks, disable_secure_dns, factory_override);

  return use_proxy;
}

bool adrbrowsielContentBrowserClient::WillInterceptWebSocket(
    content::RenderFrameHost* frame) {
  return (frame != nullptr);
}

void adrbrowsielContentBrowserClient::CreateWebSocket(
    content::RenderFrameHost* frame,
    content::ContentBrowserClient::WebSocketFactory factory,
    const GURL& url,
    const net::SiteForCookies& site_for_cookies,
    const base::Optional<std::string>& user_agent,
    mojo::PendingRemote<network::mojom::WebSocketHandshakeClient>
        handshake_client) {
  auto* proxy = adrbrowsielProxyingWebSocket::ProxyWebSocket(
      frame, std::move(factory), url, site_for_cookies, user_agent,
      std::move(handshake_client));

  if (ChromeContentBrowserClient::WillInterceptWebSocket(frame)) {
    ChromeContentBrowserClient::CreateWebSocket(
        frame, proxy->web_socket_factory(), url, site_for_cookies, user_agent,
        proxy->handshake_client().Unbind());
  } else {
    proxy->Start();
  }
}

void adrbrowsielContentBrowserClient::MaybeHideReferrer(
    content::BrowserContext* browser_context,
    const GURL& request_url,
    const GURL& document_url,
    blink::mojom::ReferrerPtr* referrer) {
  DCHECK(referrer && !referrer->is_null());
#if BUILDFLAG(ENABLE_EXTENSIONS)
  if (document_url.SchemeIs(kChromeExtensionScheme) ||
      request_url.SchemeIs(kChromeExtensionScheme)) {
    return;
  }
#endif
  if (document_url.SchemeIs(content::kChromeUIScheme) ||
      request_url.SchemeIs(content::kChromeUIScheme)) {
    return;
  }

  Profile* profile = Profile::FromBrowserContext(browser_context);
  const bool allow_referrers = adrbrowsiel_shields::AllowReferrers(
      HostContentSettingsMapFactory::GetForProfile(profile), document_url);
  const bool shields_up = adrbrowsiel_shields::GetadrbrowsielShieldsEnabled(
      HostContentSettingsMapFactory::GetForProfile(profile), document_url);

  content::Referrer new_referrer;
  if (adrbrowsiel_shields::MaybeChangeReferrer(allow_referrers, shields_up,
                                         (*referrer)->url, request_url,
                                         &new_referrer)) {
    (*referrer)->url = new_referrer.url;
    (*referrer)->policy = new_referrer.policy;
  }
}

GURL adrbrowsielContentBrowserClient::GetEffectiveURL(
    content::BrowserContext* browser_context,
    const GURL& url) {
  Profile* profile = Profile::FromBrowserContext(browser_context);
  if (!profile)
    return url;

#if BUILDFLAG(ENABLE_EXTENSIONS)
  return ChromeContentBrowserClientExtensionsPart::GetEffectiveURL(profile,
                                                                   url);
#else
  return url;
#endif
}

// [static]
bool adrbrowsielContentBrowserClient::HandleURLOverrideRewrite(
    GURL* url,
    content::BrowserContext* browser_context) {
  if (url->host() == chrome::kChromeUISyncHost) {
    GURL::Replacements replacements;
    replacements.SetHostStr(chrome::kChromeUISettingsHost);
    replacements.SetPathStr(kadrbrowsielSyncPath);
    *url = url->ReplaceComponents(replacements);
    return true;
  }

  // no special win10 welcome page
  if (url->host() == chrome::kChromeUIWelcomeHost) {
    *url = GURL(chrome::kChromeUIWelcomeURL);
    return true;
  }

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED) && BUILDFLAG(ENABLE_EXTENSIONS)
  // If the Crypto Wallets extension is loaded, then it replaces the WebUI
  auto* service = adrbrowsielWalletServiceFactory::GetForContext(browser_context);
  if (service->IsCryptoWalletsReady() &&
      url->SchemeIs(content::kChromeUIScheme) &&
      url->host() == ethereum_remote_client_host) {
    auto* registry = extensions::ExtensionRegistry::Get(browser_context);
    if (registry->ready_extensions().GetByID(
            ethereum_remote_client_extension_id)) {
      *url = GURL(ethereum_remote_client_base_url);
      return true;
    }
  }
#endif

  return false;
}

std::vector<std::unique_ptr<content::NavigationThrottle>>
adrbrowsielContentBrowserClient::CreateThrottlesForNavigation(
    content::NavigationHandle* handle) {
  std::vector<std::unique_ptr<content::NavigationThrottle>> throttles =
      ChromeContentBrowserClient::CreateThrottlesForNavigation(handle);

#if !defined(OS_ANDROID)
  std::unique_ptr<content::NavigationThrottle> ntp_shows_navigation_throttle =
      NewTabShowsNavigationThrottle::MaybeCreateThrottleFor(handle);
  if (ntp_shows_navigation_throttle)
    throttles.push_back(std::move(ntp_shows_navigation_throttle));
#endif

#if BUILDFLAG(ENABLE_adrbrowsiel_WEBTORRENT)
  throttles.push_back(
      std::make_unique<extensions::adrbrowsielWebTorrentNavigationThrottle>(handle));
#endif

  content::BrowserContext* context =
      handle->GetWebContents()->GetBrowserContext();

#if BUILDFLAG(ENABLE_TOR)
  std::unique_ptr<content::NavigationThrottle> tor_navigation_throttle =
      tor::TorNavigationThrottle::MaybeCreateThrottleFor(handle,
                                                         context->IsTor());
  if (tor_navigation_throttle)
    throttles.push_back(std::move(tor_navigation_throttle));
  std::unique_ptr<tor::OnionLocationNavigationThrottleDelegate>
      onion_location_navigation_throttle_delegate =
          std::make_unique<tor::OnionLocationNavigationThrottleDelegate>();
  std::unique_ptr<content::NavigationThrottle>
      onion_location_navigation_throttle =
          tor::OnionLocationNavigationThrottle::MaybeCreateThrottleFor(
              handle, TorProfileServiceFactory::IsTorDisabled(),
              std::move(onion_location_navigation_throttle_delegate),
              context->IsTor());
  if (onion_location_navigation_throttle)
    throttles.push_back(std::move(onion_location_navigation_throttle));
#endif

#if BUILDFLAG(IPFS_ENABLED)
  std::unique_ptr<content::NavigationThrottle> ipfs_navigation_throttle =
      ipfs::IpfsNavigationThrottle::MaybeCreateThrottleFor(
          handle, ipfs::IpfsServiceFactory::GetForContext(context),
          g_browser_process->GetApplicationLocale());
  if (ipfs_navigation_throttle)
    throttles.push_back(std::move(ipfs_navigation_throttle));
#endif

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
  std::unique_ptr<content::NavigationThrottle>
      decentralized_dns_navigation_throttle =
          decentralized_dns::DecentralizedDnsNavigationThrottle::
              MaybeCreateThrottleFor(handle, g_browser_process->local_state(),
                                     g_browser_process->GetApplicationLocale());
  if (decentralized_dns_navigation_throttle)
    throttles.push_back(std::move(decentralized_dns_navigation_throttle));
#endif

  if (std::unique_ptr<
          content::NavigationThrottle> domain_block_navigation_throttle =
          adrbrowsiel_shields::DomainBlockNavigationThrottle::MaybeCreateThrottleFor(
              handle, g_adrbrowsiel_browser_process->ad_block_service(),
              g_adrbrowsiel_browser_process->ad_block_custom_filters_service(),
              HostContentSettingsMapFactory::GetForProfile(
                  Profile::FromBrowserContext(context)),
              g_browser_process->GetApplicationLocale()))
    throttles.push_back(std::move(domain_block_navigation_throttle));

  return throttles;
}