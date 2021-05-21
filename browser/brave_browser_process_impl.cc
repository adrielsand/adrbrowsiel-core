/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/adrbrowsiel_browser_process_impl.h"

#include <utility>

#include "base/bind.h"
#include "base/path_service.h"
#include "base/task/post_task.h"
#include "adrbrowsiel/browser/adrbrowsiel_stats/adrbrowsiel_stats_updater.h"
#include "adrbrowsiel/browser/component_updater/adrbrowsiel_component_updater_configurator.h"
#include "adrbrowsiel/browser/component_updater/adrbrowsiel_component_updater_delegate.h"
#include "adrbrowsiel/browser/net/adrbrowsiel_system_request_handler.h"
#include "adrbrowsiel/browser/profiles/adrbrowsiel_profile_manager.h"
#include "adrbrowsiel/browser/themes/adrbrowsiel_dark_mode_utils.h"
#include "adrbrowsiel/browser/ui/adrbrowsiel_browser_command_controller.h"
#include "adrbrowsiel/common/adrbrowsiel_channel_info.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_ads/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_component_updater/browser/adrbrowsiel_on_demand_updater.h"
#include "adrbrowsiel/components/adrbrowsiel_component_updater/browser/local_data_files_service.h"
#include "adrbrowsiel/components/adrbrowsiel_referrals/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/ad_block_custom_filters_service.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/ad_block_regional_service_manager.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/ad_block_service.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/https_everywhere_service.h"
#include "adrbrowsiel/components/adrbrowsiel_sync/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_sync/network_time_helper.h"
#include "adrbrowsiel/components/ntp_background_images/browser/features.h"
#include "adrbrowsiel/components/ntp_background_images/browser/ntp_background_images_service.h"
#include "adrbrowsiel/components/p3a/adrbrowsiel_histogram_rewrite.h"
#include "adrbrowsiel/components/p3a/adrbrowsiel_p3a_service.h"
#include "adrbrowsiel/components/p3a/buildflags.h"
#include "adrbrowsiel/services/network/public/cpp/system_request_handler.h"
#include "chrome/browser/component_updater/component_updater_utils.h"
#include "chrome/browser/net/system_network_context_manager.h"
#include "chrome/common/buildflags.h"
#include "chrome/common/chrome_paths.h"
#include "components/component_updater/component_updater_service.h"
#include "components/component_updater/timer_update_scheduler.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/child_process_security_policy.h"
#include "services/network/public/cpp/resource_request.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"

#if BUILDFLAG(ENABLE_SYSTEM_NOTIFICATIONS)
#include "chrome/browser/notifications/notification_platform_bridge.h"
#include "adrbrowsiel/browser/notifications/adrbrowsiel_notification_platform_bridge.h"
#endif

#if BUILDFLAG(ENABLE_adrbrowsiel_REFERRALS)
#include "adrbrowsiel/components/adrbrowsiel_referrals/browser/adrbrowsiel_referrals_service.h"
#endif

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "adrbrowsiel/common/extensions/whitelist.h"
#include "adrbrowsiel/components/adrbrowsiel_component_updater/browser/extension_whitelist_service.h"
#endif

#if BUILDFLAG(ENABLE_GREASELION)
#include "adrbrowsiel/components/greaselion/browser/greaselion_download_service.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "adrbrowsiel/components/tor/adrbrowsiel_tor_client_updater.h"
#include "adrbrowsiel/components/tor/pref_names.h"
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "adrbrowsiel/components/ipfs/adrbrowsiel_ipfs_client_updater.h"
#include "adrbrowsiel/components/ipfs/ipfs_constants.h"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "adrbrowsiel/components/speedreader/speedreader_rewriter_service.h"
#endif

#if defined(OS_ANDROID)
#include "chrome/browser/flags/android/chrome_feature_list.h"
#else
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_list.h"
#endif

#if BUILDFLAG(adrbrowsiel_ADS_ENABLED)
#include "adrbrowsiel/components/adrbrowsiel_ads/browser/component_updater/resource_component.h"
#endif

using adrbrowsiel_component_updater::adrbrowsielComponent;
using ntp_background_images::features::kadrbrowsielNTPBrandedWallpaper;
using ntp_background_images::NTPBackgroundImagesService;

