/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_RENDERER_adrbrowsiel_CONTENT_RENDERER_CLIENT_H_
#define adrbrowsiel_RENDERER_adrbrowsiel_CONTENT_RENDERER_CLIENT_H_

#include <memory>

#include "adrbrowsiel/components/adrbrowsiel_search/renderer/adrbrowsiel_search_service_worker_holder.h"
#include "chrome/renderer/chrome_content_renderer_client.h"
#include "v8/include/v8.h"

class adrbrowsielRenderThreadObserver;
class GURL;

namespace blink {
class WebServiceWorkerContextProxy;
}

class adrbrowsielContentRendererClient : public ChromeContentRendererClient {
 public:
  adrbrowsielContentRendererClient();
  ~adrbrowsielContentRendererClient() override;

  void RenderThreadStarted() override;
  void SetRuntimeFeaturesDefaultsBeforeBlinkInitialization() override;
  void RenderFrameCreated(content::RenderFrame* render_frame) override;
  void RunScriptsAtDocumentStart(content::RenderFrame* render_frame) override;
  void WillEvaluateServiceWorkerOnWorkerThread(
      blink::WebServiceWorkerContextProxy* context_proxy,
      v8::Local<v8::Context> v8_context,
      int64_t service_worker_version_id,
      const GURL& service_worker_scope,
      const GURL& script_url) override;
  void WillDestroyServiceWorkerContextOnWorkerThread(
      v8::Local<v8::Context> v8_context,
      int64_t service_worker_version_id,
      const GURL& service_worker_scope,
      const GURL& script_url) override;

 private:
  std::unique_ptr<adrbrowsielRenderThreadObserver> adrbrowsiel_observer_;
  adrbrowsiel_search::adrbrowsielSearchServiceWorkerHolder
      adrbrowsiel_search_service_worker_holder_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielContentRendererClient);
};

#endif  // adrbrowsiel_RENDERER_adrbrowsiel_CONTENT_RENDERER_CLIENT_H_
