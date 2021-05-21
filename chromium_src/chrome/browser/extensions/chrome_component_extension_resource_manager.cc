/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chrome/browser/extensions/chrome_component_extension_resource_manager.h"

#include "adrbrowsiel/components/adrbrowsiel_extension/grit/adrbrowsiel_extension_generated_map.h"
#include "adrbrowsiel/components/adrbrowsiel_extension/grit/adrbrowsiel_extension_resources_map.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/browser/buildflags/buildflags.h"
#include "adrbrowsiel/components/adrbrowsiel_webtorrent/browser/buildflags/buildflags.h"

#if BUILDFLAG(ENABLE_adrbrowsiel_WEBTORRENT)
#include "adrbrowsiel/components/adrbrowsiel_webtorrent/grit/adrbrowsiel_webtorrent_generated_map.h"
#include "adrbrowsiel/components/adrbrowsiel_webtorrent/grit/adrbrowsiel_webtorrent_resources_map.h"
#endif

#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
#include "adrbrowsiel/components/adrbrowsiel_rewards/resources/extension/grit/adrbrowsiel_rewards_extension_resources_map.h"
#include "adrbrowsiel/components/adrbrowsiel_rewards/resources/extension/grit/adrbrowsiel_rewards_panel_generated_map.h"
#endif

#if BUILDFLAG(adrbrowsiel_REWARDS_ENABLED)
#define adrbrowsiel_REWARDS_EXTENSION_RESOURCES                           \
  AddComponentResourceEntries(kadrbrowsielRewardsExtensionResources,      \
                              kadrbrowsielRewardsExtensionResourcesSize); \
  AddComponentResourceEntries(kadrbrowsielRewardsPanelGenerated,          \
                              kadrbrowsielRewardsPanelGeneratedSize);
#else
#define adrbrowsiel_REWARDS_EXTENSION_RESOURCES
#endif

#if BUILDFLAG(ENABLE_adrbrowsiel_WEBTORRENT)
#define adrbrowsiel_WEBTORRENT_RESOURCES                            \
  AddComponentResourceEntries(kadrbrowsielWebtorrentResources,      \
                              kadrbrowsielWebtorrentResourcesSize); \
  AddComponentResourceEntries(kadrbrowsielWebtorrentGenerated,      \
                              kadrbrowsielWebtorrentGeneratedSize);
#else
#define adrbrowsiel_WEBTORRENT_RESOURCES
#endif

#define adrbrowsiel_CHROME_COMPONENT_EXTENSION_RESOURCE_MANAGER_DATA_DATA  \
  AddComponentResourceEntries(kadrbrowsielExtension, kadrbrowsielExtensionSize); \
  AddComponentResourceEntries(kadrbrowsielExtensionGenerated,              \
                              kadrbrowsielExtensionGeneratedSize);         \
  adrbrowsiel_REWARDS_EXTENSION_RESOURCES                                  \
  adrbrowsiel_WEBTORRENT_RESOURCES

#include "../../../../../chrome/browser/extensions/chrome_component_extension_resource_manager.cc"
#undef adrbrowsiel_CHROME_COMPONENT_EXTENSION_RESOURCE_MANAGER_DATA_DATA