namespace {

// Initializes callback for SystemRequestHandler
void InitSystemRequestHandlerCallback() {
  network::SystemRequestHandler::OnBeforeSystemRequestCallback
      before_system_request_callback = base::Bind(adrbrowsiel::OnBeforeSystemRequest);
  network::SystemRequestHandler::GetInstance()
      ->RegisterOnBeforeSystemRequestCallback(before_system_request_callback);
}

}  // namespace

adrbrowsielBrowserProcess* g_adrbrowsiel_browser_process = nullptr;

using content::BrowserThread;

adrbrowsielBrowserProcessImpl::~adrbrowsielBrowserProcessImpl() {}

adrbrowsielBrowserProcessImpl::adrbrowsielBrowserProcessImpl(StartupData* startup_data)
    : BrowserProcessImpl(startup_data) {
  g_browser_process = this;
  g_adrbrowsiel_browser_process = this;

#if BUILDFLAG(ENABLE_adrbrowsiel_REFERRALS)
  // early initialize referrals
  adrbrowsiel_referrals_service();
#endif
  // early initialize adrbrowsiel stats
  adrbrowsiel_stats_updater();

  // Disabled on mobile platforms, see for instance issues/6176
#if BUILDFLAG(adrbrowsiel_P3A_ENABLED)
  // Create P3A Service early to catch more histograms. The full initialization
  // should be started once browser process impl is ready.
  adrbrowsiel_p3a_service();
  adrbrowsiel::SetupHistogramsadrbrowsielization();
#endif  // BUILDFLAG(adrbrowsiel_P3A_ENABLED)
}

void adrbrowsielBrowserProcessImpl::Init() {
  BrowserProcessImpl::Init();
#if BUILDFLAG(IPFS_ENABLED)
  content::ChildProcessSecurityPolicy::GetInstance()->RegisterWebSafeScheme(
      ipfs::kIPFSScheme);
  content::ChildProcessSecurityPolicy::GetInstance()->RegisterWebSafeScheme(
      ipfs::kIPNSScheme);
#endif
  adrbrowsiel_component_updater::adrbrowsielOnDemandUpdater::GetInstance()->
      RegisterOnDemandUpdateCallback(
          base::BindRepeating(&component_updater::adrbrowsielOnDemandUpdate));
  UpdateadrbrowsielDarkMode();
  pref_change_registrar_.Add(
      kadrbrowsielDarkMode,
      base::Bind(&adrbrowsielBrowserProcessImpl::OnadrbrowsielDarkModeChanged,
                 base::Unretained(this)));

#if BUILDFLAG(ENABLE_TOR)
  pref_change_registrar_.Add(
      tor::prefs::kTorDisabled,
      base::Bind(&adrbrowsielBrowserProcessImpl::OnTorEnabledChanged,
                 base::Unretained(this)));
#endif

  InitSystemRequestHandlerCallback();
}

adrbrowsiel_component_updater::adrbrowsielComponent::Delegate*
adrbrowsielBrowserProcessImpl::adrbrowsiel_component_updater_delegate() {
  if (!adrbrowsiel_component_updater_delegate_)
    adrbrowsiel_component_updater_delegate_ =
        std::make_unique<adrbrowsiel::adrbrowsielComponentUpdaterDelegate>();

  return adrbrowsiel_component_updater_delegate_.get();
}

ProfileManager* adrbrowsielBrowserProcessImpl::profile_manager() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (!created_profile_manager_)
    CreateProfileManager();
  return profile_manager_.get();
}

void adrbrowsielBrowserProcessImpl::StartadrbrowsielServices() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  ad_block_service()->Start();
  https_everywhere_service()->Start();

#if BUILDFLAG(ENABLE_EXTENSIONS)
  extension_whitelist_service();
#endif
#if BUILDFLAG(ENABLE_GREASELION)
  greaselion_download_service();
#endif
#if BUILDFLAG(ENABLE_SPEEDREADER)
  speedreader_rewriter_service();
#endif
#if BUILDFLAG(adrbrowsiel_ADS_ENABLED)
  resource_component();
#endif
  // Now start the local data files service, which calls all observers.
  local_data_files_service()->Start();

#if BUILDFLAG(ENABLE_adrbrowsiel_SYNC)
  adrbrowsiel_sync::NetworkTimeHelper::GetInstance()
    ->SetNetworkTimeTracker(g_browser_process->network_time_tracker());
#endif
}

