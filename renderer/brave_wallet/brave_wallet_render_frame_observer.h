/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_RENDERER_adrbrowsiel_WALLET_adrbrowsiel_WALLET_RENDER_FRAME_OBSERVER_H_
#define adrbrowsiel_RENDERER_adrbrowsiel_WALLET_adrbrowsiel_WALLET_RENDER_FRAME_OBSERVER_H_

#include <memory>

#include "adrbrowsiel/common/adrbrowsiel_renderer_configuration.mojom.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/renderer/adrbrowsiel_wallet_js_handler.h"
#include "content/public/renderer/render_frame.h"
#include "content/public/renderer/render_frame_observer.h"
#include "third_party/blink/public/web/web_navigation_type.h"
#include "url/gurl.h"
#include "v8/include/v8.h"

namespace adrbrowsiel_wallet {

class adrbrowsielWalletRenderFrameObserver : public content::RenderFrameObserver {
 public:
  explicit adrbrowsielWalletRenderFrameObserver(
      content::RenderFrame* render_frame,
      adrbrowsiel::mojom::DynamicParams dynamic_params);
  ~adrbrowsielWalletRenderFrameObserver() override;

  // RenderFrameObserver implementation.
  void DidStartNavigation(
      const GURL& url,
      base::Optional<blink::WebNavigationType> navigation_type) override;
  void DidCreateScriptContext(v8::Local<v8::Context> context,
                              int32_t world_id) override;

 private:
  // RenderFrameObserver implementation.
  void OnDestruct() override;

  // Handle to "handler" JavaScript object functionality.
  std::unique_ptr<adrbrowsielWalletJSHandler> native_javascript_handle_;

  GURL url_;
  const adrbrowsiel::mojom::DynamicParams dynamic_params_;
};

}  // namespace adrbrowsiel_wallet

#endif  // adrbrowsiel_RENDERER_adrbrowsiel_WALLET_adrbrowsiel_WALLET_RENDER_FRAME_OBSERVER_H_
