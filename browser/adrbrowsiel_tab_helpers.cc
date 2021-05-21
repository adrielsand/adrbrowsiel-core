/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/adrbrowsiel_tab_helpers.h"

#include "base/command_line.h"
#include "base/feature_list.h"
#include "adrbrowsiel/browser/adrbrowsiel_ads/ads_tab_helper.h"
#include "adrbrowsiel/browser/adrbrowsiel_shields/adrbrowsiel_shields_web_contents_observer.h"
#include "adrbrowsiel/browser/adrbrowsiel_stats/adrbrowsiel_stats_tab_helper.h"
#include "adrbrowsiel/browser/ephemeral_storage/ephemeral_storage_tab_helper.h"
#include "adrbrowsiel/browser/ui/bookmark/adrbrowsiel_bookmark_tab_helper.h"
#include "adrbrowsiel/components/adrbrowsiel_perf_predictor/browser/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_wayback_machine/buildflags.h"
#include "adrbrowsiel/components/greaselion/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"
#include "adrbrowsiel/components/speedreader/buildflags.h"
#include "adrbrowsiel/components/tor/buildflags/buildflags.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/web_contents.h"
#include "net/base/features.h"
#include "third_party/widevine/cdm/buildflags.h"

#if BUILDFLAG(ENABLE_GREASELION)
#include "adrbrowsiel/browser/greaselion/greaselion_tab_helper.h"
#endif

#if defined(OS_ANDROID)
#include "adrbrowsiel/browser/android/preferences/background_video_playback_tab_helper.h"
#include "adrbrowsiel/browser/android/preferences/website/desktop_mode_tab_helper.h"
#endif

#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
#include "adrbrowsiel/browser/adrbrowsiel_rewards/rewards_tab_helper.h"
#endif

#if BUILDFLAG(ENABLE_WIDEVINE)
#include "adrbrowsiel/browser/adrbrowsiel_drm_tab_helper.h"
#endif

#if BUILDFLAG(ENABLE_adrbrowsiel_WAYBACK_MACHINE)
#include "adrbrowsiel/browser/infobars/adrbrowsiel_wayback_machine_delegate_impl.h"
#endif

#if BUILDFLAG(ENABLE_adrbrowsiel_PERF_PREDICTOR)
#include "adrbrowsiel/components/adrbrowsiel_perf_predictor/browser/perf_predictor_tab_helper.h"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "adrbrowsiel/browser/speedreader/speedreader_tab_helper.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "adrbrowsiel/components/tor/onion_location_tab_helper.h"
#include "adrbrowsiel/components/tor/tor_tab_helper.h"
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "adrbrowsiel/browser/ipfs/ipfs_service_factory.h"
#include "adrbrowsiel/browser/ipfs/ipfs_tab_helper.h"
#endif

namespace adrbrowsiel {

void AttachTabHelpers(content::WebContents* web_contents) {
#if BUILDFLAG(ENABLE_GREASELION)
  greaselion::GreaselionTabHelper::CreateForWebContents(web_contents);
#endif
  adrbrowsiel_shields::adrbrowsielShieldsWebContentsObserver::CreateForWebContents(
      web_contents);

#if defined(OS_ANDROID)
  DesktopModeTabHelper::CreateForWebContents(web_contents);
  BackgroundVideoPlaybackTabHelper::CreateForWebContents(web_contents);
#else
  // Add tab helpers here unless they are intended for android too
  adrbrowsielBookmarkTabHelper::CreateForWebContents(web_contents);
#endif

#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
  adrbrowsiel_rewards::RewardsTabHelper::CreateForWebContents(web_contents);
#endif

#if BUILDFLAG(ENABLE_WIDEVINE)
  adrbrowsielDrmTabHelper::CreateForWebContents(web_contents);
#endif

#if BUILDFLAG(ENABLE_adrbrowsiel_WAYBACK_MACHINE)
  adrbrowsielWaybackMachineDelegateImpl::AttachTabHelperIfNeeded(web_contents);
#endif

#if BUILDFLAG(ENABLE_adrbrowsiel_PERF_PREDICTOR)
  adrbrowsiel_perf_predictor::PerfPredictorTabHelper::CreateForWebContents(
      web_contents);
#endif

  adrbrowsiel_ads::AdsTabHelper::CreateForWebContents(web_contents);

#if BUILDFLAG(ENABLE_SPEEDREADER)
  speedreader::SpeedreaderTabHelper::CreateForWebContents(web_contents);
#endif

#if BUILDFLAG(ENABLE_TOR)
  tor::TorTabHelper::MaybeCreateForWebContents(
      web_contents, web_contents->GetBrowserContext()->IsTor());
  tor::OnionLocationTabHelper::CreateForWebContents(web_contents);
#endif

#if BUILDFLAG(IPFS_ENABLED)
  ipfs::IPFSTabHelper::MaybeCreateForWebContents(web_contents);
#endif

  adrbrowsiel_stats::adrbrowsielStatsTabHelper::CreateForWebContents(web_contents);

  if (base::FeatureList::IsEnabled(net::features::kadrbrowsielEphemeralStorage)) {
    ephemeral_storage::EphemeralStorageTabHelper::CreateForWebContents(
        web_contents);
  }
}

}  // namespace adrbrowsiel
