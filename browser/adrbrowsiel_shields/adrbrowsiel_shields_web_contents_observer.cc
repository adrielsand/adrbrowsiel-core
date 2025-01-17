/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/adrbrowsiel_shields/adrbrowsiel_shields_web_contents_observer.h"

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/no_destructor.h"
#include "base/strings/utf_string_conversions.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_perf_predictor/browser/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/adrbrowsiel_shields_util.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/common/adrbrowsiel_shield_constants.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/renderer_configuration.mojom.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/content_settings/core/common/content_settings_utils.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/navigation_handle.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/render_process_host.h"
#include "content/public/browser/web_contents.h"
#include "extensions/buildflags/buildflags.h"
#include "ipc/ipc_message_macros.h"
#include "mojo/public/cpp/bindings/associated_remote.h"
#include "third_party/blink/public/common/associated_interfaces/associated_interface_provider.h"

#if BUILDFLAG(ENABLE_adrbrowsiel_PERF_PREDICTOR)
#include "adrbrowsiel/components/adrbrowsiel_perf_predictor/browser/perf_predictor_tab_helper.h"
#endif

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "adrbrowsiel/common/extensions/api/adrbrowsiel_shields.h"
#include "chrome/browser/extensions/extension_tab_util.h"
#include "extensions/browser/event_router.h"
#include "extensions/browser/extension_api_frame_id_map.h"

using extensions::Event;
using extensions::EventRouter;
#endif

using content::Referrer;
using content::RenderFrameHost;
using content::WebContents;

namespace {

// Content Settings are only sent to the main frame currently. Chrome may fix
// this at some point, but for now we do this as a work-around. You can verify
// if this is fixed by running the following test: npm run test --
// adrbrowsiel_browser_tests --filter=adrbrowsielContentSettingsAgentImplBrowserTest.*
// Chrome seems to also have a bug with RenderFrameHostChanged not updating the
// content settings so this is fixed here too. That case is covered in tests by:
// npm run test -- adrbrowsiel_browser_tests
// --filter=adrbrowsielContentSettingsAgentImplBrowserTest.*
void UpdateContentSettingsToRendererFrames(content::WebContents* web_contents) {
  for (content::RenderFrameHost* frame : web_contents->GetAllFrames()) {
    Profile* profile =
        Profile::FromBrowserContext(web_contents->GetBrowserContext());
    const HostContentSettingsMap* map =
        HostContentSettingsMapFactory::GetForProfile(profile);
    RendererContentSettingRules rules;
    GetRendererContentSettingRules(map, &rules);
    IPC::ChannelProxy* channel = frame->GetProcess()->GetChannel();
    // channel might be NULL in tests.
    if (channel) {
      mojo::AssociatedRemote<chrome::mojom::RendererConfiguration> rc_interface;
      channel->GetRemoteAssociatedInterface(&rc_interface);
      rc_interface->SetContentSettingRules(rules);
    }
  }
}

}  // namespace