adrbrowsiel_shields::AdBlockService* adrbrowsielBrowserProcessImpl::ad_block_service() {
  if (ad_block_service_)
    return ad_block_service_.get();

  ad_block_service_ =
      adrbrowsiel_shields::AdBlockServiceFactory(adrbrowsiel_component_updater_delegate());
  return ad_block_service_.get();
}

adrbrowsiel_shields::AdBlockCustomFiltersService*
adrbrowsielBrowserProcessImpl::ad_block_custom_filters_service() {
  return ad_block_service()->custom_filters_service();
}

adrbrowsiel_shields::AdBlockRegionalServiceManager*
adrbrowsielBrowserProcessImpl::ad_block_regional_service_manager() {
  return ad_block_service()->regional_service_manager();
}

NTPBackgroundImagesService*
adrbrowsielBrowserProcessImpl::ntp_background_images_service() {
  if (!base::FeatureList::IsEnabled(kadrbrowsielNTPBrandedWallpaper))
    return nullptr;

  if (!ntp_background_images_service_) {
    ntp_background_images_service_ =
        std::make_unique<NTPBackgroundImagesService>(component_updater(),
                                                     local_state());
    ntp_background_images_service_->Init();
  }

  return ntp_background_images_service_.get();
}

#if BUILDFLAG(ENABLE_EXTENSIONS)
adrbrowsiel_component_updater::ExtensionWhitelistService*
adrbrowsielBrowserProcessImpl::extension_whitelist_service() {
  if (!extension_whitelist_service_) {
    extension_whitelist_service_ =
        adrbrowsiel_component_updater::ExtensionWhitelistServiceFactory(
            local_data_files_service(), kVettedExtensions);
  }
  return extension_whitelist_service_.get();
}
#endif

#if BUILDFLAG(ENABLE_GREASELION)
greaselion::GreaselionDownloadService*
adrbrowsielBrowserProcessImpl::greaselion_download_service() {
  if (!greaselion_download_service_) {
    greaselion_download_service_ = greaselion::GreaselionDownloadServiceFactory(
        local_data_files_service());
  }
  return greaselion_download_service_.get();
}
#endif

adrbrowsiel_shields::HTTPSEverywhereService*
adrbrowsielBrowserProcessImpl::https_everywhere_service() {
  if (!https_everywhere_service_)
    https_everywhere_service_ = adrbrowsiel_shields::HTTPSEverywhereServiceFactory(
        adrbrowsiel_component_updater_delegate());
  return https_everywhere_service_.get();
}

adrbrowsiel_component_updater::LocalDataFilesService*
adrbrowsielBrowserProcessImpl::local_data_files_service() {
  if (!local_data_files_service_)
    local_data_files_service_ =
        adrbrowsiel_component_updater::LocalDataFilesServiceFactory(
            adrbrowsiel_component_updater_delegate());
  return local_data_files_service_.get();
}

void adrbrowsielBrowserProcessImpl::UpdateadrbrowsielDarkMode() {
  // Update with proper system theme to make adrbrowsiel theme and base ui components
  // theme use same theme.
  dark_mode::SetSystemDarkMode(dark_mode::GetadrbrowsielDarkModeType());
}

void adrbrowsielBrowserProcessImpl::OnadrbrowsielDarkModeChanged() {
  UpdateadrbrowsielDarkMode();
}

#if BUILDFLAG(ENABLE_TOR)
tor::adrbrowsielTorClientUpdater*
adrbrowsielBrowserProcessImpl::tor_client_updater() {
  if (tor_client_updater_)
    return tor_client_updater_.get();

  base::FilePath user_data_dir;
  base::PathService::Get(chrome::DIR_USER_DATA, &user_data_dir);

  tor_client_updater_.reset(new tor::adrbrowsielTorClientUpdater(
      adrbrowsiel_component_updater_delegate(), local_state(), user_data_dir));
  return tor_client_updater_.get();
}

void adrbrowsielBrowserProcessImpl::OnTorEnabledChanged() {
  // Update all browsers' tor command status.
  for (Browser* browser : *BrowserList::GetInstance()) {
    static_cast<chrome::adrbrowsielBrowserCommandController*>(
        browser->command_controller())->UpdateCommandForTor();
  }
}
#endif

