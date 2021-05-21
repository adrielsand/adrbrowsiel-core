/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/renderer/adrbrowsiel_render_thread_observer.h"

#include <utility>

#include "base/logging.h"
#include "base/no_destructor.h"
#include "third_party/blink/public/common/associated_interfaces/associated_interface_registry.h"

namespace {

adrbrowsiel::mojom::DynamicParams* GetDynamicConfigParams() {
  static base::NoDestructor<adrbrowsiel::mojom::DynamicParams> dynamic_params;
  return dynamic_params.get();
}

}  // namespace

adrbrowsielRenderThreadObserver::adrbrowsielRenderThreadObserver() {}

adrbrowsielRenderThreadObserver::~adrbrowsielRenderThreadObserver() {}

// static
const adrbrowsiel::mojom::DynamicParams&
adrbrowsielRenderThreadObserver::GetDynamicParams() {
  return *GetDynamicConfigParams();
}

void adrbrowsielRenderThreadObserver::RegisterMojoInterfaces(
    blink::AssociatedInterfaceRegistry* associated_interfaces) {
  associated_interfaces->AddInterface(base::BindRepeating(
      &adrbrowsielRenderThreadObserver::OnRendererConfigurationAssociatedRequest,
      base::Unretained(this)));
}

void adrbrowsielRenderThreadObserver::UnregisterMojoInterfaces(
    blink::AssociatedInterfaceRegistry* associated_interfaces) {
  associated_interfaces->RemoveInterface(
      adrbrowsiel::mojom::adrbrowsielRendererConfiguration::Name_);
}

void adrbrowsielRenderThreadObserver::OnRendererConfigurationAssociatedRequest(
    mojo::PendingAssociatedReceiver<adrbrowsiel::mojom::adrbrowsielRendererConfiguration>
        receiver) {
  renderer_configuration_receivers_.Add(this, std::move(receiver));
}

void adrbrowsielRenderThreadObserver::SetConfiguration(
    adrbrowsiel::mojom::DynamicParamsPtr params) {
  *GetDynamicConfigParams() = std::move(*params);
}
