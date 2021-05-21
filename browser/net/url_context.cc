/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/net/url_context.h"

#include <memory>
#include <string>

#include "adrbrowsiel/browser/adrbrowsiel_shields/adrbrowsiel_shields_web_contents_observer.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/adrbrowsiel_shields_util.h"
#include "adrbrowsiel/components/adrbrowsiel_webtorrent/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_webtorrent/browser/webtorrent_util.h"
#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/browser_thread.h"
#include "net/base/isolation_info.h"

#if BUILDFLAG(IPFS_ENABLED)
#include "adrbrowsiel/components/ipfs/ipfs_constants.h"
#include "adrbrowsiel/components/ipfs/ipfs_utils.h"
#include "adrbrowsiel/components/ipfs/pref_names.h"
#include "chrome/common/channel_info.h"
#include "components/prefs/pref_service.h"
#include "components/user_prefs/user_prefs.h"
#endif

namespace adrbrowsiel {

namespace {

std::string GetUploadData(const network::ResourceRequest& request) {
  std::string upload_data;
  if (!request.request_body) {
    return {};
  }
  const auto* elements = request.request_body->elements();
  for (const network::DataElement& element : *elements) {
    if (element.type() == network::mojom::DataElementDataView::Tag::kBytes) {
      const auto& bytes = element.As<network::DataElementBytes>().bytes();
      upload_data.append(bytes.begin(), bytes.end());
    }
  }

  return upload_data;
}

}  // namespace

adrbrowsielRequestInfo::adrbrowsielRequestInfo() = default;

adrbrowsielRequestInfo::adrbrowsielRequestInfo(const GURL& url) : request_url(url) {}

adrbrowsielRequestInfo::~adrbrowsielRequestInfo() = default;

// static
std::shared_ptr<adrbrowsiel::adrbrowsielRequestInfo> adrbrowsielRequestInfo::MakeCTX(
    const network::ResourceRequest& request,
    int render_process_id,
    int frame_tree_node_id,
    uint64_t request_identifier,
    content::BrowserContext* browser_context,
    std::shared_ptr<adrbrowsiel::adrbrowsielRequestInfo> old_ctx) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  auto ctx = std::make_shared<adrbrowsiel::adrbrowsielRequestInfo>();
  ctx->request_identifier = request_identifier;
  ctx->method = request.method;
  ctx->request_url = request.url;
  // TODO(iefremov): Replace GURL with Origin
  ctx->initiator_url =
      request.request_initiator.value_or(url::Origin()).GetURL();

  ctx->referrer = request.referrer;
  ctx->referrer_policy = request.referrer_policy;

  ctx->resource_type =
      static_cast<blink::mojom::ResourceType>(request.resource_type);

  ctx->is_webtorrent_disabled =
#if BUILDFLAG(ENABLE_adrbrowsiel_WEBTORRENT)
      !webtorrent::IsWebtorrentEnabled(browser_context);
#else
      true;
#endif

  ctx->frame_tree_node_id = frame_tree_node_id;

  // TODO(iefremov): remove tab_url. Change tab_origin from GURL to Origin.
  // ctx->tab_url = request.top_frame_origin;
  if (request.trusted_params) {
    // TODO(iefremov): Turns out it provides us a not expected value for
    // cross-site top-level navigations. Fortunately for now it is not a problem
    // for shields functionality. We should reconsider this machinery, also
    // given that this is always empty for subresources.
    ctx->network_isolation_key =
        request.trusted_params->isolation_info.network_isolation_key();
    ctx->tab_origin = request.trusted_params->isolation_info.top_frame_origin()
                          .value_or(url::Origin())
                          .GetURL();
  }
  // TODO(iefremov): We still need this for WebSockets, currently
  // |AddChannelRequest| provides only old-fashioned |site_for_cookies|.
  // (See |adrbrowsielProxyingWebSocket|).
  if (ctx->tab_origin.is_empty()) {
    ctx->tab_origin = adrbrowsiel_shields::adrbrowsielShieldsWebContentsObserver::
                          GetTabURLFromRenderFrameInfo(ctx->frame_tree_node_id)
                              .GetOrigin();
  }

  if (old_ctx) {
    ctx->internal_redirect = old_ctx->internal_redirect;
    ctx->redirect_source = old_ctx->redirect_source;
  }

#if BUILDFLAG(IPFS_ENABLED)
  auto* prefs = user_prefs::UserPrefs::Get(browser_context);
  ctx->ipfs_gateway_url =
      ipfs::GetConfiguredBaseGateway(browser_context, chrome::GetChannel());
  ctx->ipfs_auto_fallback = prefs->GetBoolean(kIPFSAutoRedirectGateway);

  // ipfs:// navigations have no tab origin set, but we want it to be the tab
  // origin of the gateway so that ad-block in particular won't give up early.
  if (ipfs::IsLocalGatewayConfigured(browser_context) &&
      ctx->tab_origin.is_empty() &&
      ipfs::IsLocalGatewayURL(ctx->initiator_url)) {
    ctx->tab_url = ctx->initiator_url;
    ctx->tab_origin = ctx->initiator_url.GetOrigin();
  }
#endif

  Profile* profile = Profile::FromBrowserContext(browser_context);
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile);
  ctx->allow_adrbrowsiel_shields =
      adrbrowsiel_shields::GetadrbrowsielShieldsEnabled(map, ctx->tab_origin);
  ctx->allow_ads = adrbrowsiel_shields::GetAdControlType(map, ctx->tab_origin) ==
                   adrbrowsiel_shields::ControlType::ALLOW;
  ctx->allow_http_upgradable_resource =
      !adrbrowsiel_shields::GetHTTPSEverywhereEnabled(map, ctx->tab_origin);

  // HACK: after we fix multiple creations of adrbrowsielRequestInfo we should
  // use only tab_origin. Since we recreate adrbrowsielRequestInfo during consequent
  // stages of navigation, |tab_origin| changes and so does |allow_referrers|
  // flag, which is not what we want for determining referrers.
  ctx->allow_referrers = adrbrowsiel_shields::AllowReferrers(
      map,
      ctx->redirect_source.is_empty() ? ctx->tab_origin : ctx->redirect_source);
  ctx->upload_data = GetUploadData(request);

  ctx->browser_context = browser_context;

  // TODO(fmarier): remove this once the hacky code in
  // adrbrowsiel_proxying_url_loader_factory.cc is refactored. See
  // adrbrowsielProxyingURLLoaderFactory::InProgressRequest::UpdateRequestInfo().
  if (old_ctx) {
    ctx->internal_redirect = old_ctx->internal_redirect;
    ctx->redirect_source = old_ctx->redirect_source;
  }

  return ctx;
}

}  // namespace adrbrowsiel