adrbrowsiel::adrbrowsielP3AService* adrbrowsielBrowserProcessImpl::adrbrowsiel_p3a_service() {
  if (adrbrowsiel_p3a_service_) {
    return adrbrowsiel_p3a_service_.get();
  }
  adrbrowsiel_p3a_service_ = base::MakeRefCounted<adrbrowsiel::adrbrowsielP3AService>(
      local_state(), adrbrowsiel::GetChannelName(),
      local_state()->GetString(kWeekOfInstallation));
  adrbrowsiel_p3a_service()->InitCallbacks();
  return adrbrowsiel_p3a_service_.get();
}

adrbrowsiel::adrbrowsielReferralsService*
adrbrowsielBrowserProcessImpl::adrbrowsiel_referrals_service() {
  if (!adrbrowsiel_referrals_service_)
    adrbrowsiel_referrals_service_ = std::make_unique<adrbrowsiel::adrbrowsielReferralsService>(
        local_state(), adrbrowsiel_stats::GetAPIKey(),
        adrbrowsiel_stats::GetPlatformIdentifier());
  return adrbrowsiel_referrals_service_.get();
}

adrbrowsiel_stats::adrbrowsielStatsUpdater* adrbrowsielBrowserProcessImpl::adrbrowsiel_stats_updater() {
  if (!adrbrowsiel_stats_updater_)
    adrbrowsiel_stats_updater_ =
        std::make_unique<adrbrowsiel_stats::adrbrowsielStatsUpdater>(local_state());
  return adrbrowsiel_stats_updater_.get();
}

void adrbrowsielBrowserProcessImpl::CreateProfileManager() {
  DCHECK(!created_profile_manager_ && !profile_manager_);
  created_profile_manager_ = true;

  base::FilePath user_data_dir;
  base::PathService::Get(chrome::DIR_USER_DATA, &user_data_dir);
  profile_manager_ = std::make_unique<adrbrowsielProfileManager>(user_data_dir);
}

NotificationPlatformBridge*
adrbrowsielBrowserProcessImpl::notification_platform_bridge() {
#if !defined(OS_MAC)
  return BrowserProcessImpl::notification_platform_bridge();
#else
#if BUILDFLAG(ENABLE_SYSTEM_NOTIFICATIONS)
  if (!created_notification_bridge_)
    CreateNotificationPlatformBridge();
  return notification_bridge_.get();
#else
  return nullptr;
#endif
#endif
}

void adrbrowsielBrowserProcessImpl::CreateNotificationPlatformBridge() {
#if defined(OS_MAC)
#if BUILDFLAG(ENABLE_SYSTEM_NOTIFICATIONS)
  DCHECK(!notification_bridge_);
  notification_bridge_ = adrbrowsielNotificationPlatformBridge::Create();
  created_notification_bridge_ = true;
#endif
#endif
}

#if BUILDFLAG(ENABLE_SPEEDREADER)
speedreader::SpeedreaderRewriterService*
adrbrowsielBrowserProcessImpl::speedreader_rewriter_service() {
  if (!speedreader_rewriter_service_) {
    speedreader_rewriter_service_.reset(
        new speedreader::SpeedreaderRewriterService(
            adrbrowsiel_component_updater_delegate()));
  }
  return speedreader_rewriter_service_.get();
}
#endif  // BUILDFLAG(ENABLE_SPEEDREADER)

#if BUILDFLAG(adrbrowsiel_ADS_ENABLED)
adrbrowsiel_ads::ResourceComponent* adrbrowsielBrowserProcessImpl::resource_component() {
  if (!resource_component_) {
    resource_component_.reset(
        new adrbrowsiel_ads::ResourceComponent(adrbrowsiel_component_updater_delegate()));
  }
  return resource_component_.get();
}

#endif  // BUILDFLAG(adrbrowsiel_ADS_ENABLED)

#if BUILDFLAG(IPFS_ENABLED)
ipfs::adrbrowsielIpfsClientUpdater*
adrbrowsielBrowserProcessImpl::ipfs_client_updater() {
  if (ipfs_client_updater_)
    return ipfs_client_updater_.get();

  base::FilePath user_data_dir;
  base::PathService::Get(chrome::DIR_USER_DATA, &user_data_dir);

  ipfs_client_updater_ = ipfs::adrbrowsielIpfsClientUpdaterFactory(
      adrbrowsiel_component_updater_delegate(), user_data_dir);
  return ipfs_client_updater_.get();
}
#endif  // BUILDFLAG(IPFS_ENABLED)