namespace adrbrowsiel_shields {

base::NoDestructor<std::map<int, GURL>> frame_tree_node_id_to_tab_url_;

adrbrowsielShieldsWebContentsObserver::~adrbrowsielShieldsWebContentsObserver() {
  adrbrowsiel_shields_remotes_.clear();
}

adrbrowsielShieldsWebContentsObserver::adrbrowsielShieldsWebContentsObserver(
    WebContents* web_contents)
    : WebContentsObserver(web_contents),
      adrbrowsiel_shields_receivers_(web_contents, this) {}

void adrbrowsielShieldsWebContentsObserver::RenderFrameCreated(RenderFrameHost* rfh) {
  if (rfh && allowed_script_origins_.size()) {
    GetadrbrowsielShieldsRemote(rfh)->SetAllowScriptsFromOriginsOnce(
        allowed_script_origins_);
  }

  WebContents* web_contents = WebContents::FromRenderFrameHost(rfh);
  if (web_contents) {
    UpdateContentSettingsToRendererFrames(web_contents);

    (*frame_tree_node_id_to_tab_url_)[rfh->GetFrameTreeNodeId()] =
        web_contents->GetURL();
  }
}

void adrbrowsielShieldsWebContentsObserver::RenderFrameDeleted(RenderFrameHost* rfh) {
  (*frame_tree_node_id_to_tab_url_).erase(rfh->GetFrameTreeNodeId());
  adrbrowsiel_shields_remotes_.erase(rfh);
}

void adrbrowsielShieldsWebContentsObserver::RenderFrameHostChanged(
    RenderFrameHost* old_host,
    RenderFrameHost* new_host) {
  if (old_host) {
    RenderFrameDeleted(old_host);
  }
  if (new_host) {
    RenderFrameCreated(new_host);
  }
}

void adrbrowsielShieldsWebContentsObserver::DidFinishNavigation(
    content::NavigationHandle* navigation_handle) {
  RenderFrameHost* main_frame = web_contents()->GetMainFrame();
  if (!web_contents() || !main_frame) {
    return;
  }
  int tree_node_id = main_frame->GetFrameTreeNodeId();

  (*frame_tree_node_id_to_tab_url_)[tree_node_id] = web_contents()->GetURL();
}

// static
GURL adrbrowsielShieldsWebContentsObserver::GetTabURLFromRenderFrameInfo(
    int render_frame_tree_node_id) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  if (render_frame_tree_node_id != RenderFrameHost::kNoFrameTreeNodeId) {
    auto iter =
        (*frame_tree_node_id_to_tab_url_).find(render_frame_tree_node_id);
    if (iter != (*frame_tree_node_id_to_tab_url_).end()) {
      return iter->second;
    }
  }
  return GURL();
}

bool adrbrowsielShieldsWebContentsObserver::IsBlockedSubresource(
    const std::string& subresource) {
  return blocked_url_paths_.find(subresource) != blocked_url_paths_.end();
}

void adrbrowsielShieldsWebContentsObserver::AddBlockedSubresource(
    const std::string& subresource) {
  blocked_url_paths_.insert(subresource);
}

// static
void adrbrowsielShieldsWebContentsObserver::DispatchBlockedEvent(
    const GURL& request_url,
    int frame_tree_node_id,
    const std::string& block_type) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  auto subresource = request_url.spec();
  WebContents* web_contents =
      WebContents::FromFrameTreeNodeId(frame_tree_node_id);
  DispatchBlockedEventForWebContents(block_type, subresource, web_contents);

  if (web_contents) {
    adrbrowsielShieldsWebContentsObserver* observer =
        adrbrowsielShieldsWebContentsObserver::FromWebContents(web_contents);
    if (observer && !observer->IsBlockedSubresource(subresource)) {
      observer->AddBlockedSubresource(subresource);
      PrefService* prefs =
          Profile::FromBrowserContext(web_contents->GetBrowserContext())
              ->GetOriginalProfile()
              ->GetPrefs();

      if (block_type == kAds) {
        prefs->SetUint64(kAdsBlocked, prefs->GetUint64(kAdsBlocked) + 1);
      } else if (block_type == kHTTPUpgradableResources) {
        prefs->SetUint64(kHttpsUpgrades, prefs->GetUint64(kHttpsUpgrades) + 1);
      } else if (block_type == kJavaScript) {
        prefs->SetUint64(kJavascriptBlocked,
                         prefs->GetUint64(kJavascriptBlocked) + 1);
      } else if (block_type == kFingerprintingV2) {
        prefs->SetUint64(kFingerprintingBlocked,
                         prefs->GetUint64(kFingerprintingBlocked) + 1);
      }
    }
  }
#if BUILDFLAG(ENABLE_adrbrowsiel_PERF_PREDICTOR)
  adrbrowsiel_perf_predictor::PerfPredictorTabHelper::DispatchBlockedEvent(
      request_url.spec(), frame_tree_node_id);
#endif
}

