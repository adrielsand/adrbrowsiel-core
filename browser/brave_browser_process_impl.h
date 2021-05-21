/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_adrbrowsiel_BROWSER_PROCESS_IMPL_H_
#define adrbrowsiel_BROWSER_adrbrowsiel_BROWSER_PROCESS_IMPL_H_

#include <memory>

#include "base/memory/ref_counted.h"
#include "adrbrowsiel/browser/adrbrowsiel_browser_process.h"
#include "adrbrowsiel/components/adrbrowsiel_ads/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_component_updater/browser/adrbrowsiel_component.h"
#include "adrbrowsiel/components/adrbrowsiel_referrals/buildflags/buildflags.h"
#include "adrbrowsiel/components/greaselion/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"
#include "adrbrowsiel/components/speedreader/buildflags.h"
#include "adrbrowsiel/components/tor/buildflags/buildflags.h"
#include "chrome/browser/browser_process_impl.h"
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

class adrbrowsielBrowserProcessImpl : public adrbrowsielBrowserProcess,
                                public BrowserProcessImpl {
 public:
  explicit adrbrowsielBrowserProcessImpl(StartupData* startup_data);
  ~adrbrowsielBrowserProcessImpl() override;

  // BrowserProcess implementation.

  ProfileManager* profile_manager() override;
  NotificationPlatformBridge* notification_platform_bridge() override;

  // adrbrowsielBrowserProcess implementation.

  void StartadrbrowsielServices() override;
  adrbrowsiel_shields::AdBlockService* ad_block_service() override;
  adrbrowsiel_shields::AdBlockCustomFiltersService* ad_block_custom_filters_service()
      override;
  adrbrowsiel_shields::AdBlockRegionalServiceManager*
  ad_block_regional_service_manager() override;
#if BUILDFLAG(ENABLE_EXTENSIONS)
  adrbrowsiel_component_updater::ExtensionWhitelistService*
  extension_whitelist_service() override;
#endif
#if BUILDFLAG(ENABLE_GREASELION)
  greaselion::GreaselionDownloadService* greaselion_download_service() override;
#endif
  adrbrowsiel_shields::HTTPSEverywhereService* https_everywhere_service() override;
  adrbrowsiel_component_updater::LocalDataFilesService* local_data_files_service()
      override;
#if BUILDFLAG(ENABLE_TOR)
  tor::adrbrowsielTorClientUpdater* tor_client_updater() override;
#endif
#if BUILDFLAG(IPFS_ENABLED)
  ipfs::adrbrowsielIpfsClientUpdater* ipfs_client_updater() override;
#endif
  adrbrowsiel::adrbrowsielP3AService* adrbrowsiel_p3a_service() override;
  adrbrowsiel::adrbrowsielReferralsService* adrbrowsiel_referrals_service() override;
  adrbrowsiel_stats::adrbrowsielStatsUpdater* adrbrowsiel_stats_updater() override;
  ntp_background_images::NTPBackgroundImagesService*
  ntp_background_images_service() override;
#if BUILDFLAG(ENABLE_SPEEDREADER)
  speedreader::SpeedreaderRewriterService* speedreader_rewriter_service()
      override;
#endif
#if BUILDFLAG(adrbrowsiel_ADS_ENABLED)
  adrbrowsiel_ads::ResourceComponent* resource_component() override;
#endif

 private:
  // BrowserProcessImpl overrides:
  void Init() override;

  void CreateProfileManager();
  void CreateNotificationPlatformBridge();

#if BUILDFLAG(ENABLE_TOR)
  void OnTorEnabledChanged();
#endif

  void UpdateadrbrowsielDarkMode();
  void OnadrbrowsielDarkModeChanged();

  adrbrowsiel_component_updater::adrbrowsielComponent::Delegate*
  adrbrowsiel_component_updater_delegate();

  // local_data_files_service_ should always be first because it needs
  // to be destroyed last
  std::unique_ptr<adrbrowsiel_component_updater::LocalDataFilesService>
      local_data_files_service_;
  std::unique_ptr<adrbrowsiel_component_updater::adrbrowsielComponent::Delegate>
      adrbrowsiel_component_updater_delegate_;
  std::unique_ptr<adrbrowsiel_shields::AdBlockService> ad_block_service_;
#if BUILDFLAG(ENABLE_EXTENSIONS)
  std::unique_ptr<adrbrowsiel_component_updater::ExtensionWhitelistService>
      extension_whitelist_service_;
#endif
#if BUILDFLAG(ENABLE_GREASELION)
  std::unique_ptr<greaselion::GreaselionDownloadService>
      greaselion_download_service_;
#endif
  std::unique_ptr<adrbrowsiel_shields::HTTPSEverywhereService>
      https_everywhere_service_;
  std::unique_ptr<adrbrowsiel_stats::adrbrowsielStatsUpdater> adrbrowsiel_stats_updater_;
#if BUILDFLAG(ENABLE_adrbrowsiel_REFERRALS)
  std::unique_ptr<adrbrowsiel::adrbrowsielReferralsService> adrbrowsiel_referrals_service_;
#endif
#if BUILDFLAG(ENABLE_TOR)
  std::unique_ptr<tor::adrbrowsielTorClientUpdater> tor_client_updater_;
#endif
#if BUILDFLAG(IPFS_ENABLED)
  std::unique_ptr<ipfs::adrbrowsielIpfsClientUpdater> ipfs_client_updater_;
#endif
  scoped_refptr<adrbrowsiel::adrbrowsielP3AService> adrbrowsiel_p3a_service_;
  std::unique_ptr<ntp_background_images::NTPBackgroundImagesService>
      ntp_background_images_service_;

#if BUILDFLAG(ENABLE_SPEEDREADER)
  std::unique_ptr<speedreader::SpeedreaderRewriterService>
      speedreader_rewriter_service_;
#endif

#if BUILDFLAG(adrbrowsiel_ADS_ENABLED)
  std::unique_ptr<adrbrowsiel_ads::ResourceComponent> resource_component_;
#endif

  SEQUENCE_CHECKER(sequence_checker_);

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielBrowserProcessImpl);
};

#endif  // adrbrowsiel_BROWSER_adrbrowsiel_BROWSER_PROCESS_IMPL_H_
