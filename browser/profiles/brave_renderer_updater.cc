/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/profiles/adrbrowsiel_renderer_updater.h"

#include <utility>

#include "base/bind.h"
#include "adrbrowsiel/common/adrbrowsiel_renderer_configuration.mojom.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/buildflags/buildflags.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/pref_names.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/render_process_host.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/adrbrowsiel_wallet_constants.h"
#include "adrbrowsiel/browser/adrbrowsiel_wallet/adrbrowsiel_wallet_context_utils.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/browser/pref_names.h"
#endif

adrbrowsielRendererUpdater::adrbrowsielRendererUpdater(Profile* profile)
    : profile_(profile) {
  PrefService* pref_service = profile->GetPrefs();
  adrbrowsiel_wallet_web3_provider_.Init(kadrbrowsielWalletWeb3Provider, pref_service);

  pref_change_registrar_.Init(pref_service);
#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
  pref_change_registrar_.Add(
      kadrbrowsielWalletWeb3Provider,
      base::BindRepeating(&adrbrowsielRendererUpdater::UpdateAllRenderers,
                          base::Unretained(this)));
#endif
}

adrbrowsielRendererUpdater::~adrbrowsielRendererUpdater() {}

void adrbrowsielRendererUpdater::InitializeRenderer(
    content::RenderProcessHost* render_process_host) {
  auto renderer_configuration = GetRendererConfiguration(render_process_host);
  UpdateRenderer(&renderer_configuration);
}

std::vector<mojo::AssociatedRemote<adrbrowsiel::mojom::adrbrowsielRendererConfiguration>>
adrbrowsielRendererUpdater::GetRendererConfigurations() {
  std::vector<mojo::AssociatedRemote<adrbrowsiel::mojom::adrbrowsielRendererConfiguration>>
      rv;
  for (content::RenderProcessHost::iterator it(
           content::RenderProcessHost::AllHostsIterator());
       !it.IsAtEnd(); it.Advance()) {
    Profile* renderer_profile =
        static_cast<Profile*>(it.GetCurrentValue()->GetBrowserContext());
    if (renderer_profile == profile_ ||
        renderer_profile->GetOriginalProfile() == profile_) {
      auto renderer_configuration =
          GetRendererConfiguration(it.GetCurrentValue());
      if (renderer_configuration)
        rv.push_back(std::move(renderer_configuration));
    }
  }
  return rv;
}

mojo::AssociatedRemote<adrbrowsiel::mojom::adrbrowsielRendererConfiguration>
adrbrowsielRendererUpdater::GetRendererConfiguration(
    content::RenderProcessHost* render_process_host) {
  IPC::ChannelProxy* channel = render_process_host->GetChannel();
  if (!channel)
    return mojo::AssociatedRemote<adrbrowsiel::mojom::adrbrowsielRendererConfiguration>();

  mojo::AssociatedRemote<adrbrowsiel::mojom::adrbrowsielRendererConfiguration>
      renderer_configuration;
  channel->GetRemoteAssociatedInterface(&renderer_configuration);

  return renderer_configuration;
}

void adrbrowsielRendererUpdater::UpdateAllRenderers() {
  auto renderer_configurations = GetRendererConfigurations();
  for (auto& renderer_configuration : renderer_configurations)
    UpdateRenderer(&renderer_configuration);
}

void adrbrowsielRendererUpdater::UpdateRenderer(
    mojo::AssociatedRemote<adrbrowsiel::mojom::adrbrowsielRendererConfiguration>*
        renderer_configuration) {
#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
  bool use_adrbrowsiel_web3_provider = (static_cast<adrbrowsielWalletWeb3ProviderTypes>(
                                      adrbrowsiel_wallet_web3_provider_.GetValue()) ==
                                  adrbrowsielWalletWeb3ProviderTypes::adrbrowsiel_WALLET) &&
                                 adrbrowsiel_wallet::IsAllowedForContext(profile_);

  (*renderer_configuration)
      ->SetConfiguration(
          adrbrowsiel::mojom::DynamicParams::New(use_adrbrowsiel_web3_provider));
#endif
}