#if !defined(OS_ANDROID)
// static
void adrbrowsielShieldsWebContentsObserver::DispatchBlockedEventForWebContents(
    const std::string& block_type,
    const std::string& subresource,
    WebContents* web_contents) {
#if BUILDFLAG(ENABLE_EXTENSIONS)
  if (!web_contents) {
    return;
  }
  EventRouter* event_router =
      EventRouter::Get(web_contents->GetBrowserContext());
  if (event_router) {
    extensions::api::adrbrowsiel_shields::OnBlocked::Details details;
    details.tab_id = extensions::ExtensionTabUtil::GetTabId(web_contents);
    details.block_type = block_type;
    details.subresource = subresource;
    std::unique_ptr<base::ListValue> args(
        extensions::api::adrbrowsiel_shields::OnBlocked::Create(details).release());
    std::unique_ptr<Event> event(
        new Event(extensions::events::adrbrowsiel_AD_BLOCKED,
                  extensions::api::adrbrowsiel_shields::OnBlocked::kEventName,
                  std::move(args)));
    event_router->BroadcastEvent(std::move(event));
  }
#endif
}
#endif

void adrbrowsielShieldsWebContentsObserver::OnJavaScriptBlocked(
    const std::u16string& details) {
  WebContents* web_contents = WebContents::FromRenderFrameHost(
      adrbrowsiel_shields_receivers_.GetCurrentTargetFrame());
  if (!web_contents)
    return;

  DispatchBlockedEventForWebContents(adrbrowsiel_shields::kJavaScript,
                                     base::UTF16ToUTF8(details), web_contents);
}

// static
void adrbrowsielShieldsWebContentsObserver::RegisterProfilePrefs(
    PrefRegistrySimple* registry) {
  registry->RegisterUint64Pref(kAdsBlocked, 0);
  registry->RegisterUint64Pref(kTrackersBlocked, 0);
  registry->RegisterUint64Pref(kJavascriptBlocked, 0);
  registry->RegisterUint64Pref(kHttpsUpgrades, 0);
  registry->RegisterUint64Pref(kFingerprintingBlocked, 0);
}

void adrbrowsielShieldsWebContentsObserver::ReadyToCommitNavigation(
    content::NavigationHandle* navigation_handle) {
  // when the main frame navigate away
  content::ReloadType reload_type = navigation_handle->GetReloadType();
  if (navigation_handle->IsInMainFrame() &&
      !navigation_handle->IsSameDocument()) {
    if (reload_type == content::ReloadType::NONE) {
      // For new loads, we reset the counters for both blocked scripts and URLs.
      allowed_script_origins_.clear();
      blocked_url_paths_.clear();
    } else if (reload_type == content::ReloadType::NORMAL) {
      // For normal reloads (or loads to the current URL, internally converted
      // into reloads i.e see NavigationControllerImpl::NavigateWithoutEntry),
      // we only reset the counter for blocked URLs, not the one for scripts.
      blocked_url_paths_.clear();
    }
  }

  auto render_frame_hosts = navigation_handle->GetWebContents()->GetAllFrames();
  for (content::RenderFrameHost* rfh : render_frame_hosts) {
    GetadrbrowsielShieldsRemote(rfh)->SetAllowScriptsFromOriginsOnce(
        allowed_script_origins_);
  }
}

void adrbrowsielShieldsWebContentsObserver::AllowScriptsOnce(
    const std::vector<std::string>& origins,
    WebContents* contents) {
  allowed_script_origins_ = std::move(origins);
}

mojo::AssociatedRemote<adrbrowsiel_shields::mojom::adrbrowsielShields>&
adrbrowsielShieldsWebContentsObserver::GetadrbrowsielShieldsRemote(
    content::RenderFrameHost* rfh) {
  if (!adrbrowsiel_shields_remotes_.contains(rfh)) {
    rfh->GetRemoteAssociatedInterfaces()->GetInterface(
        &adrbrowsiel_shields_remotes_[rfh]);
  }

  DCHECK(adrbrowsiel_shields_remotes_[rfh].is_bound());
  return adrbrowsiel_shields_remotes_[rfh];
}

WEB_CONTENTS_USER_DATA_KEY_IMPL(adrbrowsielShieldsWebContentsObserver)

}  // namespace adrbrowsiel_shields
