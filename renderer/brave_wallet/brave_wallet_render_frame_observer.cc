/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/renderer/adrbrowsiel_wallet/adrbrowsiel_wallet_render_frame_observer.h"

#include "content/public/renderer/render_frame.h"
#include "third_party/blink/public/web/web_local_frame.h"

namespace adrbrowsiel_wallet {

adrbrowsielWalletRenderFrameObserver::adrbrowsielWalletRenderFrameObserver(
    content::RenderFrame* render_frame,
    adrbrowsiel::mojom::DynamicParams dynamic_params)
    : RenderFrameObserver(render_frame), dynamic_params_(dynamic_params) {
  native_javascript_handle_.reset(new adrbrowsielWalletJSHandler(render_frame));
}

adrbrowsielWalletRenderFrameObserver::~adrbrowsielWalletRenderFrameObserver() {}

void adrbrowsielWalletRenderFrameObserver::DidStartNavigation(
    const GURL& url,
    base::Optional<blink::WebNavigationType> navigation_type) {
  url_ = url;
}

void adrbrowsielWalletRenderFrameObserver::DidCreateScriptContext(
    v8::Local<v8::Context> context,
    int32_t world_id) {
  // There could be empty, invalid and "about:blank" URLs,
  // they should fallback to the main frame rules
  if (url_.is_empty() || !url_.is_valid() || url_.spec() == "about:blank")
    url_ = url::Origin(render_frame()->GetWebFrame()->GetSecurityOrigin())
               .GetURL();

  if (!dynamic_params_.adrbrowsiel_use_native_wallet || !native_javascript_handle_ ||
      !url_.SchemeIsHTTPOrHTTPS())
    return;

  native_javascript_handle_->AddJavaScriptObjectToFrame(context);
}

void adrbrowsielWalletRenderFrameObserver::OnDestruct() {
  delete this;
}

}  // namespace adrbrowsiel_wallet
