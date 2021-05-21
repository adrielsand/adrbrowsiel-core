/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_RENDERER_adrbrowsiel_RENDER_THREAD_OBSERVER_H_
#define adrbrowsiel_RENDERER_adrbrowsiel_RENDER_THREAD_OBSERVER_H_

#include "adrbrowsiel/common/adrbrowsiel_renderer_configuration.mojom.h"
#include "content/public/renderer/render_thread_observer.h"
#include "mojo/public/cpp/bindings/associated_receiver_set.h"
#include "mojo/public/cpp/bindings/pending_associated_receiver.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"

namespace blink {
class AssociatedInterfaceRegistry;
}

class adrbrowsielRenderThreadObserver
    : public content::RenderThreadObserver,
      public adrbrowsiel::mojom::adrbrowsielRendererConfiguration {
 public:
  adrbrowsielRenderThreadObserver(const adrbrowsielRenderThreadObserver&) = delete;
  adrbrowsielRenderThreadObserver& operator=(const adrbrowsielRenderThreadObserver&) =
      delete;
  adrbrowsielRenderThreadObserver();
  ~adrbrowsielRenderThreadObserver() override;

  // Return the dynamic parameters - those that may change while the
  // render process is running.
  static const adrbrowsiel::mojom::DynamicParams& GetDynamicParams();

 private:
  // content::RenderThreadObserver:
  void RegisterMojoInterfaces(
      blink::AssociatedInterfaceRegistry* associated_interfaces) override;
  void UnregisterMojoInterfaces(
      blink::AssociatedInterfaceRegistry* associated_interfaces) override;

  // adrbrowsiel::mojom::adrbrowsielRendererConfiguration:
  void SetConfiguration(adrbrowsiel::mojom::DynamicParamsPtr params) override;

  void OnRendererConfigurationAssociatedRequest(
      mojo::PendingAssociatedReceiver<adrbrowsiel::mojom::adrbrowsielRendererConfiguration>
          receiver);

  mojo::AssociatedReceiverSet<adrbrowsiel::mojom::adrbrowsielRendererConfiguration>
      renderer_configuration_receivers_;
};

#endif  // adrbrowsiel_RENDERER_adrbrowsiel_RENDER_THREAD_OBSERVER_H_
