/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/adrbrowsiel_browser_command_controller.h"

#include <vector>

#include "adrbrowsiel/app/adrbrowsiel_command_ids.h"
#include "adrbrowsiel/browser/profiles/profile_util.h"
#include "adrbrowsiel/browser/ui/adrbrowsiel_pages.h"
#include "adrbrowsiel/browser/ui/browser_commands.h"
#include "adrbrowsiel/common/pref_names.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_sync/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_wallet/common/buildflags/buildflags.h"
#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"
#include "adrbrowsiel/components/sidebar/buildflags/buildflags.h"
#include "chrome/app/chrome_command_ids.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/common/pref_names.h"
#include "components/prefs/pref_service.h"

#if BUILDFLAG(ENABLE_adrbrowsiel_SYNC)
#include "components/sync/driver/sync_driver_switches.h"
#endif

#if BUILDFLAG(ENABLE_SIDEBAR)
#include "adrbrowsiel/browser/ui/sidebar/sidebar_utils.h"
#endif

#if BUILDFLAG(IPFS_ENABLED)
#include "adrbrowsiel/components/ipfs/ipfs_constants.h"
#include "adrbrowsiel/components/ipfs/ipfs_utils.h"
#include "adrbrowsiel/components/ipfs/pref_names.h"
#endif

namespace {

bool IsadrbrowsielCommands(int id) {
  return id >= IDC_adrbrowsiel_COMMANDS_START && id <= IDC_adrbrowsiel_COMMANDS_LAST;
}

bool IsadrbrowsielOverrideCommands(int id) {
  static std::vector<int> override_commands({
      IDC_NEW_WINDOW,
      IDC_NEW_INCOGNITO_WINDOW,
  });
  return std::find(override_commands.begin(), override_commands.end(), id) !=
         override_commands.end();
}

}  // namespace

