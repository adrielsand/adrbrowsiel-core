/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "build/build_config.h"
#include "adrbrowsiel/components/adrbrowsiel_ads/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"
#include "adrbrowsiel/components/tor/buildflags/buildflags.h"

#if !defined(OS_ANDROID)
#include "adrbrowsiel/utility/importer/adrbrowsiel_profile_import_impl.h"
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "adrbrowsiel/components/services/ipfs/ipfs_service_impl.h"
#include "adrbrowsiel/components/services/ipfs/public/mojom/ipfs_service.mojom.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "adrbrowsiel/components/services/tor/public/interfaces/tor.mojom.h"
#include "adrbrowsiel/components/services/tor/tor_launcher_impl.h"
#endif

#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
#include "adrbrowsiel/components/services/bat_ledger/bat_ledger_service_impl.h"
#include "adrbrowsiel/components/services/bat_ledger/public/interfaces/bat_ledger.mojom.h"
#endif

#if BUILDFLAG(adrbrowsiel_ADS_ENABLED)
#include "adrbrowsiel/components/services/bat_ads/bat_ads_service_impl.h"
#include "adrbrowsiel/components/services/bat_ads/public/interfaces/bat_ads.mojom.h"
#endif

namespace {

#if !defined(OS_ANDROID)
auto RunadrbrowsielProfileImporter(
    mojo::PendingReceiver<adrbrowsiel::mojom::ProfileImport> receiver) {
  return std::make_unique<adrbrowsielProfileImportImpl>(std::move(receiver));
}
#endif

#if BUILDFLAG(IPFS_ENABLED)
auto RunIpfsService(mojo::PendingReceiver<ipfs::mojom::IpfsService> receiver) {
  return std::make_unique<ipfs::IpfsServiceImpl>(std::move(receiver));
}
#endif

#if BUILDFLAG(ENABLE_TOR)
auto RunTorLauncher(mojo::PendingReceiver<tor::mojom::TorLauncher> receiver) {
  return std::make_unique<tor::TorLauncherImpl>(std::move(receiver));
}
#endif

#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
auto RunBatLedgerService(
    mojo::PendingReceiver<bat_ledger::mojom::BatLedgerService> receiver) {
  return std::make_unique<bat_ledger::BatLedgerServiceImpl>(
      std::move(receiver));
}
#endif

#if BUILDFLAG(adrbrowsiel_ADS_ENABLED)
auto RunBatAdsService(
    mojo::PendingReceiver<bat_ads::mojom::BatAdsService> receiver) {
  return std::make_unique<bat_ads::BatAdsServiceImpl>(
      std::move(receiver));
}
#endif

}  // namespace

#if defined(OS_ANDROID)
#define adrbrowsiel_PROFILE_IMPORTER
#else
#define adrbrowsiel_PROFILE_IMPORTER services.Add(RunadrbrowsielProfileImporter);
#endif

#if BUILDFLAG(IPFS_ENABLED)
#define adrbrowsiel_IPFS_SERVICE services.Add(RunIpfsService);
#else
#define adrbrowsiel_IPFS_SERVICE
#endif

#if BUILDFLAG(ENABLE_TOR)
#define adrbrowsiel_TOR_LAUNCHER services.Add(RunTorLauncher);
#else
#define adrbrowsiel_TOR_LAUNCHER
#endif

#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
#define adrbrowsiel_BAT_LEDGER_SERVICE services.Add(RunBatLedgerService);
#else
#define adrbrowsiel_BAT_LEDGER_SERVICE
#endif

#if BUILDFLAG(adrbrowsiel_ADS_ENABLED)
#define adrbrowsiel_BAT_ADS_SERVICE services.Add(RunBatAdsService);
#else
#define adrbrowsiel_BAT_ADS_SERVICE
#endif

#define adrbrowsiel_GET_MAIN_THREAD_SERVICE_FACTORY \
    adrbrowsiel_PROFILE_IMPORTER \
    adrbrowsiel_IPFS_SERVICE \
    adrbrowsiel_TOR_LAUNCHER \
    adrbrowsiel_BAT_LEDGER_SERVICE \
    adrbrowsiel_BAT_ADS_SERVICE

#include "../../../../chrome/utility/services.cc"

#undef adrbrowsiel_GET_MAIN_THREAD_SERVICE_FACTORY
#undef adrbrowsiel_PROFILE_IMPORTER
#undef adrbrowsiel_IPFS_SERVICE
#undef adrbrowsiel_TOR_LAUNCHER
#undef adrbrowsiel_BAT_LEDGER_SERVICE
#undef adrbrowsiel_BAT_ADS_SERVICE
