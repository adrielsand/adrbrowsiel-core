/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// This interface is for managing the global services of the application. Each
// service is lazily created when requested the first time. The service getters
// will return NULL if the service is not available, so callers must check for
// this condition.

#ifndef adrbrowsiel_BROWSER_adrbrowsiel_BROWSER_PROCESS_H_
#define adrbrowsiel_BROWSER_adrbrowsiel_BROWSER_PROCESS_H_

#include "adrbrowsiel/components/adrbrowsiel_ads/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_referrals/buildflags/buildflags.h"
#include "adrbrowsiel/components/greaselion/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"
#include "adrbrowsiel/components/speedreader/buildflags.h"
#include "adrbrowsiel/components/tor/buildflags/buildflags.h"
#include "extensions/buildflags/buildflags.h"

namespace adrbrowsiel {
class adrbrowsielReferralsService;
class adrbrowsielP3AService;
}  // namespace adrbrowsiel

namespace adrbrowsiel_component_updater {
#if BUILDFLAG(ENABLE_EXTENSIONS)
class ExtensionWhitelistService;
#endif
class LocalDataFilesService;
}  // namespace adrbrowsiel_component_updater

namespace adrbrowsiel_shields {
class AdBlockService;
class AdBlockCustomFiltersService;
class AdBlockRegionalServiceManager;
class HTTPSEverywhereService;
}  // namespace adrbrowsiel_shields

namespace adrbrowsiel_stats {
class adrbrowsielStatsUpdater;
}  // namespace adrbrowsiel_stats

namespace greaselion {
#if BUILDFLAG(ENABLE_GREASELION)
class GreaselionDownloadService;
#endif
}  // namespace greaselion

namespace ntp_background_images {
class NTPBackgroundImagesService;
}  // namespace ntp_background_images

namespace tor {
class adrbrowsielTorClientUpdater;
}

namespace ipfs {
class adrbrowsielIpfsClientUpdater;
}

namespace speedreader {
class SpeedreaderRewriterService;
}

namespace adrbrowsiel_ads {
class ResourceComponent;
}

class adrbrowsielBrowserProcess {
 public:
  virtual void StartadrbrowsielServices() = 0;
  virtual adrbrowsiel_shields::AdBlockService* ad_block_service() = 0;
  virtual adrbrowsiel_shields::AdBlockCustomFiltersService*
  ad_block_custom_filters_service() = 0;
  virtual adrbrowsiel_shields::AdBlockRegionalServiceManager*
  ad_block_regional_service_manager() = 0;
#if BUILDFLAG(ENABLE_EXTENSIONS)
  virtual adrbrowsiel_component_updater::ExtensionWhitelistService*
  extension_whitelist_service() = 0;
#endif
#if BUILDFLAG(ENABLE_GREASELION)
  virtual greaselion::GreaselionDownloadService*
  greaselion_download_service() = 0;
#endif
  virtual adrbrowsiel_shields::HTTPSEverywhereService* https_everywhere_service() = 0;
  virtual adrbrowsiel_component_updater::LocalDataFilesService*
  local_data_files_service() = 0;
#if BUILDFLAG(ENABLE_TOR)
  virtual tor::adrbrowsielTorClientUpdater* tor_client_updater() = 0;
#endif
#if BUILDFLAG(IPFS_ENABLED)
  virtual ipfs::adrbrowsielIpfsClientUpdater* ipfs_client_updater() = 0;
#endif
  virtual adrbrowsiel::adrbrowsielP3AService* adrbrowsiel_p3a_service() = 0;
  virtual adrbrowsiel::adrbrowsielReferralsService* adrbrowsiel_referrals_service() = 0;
  virtual adrbrowsiel_stats::adrbrowsielStatsUpdater* adrbrowsiel_stats_updater() = 0;
  virtual ntp_background_images::NTPBackgroundImagesService*
  ntp_background_images_service() = 0;
#if BUILDFLAG(ENABLE_SPEEDREADER)
  virtual speedreader::SpeedreaderRewriterService*
  speedreader_rewriter_service() = 0;
#endif
#if BUILDFLAG(adrbrowsiel_ADS_ENABLED)
  virtual adrbrowsiel_ads::ResourceComponent* resource_component() = 0;
#endif
};

extern adrbrowsielBrowserProcess* g_adrbrowsiel_browser_process;

#endif  // adrbrowsiel_BROWSER_adrbrowsiel_BROWSER_PROCESS_H_