namespace chrome {

adrbrowsielBrowserCommandController::adrbrowsielBrowserCommandController(Browser* browser)
    : BrowserCommandController(browser),
      browser_(browser),
      adrbrowsiel_command_updater_(nullptr) {
  InitadrbrowsielCommandState();
}

bool adrbrowsielBrowserCommandController::SupportsCommand(int id) const {
  return IsadrbrowsielCommands(id)
      ? adrbrowsiel_command_updater_.SupportsCommand(id)
      : BrowserCommandController::SupportsCommand(id);
}

bool adrbrowsielBrowserCommandController::IsCommandEnabled(int id) const {
  return IsadrbrowsielCommands(id)
      ? adrbrowsiel_command_updater_.IsCommandEnabled(id)
      : BrowserCommandController::IsCommandEnabled(id);
}

bool adrbrowsielBrowserCommandController::ExecuteCommandWithDisposition(
    int id,
    WindowOpenDisposition disposition,
    base::TimeTicks time_stamp) {
  return IsadrbrowsielCommands(id) || IsadrbrowsielOverrideCommands(id)
             ? ExecuteadrbrowsielCommandWithDisposition(id, disposition, time_stamp)
             : BrowserCommandController::ExecuteCommandWithDisposition(
                   id, disposition, time_stamp);
}

void adrbrowsielBrowserCommandController::AddCommandObserver(
    int id, CommandObserver* observer) {
  IsadrbrowsielCommands(id)
      ? adrbrowsiel_command_updater_.AddCommandObserver(id, observer)
      : BrowserCommandController::AddCommandObserver(id, observer);
}

void adrbrowsielBrowserCommandController::RemoveCommandObserver(
    int id, CommandObserver* observer) {
  IsadrbrowsielCommands(id)
      ? adrbrowsiel_command_updater_.RemoveCommandObserver(id, observer)
      : BrowserCommandController::RemoveCommandObserver(id, observer);
}

void adrbrowsielBrowserCommandController::RemoveCommandObserver(
    CommandObserver* observer) {
  adrbrowsiel_command_updater_.RemoveCommandObserver(observer);
  BrowserCommandController::RemoveCommandObserver(observer);
}

bool adrbrowsielBrowserCommandController::UpdateCommandEnabled(int id, bool state) {
  return IsadrbrowsielCommands(id)
      ? adrbrowsiel_command_updater_.UpdateCommandEnabled(id, state)
      : BrowserCommandController::UpdateCommandEnabled(id, state);
}

void adrbrowsielBrowserCommandController::InitadrbrowsielCommandState() {
  // Sync & Rewards pages doesn't work on tor(guest) session.
  // They also doesn't work on private window but they are redirected
  // to normal window in this case.
  const bool is_guest_session = browser_->profile()->IsGuestSession();
  if (!is_guest_session) {
#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
    UpdateCommandForadrbrowsielRewards();
#endif
#if BUILDFLAG(adrbrowsiel_WALLET_ENABLED)
    UpdateCommandForadrbrowsielWallet();
#endif
#if BUILDFLAG(ENABLE_adrbrowsiel_SYNC)
    if (switches::IsSyncAllowedByFlag())
      UpdateCommandForadrbrowsielSync();
#endif
  }
  UpdateCommandForadrbrowsielAdblock();
  UpdateCommandForWebcompatReporter();
#if BUILDFLAG(ENABLE_TOR)
  UpdateCommandForTor();
#endif
  UpdateCommandForSidebar();
  bool add_new_profile_enabled = !is_guest_session;
  bool open_guest_profile_enabled = !is_guest_session;
  if (!is_guest_session) {
    if (PrefService* local_state = g_browser_process->local_state()) {
      add_new_profile_enabled =
          local_state->GetBoolean(prefs::kBrowserAddPersonEnabled);
      open_guest_profile_enabled =
          local_state->GetBoolean(prefs::kBrowserGuestModeEnabled);
    }
  }
  UpdateCommandEnabled(IDC_ADD_NEW_PROFILE, add_new_profile_enabled);
  UpdateCommandEnabled(IDC_OPEN_GUEST_PROFILE, open_guest_profile_enabled);
  UpdateCommandEnabled(IDC_TOGGLE_SPEEDREADER, true);
}

void adrbrowsielBrowserCommandController::UpdateCommandForadrbrowsielRewards() {
  UpdateCommandEnabled(IDC_SHOW_adrbrowsiel_REWARDS, true);
}

void adrbrowsielBrowserCommandController::UpdateCommandForadrbrowsielAdblock() {
  UpdateCommandEnabled(IDC_SHOW_adrbrowsiel_ADBLOCK, true);
}

void adrbrowsielBrowserCommandController::UpdateCommandForWebcompatReporter() {
  UpdateCommandEnabled(IDC_SHOW_adrbrowsiel_WEBCOMPAT_REPORTER, true);
}

#if BUILDFLAG(ENABLE_TOR)
void adrbrowsielBrowserCommandController::UpdateCommandForTor() {
  // Enable new tor connection only for tor profile.
  UpdateCommandEnabled(IDC_NEW_TOR_CONNECTION_FOR_SITE,
                       browser_->profile()->IsTor());
  UpdateCommandEnabled(IDC_NEW_OFFTHERECORD_WINDOW_TOR,
                       !adrbrowsiel::IsTorDisabledForProfile(browser_->profile()));
}
#endif

void adrbrowsielBrowserCommandController::UpdateCommandForSidebar() {
#if BUILDFLAG(ENABLE_SIDEBAR)
  if (sidebar::CanUseSidebar(browser_->profile()))
    UpdateCommandEnabled(IDC_SIDEBAR_SHOW_OPTION_MENU, true);
#endif
}

void adrbrowsielBrowserCommandController::UpdateCommandForadrbrowsielSync() {
  UpdateCommandEnabled(IDC_SHOW_adrbrowsiel_SYNC, true);
}

void adrbrowsielBrowserCommandController::UpdateCommandForadrbrowsielWallet() {
  UpdateCommandEnabled(IDC_SHOW_adrbrowsiel_WALLET, true);
  UpdateCommandEnabled(IDC_SHOW_adrbrowsiel_WALLET_PANEL, true);
  UpdateCommandEnabled(IDC_CLOSE_adrbrowsiel_WALLET_PANEL, true);
}

bool adrbrowsielBrowserCommandController::ExecuteadrbrowsielCommandWithDisposition(
    int id,
    WindowOpenDisposition disposition,
    base::TimeTicks time_stamp) {
  if (!SupportsCommand(id) || !IsCommandEnabled(id))
    return false;

  if (browser_->tab_strip_model()->active_index() == TabStripModel::kNoTab)
    return true;

  DCHECK(IsCommandEnabled(id)) << "Invalid/disabled command " << id;

  switch (id) {
    case IDC_NEW_WINDOW:
      // Use chromium's action for non-Tor profiles.
      if (!browser_->profile()->IsTor())
        return BrowserCommandController::ExecuteCommandWithDisposition(
            id, disposition, time_stamp);
      NewEmptyWindow(browser_->profile()->GetOriginalProfile());
      break;
    case IDC_NEW_INCOGNITO_WINDOW:
      // Use chromium's action for non-Tor profiles.
      if (!browser_->profile()->IsTor())
        return BrowserCommandController::ExecuteCommandWithDisposition(
            id, disposition, time_stamp);
      NewIncognitoWindow(browser_->profile()->GetOriginalProfile());
      break;
    case IDC_SHOW_adrbrowsiel_REWARDS:
      adrbrowsiel::ShowadrbrowsielRewards(browser_);
      break;
    case IDC_SHOW_adrbrowsiel_ADBLOCK:
      adrbrowsiel::ShowadrbrowsielAdblock(browser_);
      break;
    case IDC_SHOW_adrbrowsiel_WEBCOMPAT_REPORTER:
      adrbrowsiel::ShowWebcompatReporter(browser_);
      break;
    case IDC_NEW_OFFTHERECORD_WINDOW_TOR:
      adrbrowsiel::NewOffTheRecordWindowTor(browser_);
      break;
    case IDC_NEW_TOR_CONNECTION_FOR_SITE:
      adrbrowsiel::NewTorConnectionForSite(browser_);
      break;
    case IDC_SHOW_adrbrowsiel_SYNC:
      adrbrowsiel::ShowSync(browser_);
      break;
    case IDC_SHOW_adrbrowsiel_WALLET:
      adrbrowsiel::ShowadrbrowsielWallet(browser_);
      break;
    case IDC_ADD_NEW_PROFILE:
      adrbrowsiel::AddNewProfile();
      break;
    case IDC_OPEN_GUEST_PROFILE:
      adrbrowsiel::OpenGuestProfile();
      break;
    case IDC_TOGGLE_SPEEDREADER:
      adrbrowsiel::ToggleSpeedreader(browser_);
      break;
    case IDC_SHOW_adrbrowsiel_WALLET_PANEL:
      adrbrowsiel::ShowWalletBubble(browser_);
      break;
    case IDC_CLOSE_adrbrowsiel_WALLET_PANEL:
      adrbrowsiel::CloseWalletBubble(browser_);
      break;
    default:
      LOG(WARNING) << "Received Unimplemented Command: " << id;
      break;
  }

  return true;
}

}  // namespace chrome
