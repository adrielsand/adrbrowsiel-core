/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/renderer/adrbrowsiel_content_renderer_client.h"

#include "base/feature_list.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/common/features.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/buildflags/buildflags.h"
#include "adrbrowsiel/components/cosmetic_filters/renderer/cosmetic_filters_js_render_frame_observer.h"
#include "adrbrowsiel/renderer/adrbrowsiel_render_thread_observer.h"
#include "chrome/common/chrome_isolated_world_ids.h"
#include "content/public/renderer/render_thread.h"
#include "third_party/blink/public/platform/web_runtime_features.h"
#include "third_party/blink/public/web/modules/service_worker/web_service_worker_context_proxy.h"
#include "url/gurl.h"

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/features.h"
#include "adrbrowsiel/renderer/adrbrowsiel_wallet/adrbrowsiel_wallet_render_frame_observer.h"
#endif

adrbrowsielContentRendererClient::adrbrowsielContentRendererClient()
    : ChromeContentRendererClient() {}

void adrbrowsielContentRendererClient::
    SetRuntimeFeaturesDefaultsBeforeBlinkInitialization() {
  ChromeContentRendererClient::
      SetRuntimeFeaturesDefaultsBeforeBlinkInitialization();

  blink::WebRuntimeFeatures::EnableWebNfc(false);

  // These features don't have dedicated WebRuntimeFeatures wrappers.
  blink::WebRuntimeFeatures::EnableFeatureFromString("DigitalGoods", false);
  blink::WebRuntimeFeatures::EnableFeatureFromString("FileSystemAccess", false);
  blink::WebRuntimeFeatures::EnableFeatureFromString(
      "FileSystemAccessAPIExperimental", false);
  blink::WebRuntimeFeatures::EnableFeatureFromString("Serial", false);
}

adrbrowsielContentRendererClient::~adrbrowsielContentRendererClient() = default;

void adrbrowsielContentRendererClient::RenderThreadStarted() {
  ChromeContentRendererClient::RenderThreadStarted();

  adrbrowsiel_observer_ = std::make_unique<adrbrowsielRenderThreadObserver>();
  content::RenderThread::Get()->AddObserver(adrbrowsiel_observer_.get());
  adrbrowsiel_search_service_worker_holder_.SetBrowserInterfaceBrokerProxy(
      browser_interface_broker_.get());
}

void adrbrowsielContentRendererClient::RenderFrameCreated(
    content::RenderFrame* render_frame) {
  ChromeContentRendererClient::RenderFrameCreated(render_frame);

#if !defined(OS_ANDROID) && !defined(CHROME_OS)
  if (base::FeatureList::IsEnabled(
          adrbrowsiel_shields::features::kadrbrowsielAdblockCosmeticFilteringNative))
#endif
    new cosmetic_filters::CosmeticFiltersJsRenderFrameObserver(
        render_frame, ISOLATED_WORLD_ID_adrbrowsiel_INTERNAL);

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
  if (base::FeatureList::IsEnabled(
          adrbrowsiel_wallet::features::kNativeadrbrowsielWalletFeature)) {
    new adrbrowsiel_wallet::adrbrowsielWalletRenderFrameObserver(
        render_frame, adrbrowsielRenderThreadObserver::GetDynamicParams());
  }
#endif
}

void adrbrowsielContentRendererClient::RunScriptsAtDocumentStart(
    content::RenderFrame* render_frame) {
  auto* observer =
      cosmetic_filters::CosmeticFiltersJsRenderFrameObserver::Get(render_frame);
  // Run this before any extensions
  if (observer)
    observer->RunScriptsAtDocumentStart();

  ChromeContentRendererClient::RunScriptsAtDocumentStart(render_frame);
}

void adrbrowsielContentRendererClient::WillEvaluateServiceWorkerOnWorkerThread(
    blink::WebServiceWorkerContextProxy* context_proxy,
    v8::Local<v8::Context> v8_context,
    int64_t service_worker_version_id,
    const GURL& service_worker_scope,
    const GURL& script_url) {
  adrbrowsiel_search_service_worker_holder_.WillEvaluateServiceWorkerOnWorkerThread(
      context_proxy, v8_context, service_worker_version_id,
      service_worker_scope, script_url);
  ChromeContentRendererClient::WillEvaluateServiceWorkerOnWorkerThread(
      context_proxy, v8_context, service_worker_version_id,
      service_worker_scope, script_url);
}

void adrbrowsielContentRendererClient::WillDestroyServiceWorkerContextOnWorkerThread(
    v8::Local<v8::Context> v8_context,
    int64_t service_worker_version_id,
    const GURL& service_worker_scope,
    const GURL& script_url) {
  adrbrowsiel_search_service_worker_holder_
      .WillDestroyServiceWorkerContextOnWorkerThread(
          v8_context, service_worker_version_id, service_worker_scope,
          script_url);
  ChromeContentRendererClient::WillDestroyServiceWorkerContextOnWorkerThread(
      v8_context, service_worker_version_id, service_worker_scope, script_url);
}
