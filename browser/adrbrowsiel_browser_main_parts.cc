/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/adrbrowsiel_browser_main_parts.h"

#include "base/command_line.h"
#include "adrbrowsiel/browser/browsing_data/adrbrowsiel_clear_browsing_data.h"
#include "adrbrowsiel/common/adrbrowsiel_constants.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_sync/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_sync/features.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/buildflags/buildflags.h"
#include "adrbrowsiel/components/tor/buildflags/buildflags.h"
#include "chrome/common/chrome_features.h"
#include "components/prefs/pref_service.h"
#include "components/sync/driver/sync_driver_switches.h"
#include "content/public/browser/render_frame_host.h"
#include "extensions/buildflags/buildflags.h"
#include "media/base/media_switches.h"

#if BUILDFLAG(ENABLE_TOR)
#include <string>
#include "base/files/file_util.h"
#include "adrbrowsiel/components/tor/tor_constants.h"
#include "chrome/browser/browser_process_impl.h"
#include "chrome/browser/profiles/profile_attributes_storage.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/browser/profiles/profile_metrics.h"
#include "components/account_id/account_id.h"
#endif

#if !defined(OS_ANDROID)
#include "adrbrowsiel/browser/infobars/adrbrowsiel_confirm_p3a_infobar_delegate.h"
#include "adrbrowsiel/browser/infobars/crypto_wallets_infobar_delegate.h"
#include "chrome/browser/infobars/infobar_service.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "content/public/browser/web_contents.h"
#endif

#if BUILDFLAG(ENABLE_TOR) || !defined(OS_ANDROID)
#include "chrome/browser/browser_process.h"
#endif

#if BUILDFLAG(ENABLE_adrbrowsiel_SYNC) && !defined(OS_ANDROID)
#include "adrbrowsiel/browser/infobars/sync_v2_migrate_infobar_delegate.h"
#include "chrome/browser/sync/profile_sync_service_factory.h"
#include "components/sync/driver/sync_service.h"
#include "components/sync/driver/sync_user_settings.h"
#endif

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED) && BUILDFLAG(ENABLE_EXTENSIONS)
#include "adrbrowsiel/browser/extensions/adrbrowsiel_component_loader.h"
#include "chrome/browser/extensions/extension_service.h"
#include "extensions/browser/extension_system.h"
#endif

void adrbrowsielBrowserMainParts::PostBrowserStart() {
  ChromeBrowserMainParts::PostBrowserStart();

#if BUILDFLAG(ENABLE_TOR)
  ProfileManager* profile_manager = g_browser_process->profile_manager();
  base::FilePath tor_legacy_path =
      profile_manager->user_data_dir().Append(tor::kTorProfileDir);

  // Delete Tor legacy profile if exists.
  if (base::PathExists(tor_legacy_path)) {
    // Add tor legacy path into profile attributes storage first if nonexist
    // because we will hit DCHECK(!GetProfileAttributesWithPath(...))  in
    // ProfileInfoCache::DeleteProfileFromCache when we trying to delete it
    // without this being added into the storage first.
    ProfileAttributesStorage& storage =
        profile_manager->GetProfileAttributesStorage();
    ProfileAttributesEntry* entry =
        storage.GetProfileAttributesWithPath(tor_legacy_path);
    if (!entry) {
      storage.AddProfile(tor_legacy_path, std::u16string(), std::string(),
                         std::u16string(),
                         /* is_consented_primary_account*/ false, 0,
                         std::string(), EmptyAccountId());
    }

    profile_manager->MaybeScheduleProfileForDeletion(
        tor_legacy_path, base::DoNothing(),
        ProfileMetrics::DELETE_PROFILE_SETTINGS);
  }
  for (Profile* profile : profile_manager->GetLoadedProfiles()) {
    const base::FilePath tor_legacy_session_path =
        profile->GetPath()
            .Append(adrbrowsiel::kSessionProfileDir)
            .Append(tor::kTorProfileDir);
    if (base::PathExists(tor_legacy_session_path)) {
      profile_manager->MaybeScheduleProfileForDeletion(
          tor_legacy_session_path, base::DoNothing(),
          ProfileMetrics::DELETE_PROFILE_SETTINGS);
    }
  }
#endif

#if !defined(OS_ANDROID)
  Browser* browser = chrome::FindLastActive();
  content::WebContents* active_web_contents = nullptr;

  if (browser) {
    active_web_contents = browser->tab_strip_model()->GetActiveWebContents();

    if (active_web_contents) {
      InfoBarService* infobar_service =
          InfoBarService::FromWebContents(active_web_contents);

      if (infobar_service) {
        adrbrowsielConfirmP3AInfoBarDelegate::Create(
            infobar_service, g_browser_process->local_state());
#if BUILDFLAG(ENABLE_adrbrowsiel_SYNC)
        auto* sync_service =
            ProfileSyncServiceFactory::IsSyncAllowed(profile())
                ? ProfileSyncServiceFactory::GetForProfile(profile())
                : nullptr;
        const bool is_v2_user =
            sync_service &&
            sync_service->GetUserSettings()->IsFirstSetupComplete();
        SyncV2MigrateInfoBarDelegate::Create(infobar_service, is_v2_user,
                                             profile(), browser);
#endif  // BUILDFLAG(ENABLE_adrbrowsiel_SYNC)
      }
    }
  }
#endif  // !defined(OS_ANDROID)
}

void adrbrowsielBrowserMainParts::PreShutdown() {
  content::adrbrowsielClearBrowsingData::ClearOnExit();
}

void adrbrowsielBrowserMainParts::PreProfileInit() {
  ChromeBrowserMainParts::PreProfileInit();
#if !defined(OS_ANDROID)
  auto* command_line = base::CommandLine::ForCurrentProcess();
  if (!base::FeatureList::IsEnabled(adrbrowsiel_sync::features::kadrbrowsielSync)) {
    // Disable sync temporarily
    if (!command_line->HasSwitch(switches::kDisableSync))
      command_line->AppendSwitch(switches::kDisableSync);
  } else {
    // Relaunch after flag changes will still have the switch
    // when switching from disabled to enabled
    command_line->RemoveSwitch(switches::kDisableSync);
  }
#endif
}

void adrbrowsielBrowserMainParts::PostProfileInit() {
  ChromeBrowserMainParts::PostProfileInit();

#if defined(OS_ANDROID)
  if (profile()->GetPrefs()->GetBoolean(kBackgroundVideoPlaybackEnabled)) {
    content::RenderFrameHost::AllowInjectingJavaScript();
    auto* command_line = base::CommandLine::ForCurrentProcess();
    command_line->AppendSwitch(switches::kDisableBackgroundMediaSuspend);
  }
#endif

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED) && BUILDFLAG(ENABLE_EXTENSIONS)
  extensions::ExtensionService* service =
      extensions::ExtensionSystem::Get(profile())->extension_service();
  if (service) {
    extensions::ComponentLoader* loader = service->component_loader();
    static_cast<extensions::adrbrowsielComponentLoader*>(loader)
        ->AddEthereumRemoteClientExtensionOnStartup();
  }
#endif
}
