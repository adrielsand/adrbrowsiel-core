/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>

#include "adrbrowsiel/browser/ui/adrbrowsiel_browser_command_controller.h"
#include "adrbrowsiel/browser/ui/browser_commands.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_sync/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/buildflags/buildflags.h"
#include "adrbrowsiel/components/tor/buildflags/buildflags.h"
#include "chrome/app/chrome_command_ids.h"
#include "chrome/browser/chrome_notification_types.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_window.h"
#include "chrome/browser/ui/browser_list.h"
#include "chrome/browser/ui/browser_list_observer.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "content/public/browser/notification_service.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/test_utils.h"

#if BUILDFLAG(ENABLE_adrbrowsiel_SYNC)
#include "components/sync/driver/sync_driver_switches.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "adrbrowsiel/browser/tor/tor_profile_service_factory.h"
#endif

using adrbrowsielBrowserCommandControllerTest = InProcessBrowserTest;

IN_PROC_BROWSER_TEST_F(adrbrowsielBrowserCommandControllerTest,
                       adrbrowsielCommandsEnableTest) {
  // Test normal browser's adrbrowsiel commands status.
  auto* command_controller = browser()->command_controller();
#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_REWARDS));
#else
  EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_REWARDS));
#endif

  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_ADBLOCK));

#if BUILDFLAG(ENABLE_TOR)
  EXPECT_FALSE(
      command_controller->IsCommandEnabled(IDC_NEW_TOR_CONNECTION_FOR_SITE));
  EXPECT_TRUE(
      command_controller->IsCommandEnabled(IDC_NEW_OFFTHERECORD_WINDOW_TOR));
#else
  EXPECT_FALSE(
      command_controller->IsCommandEnabled(IDC_NEW_TOR_CONNECTION_FOR_SITE));
  EXPECT_FALSE(
      command_controller->IsCommandEnabled(IDC_NEW_OFFTHERECORD_WINDOW_TOR));
#endif

#if BUILDFLAG(ENABLE_adrbrowsiel_SYNC)
  if (switches::IsSyncAllowedByFlag())
    EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_SYNC));
  else
    EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_SYNC));
#else
  EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_SYNC));
#endif

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_WALLET));
#else
  EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_WALLET));
#endif

  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_ADD_NEW_PROFILE));
  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_OPEN_GUEST_PROFILE));
  EXPECT_TRUE(
      command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_WEBCOMPAT_REPORTER));

  // Create private browser and test its adrbrowsiel commands status.
  auto* private_browser = CreateIncognitoBrowser();
  command_controller = private_browser->command_controller();
#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_REWARDS));
#endif

  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_ADBLOCK));

#if BUILDFLAG(ENABLE_TOR)
  EXPECT_FALSE(
      command_controller->IsCommandEnabled(IDC_NEW_TOR_CONNECTION_FOR_SITE));
  EXPECT_TRUE(
      command_controller->IsCommandEnabled(IDC_NEW_OFFTHERECORD_WINDOW_TOR));
#endif

#if BUILDFLAG(ENABLE_adrbrowsiel_SYNC)
  if (switches::IsSyncAllowedByFlag())
    EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_SYNC));
  else
    EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_SYNC));
#else
  EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_SYNC));
#endif

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_WALLET));
#endif

  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_ADD_NEW_PROFILE));
  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_OPEN_GUEST_PROFILE));
  EXPECT_TRUE(
      command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_WEBCOMPAT_REPORTER));

  // Create guest browser and test its adrbrowsiel commands status.
  ui_test_utils::BrowserChangeObserver browser_creation_observer(
      nullptr, ui_test_utils::BrowserChangeObserver::ChangeType::kAdded);
  profiles::SwitchToGuestProfile(ProfileManager::CreateCallback());

  Browser* guest_browser = browser_creation_observer.Wait();
  DCHECK(guest_browser);
  EXPECT_TRUE(guest_browser->profile()->IsGuestSession());
  command_controller = guest_browser->command_controller();
#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
  EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_REWARDS));
#endif

  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_ADBLOCK));

#if BUILDFLAG(ENABLE_TOR)
  EXPECT_FALSE(
      command_controller->IsCommandEnabled(IDC_NEW_TOR_CONNECTION_FOR_SITE));
  EXPECT_FALSE(
      command_controller->IsCommandEnabled(IDC_NEW_OFFTHERECORD_WINDOW_TOR));
#endif

#if BUILDFLAG(ENABLE_adrbrowsiel_SYNC)
  EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_SYNC));
#endif

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
  EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_WALLET));
#endif

  EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_ADD_NEW_PROFILE));
  EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_OPEN_GUEST_PROFILE));
  EXPECT_TRUE(
      command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_WEBCOMPAT_REPORTER));

#if BUILDFLAG(ENABLE_TOR)
  // Launch tor window and check its command status.
  ui_test_utils::BrowserChangeObserver tor_browser_creation_observer(
      nullptr, ui_test_utils::BrowserChangeObserver::ChangeType::kAdded);
  adrbrowsiel::NewOffTheRecordWindowTor(browser());
  Browser* tor_browser = tor_browser_creation_observer.Wait();
  DCHECK(tor_browser);
  EXPECT_TRUE(tor_browser->profile()->IsTor());
  command_controller = tor_browser->command_controller();
#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_REWARDS));
#endif

  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_ADBLOCK));

  EXPECT_TRUE(
      command_controller->IsCommandEnabled(IDC_NEW_TOR_CONNECTION_FOR_SITE));
  EXPECT_TRUE(
      command_controller->IsCommandEnabled(IDC_NEW_OFFTHERECORD_WINDOW_TOR));

#if BUILDFLAG(ENABLE_adrbrowsiel_SYNC)
  if (switches::IsSyncAllowedByFlag())
    EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_SYNC));
  else
    EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_SYNC));
#endif

#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_WALLET));
#endif

  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_ADD_NEW_PROFILE));
  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_OPEN_GUEST_PROFILE));
  EXPECT_TRUE(
      command_controller->IsCommandEnabled(IDC_SHOW_adrbrowsiel_WEBCOMPAT_REPORTER));

  // Check tor commands when tor is disabled.
  TorProfileServiceFactory::SetTorDisabled(true);
  command_controller = browser()->command_controller();
  EXPECT_FALSE(
      command_controller->IsCommandEnabled(IDC_NEW_TOR_CONNECTION_FOR_SITE));
  EXPECT_FALSE(
      command_controller->IsCommandEnabled(IDC_NEW_OFFTHERECORD_WINDOW_TOR));
#endif
}
