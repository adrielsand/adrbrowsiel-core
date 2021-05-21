/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ui/singleton_tabs.h"

namespace {

const char kadrbrowsielStubSessionTag[] = "adrbrowsiel_stub_more_session_tag";
const char kadrbrowsielSyncedTabsUrl[] = "adrbrowsiel://history/syncedTabs";

}  //  namespace

#define adrbrowsiel_BUILD_TABS_FROM_OTHER_DEVICES                                 \
  if (tabs_in_session.size() > kMaxTabsPerSessionToShow) {                  \
    /* Not all the tabs are shown in menu */                                \
    if (!stub_tab_.get()) {                                                 \
      stub_tab_.reset(new sessions::SessionTab());                          \
      sessions::SerializedNavigationEntry stub_nav_entry;                   \
      stub_nav_entry.set_title(                                             \
          l10n_util::GetStringUTF16(IDS_OPEN_MORE_OTHER_DEVICES_SESSIONS)); \
      stub_nav_entry.set_virtual_url(GURL(kadrbrowsielSyncedTabsUrl));            \
      stub_tab_->navigations.push_back(stub_nav_entry);                     \
      stub_tab_->tab_id = SessionID::NewUnique();                           \
    }                                                                       \
    tabs_in_session[kMaxTabsPerSessionToShow] = stub_tab_.get();            \
    BuildOtherDevicesTabItem(kadrbrowsielStubSessionTag,                          \
                             *tabs_in_session[kMaxTabsPerSessionToShow]);   \
  }

#include "../../../../../../chrome/browser/ui/toolbar/recent_tabs_sub_menu_model.cc"

#undef adrbrowsiel_BUILD_TABS_FROM_OTHER_DEVICES

#include "adrbrowsiel/browser/ui/toolbar/adrbrowsiel_recent_tabs_sub_menu_model.h"

// Methods of adrbrowsielRecentTabsSubMenuModel are implemented below instead of
// adrbrowsiel_recent_tabs_sub_menu_model.cc to have the access to functions in
// anonymous namespace in recent_tabs_sub_menu_model.cc

adrbrowsielRecentTabsSubMenuModel::adrbrowsielRecentTabsSubMenuModel(
    ui::AcceleratorProvider* accelerator_provider,
    Browser* browser)
    : RecentTabsSubMenuModel(accelerator_provider, browser) {}

adrbrowsielRecentTabsSubMenuModel::~adrbrowsielRecentTabsSubMenuModel() {}

void adrbrowsielRecentTabsSubMenuModel::ExecuteCommand(int command_id,
                                                 int event_flags) {
  if (IsTabModelCommandId(command_id)) {
    TabNavigationItems* tab_items = NULL;
    int tab_items_idx = CommandIdToTabVectorIndex(command_id, &tab_items);
    const TabNavigationItem& item = (*tab_items)[tab_items_idx];
    DCHECK(item.tab_id.is_valid() && item.url.is_valid());

    if (item.session_tag == kadrbrowsielStubSessionTag) {
      NavigateParams params(
          GetSingletonTabNavigateParams(browser_, GURL(kadrbrowsielSyncedTabsUrl)));
      ShowSingletonTabOverwritingNTP(browser_, &params);
      return;
    }
  }

  RecentTabsSubMenuModel::ExecuteCommand(command_id, event_flags);
}
