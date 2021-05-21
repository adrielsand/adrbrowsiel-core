/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/browser_context_keyed_service_factories.h"

#include "adrbrowsiel/browser/adrbrowsiel_ads/ads_service_factory.h"
#include "adrbrowsiel/browser/adrbrowsiel_shields/ad_block_pref_service_factory.h"
#include "adrbrowsiel/browser/adrbrowsiel_shields/cookie_pref_service_factory.h"
#include "adrbrowsiel/browser/ntp_background_images/view_counter_service_factory.h"
#include "adrbrowsiel/browser/permissions/permission_lifetime_manager_factory.h"
#include "adrbrowsiel/browser/search_engines/search_engine_provider_service_factory.h"
#include "adrbrowsiel/browser/search_engines/search_engine_tracker.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/buildflags/buildflags.h"
#include "adrbrowsiel/components/greaselion/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"
#include "adrbrowsiel/components/tor/buildflags/buildflags.h"

#if BUILDFLAG(ENABLE_GREASELION)
#include "adrbrowsiel/browser/greaselion/greaselion_service_factory.h"
#endif

#if !defined(OS_ANDROID)
#include "adrbrowsiel/browser/ui/bookmark/bookmark_prefs_service_factory.h"
#else
#include "adrbrowsiel/browser/ntp_background_images/android/ntp_background_images_bridge.h"
#endif

#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
#include "adrbrowsiel/browser/adrbrowsiel_rewards/rewards_service_factory.h"
#endif

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
#include "adrbrowsiel/browser/adrbrowsiel_wallet/adrbrowsiel_wallet_service_factory.h"
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "adrbrowsiel/browser/ipfs/ipfs_service_factory.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "adrbrowsiel/browser/tor/tor_profile_service_factory.h"
#endif

namespace adrbrowsiel {

void EnsureBrowserContextKeyedServiceFactoriesBuilt() {
  adrbrowsiel_ads::AdsServiceFactory::GetInstance();
#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
  adrbrowsiel_rewards::RewardsServiceFactory::GetInstance();
#endif
  adrbrowsiel_shields::AdBlockPrefServiceFactory::GetInstance();
  adrbrowsiel_shields::CookiePrefServiceFactory::GetInstance();
#if BUILDFLAG(ENABLE_GREASELION)
  greaselion::GreaselionServiceFactory::GetInstance();
#endif
#if BUILDFLAG(ENABLE_TOR)
  TorProfileServiceFactory::GetInstance();
#endif
  SearchEngineProviderServiceFactory::GetInstance();
  SearchEngineTrackerFactory::GetInstance();
  ntp_background_images::ViewCounterServiceFactory::GetInstance();

#if !defined(OS_ANDROID)
  BookmarkPrefsServiceFactory::GetInstance();
#else
  ntp_background_images::NTPBackgroundImagesBridgeFactory::GetInstance();
#endif

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
  adrbrowsielWalletServiceFactory::GetInstance();
#endif

#if BUILDFLAG(IPFS_ENABLED)
  ipfs::IpfsServiceFactory::GetInstance();
#endif
  PermissionLifetimeManagerFactory::GetInstance();
}

}  // namespace adrbrowsiel
