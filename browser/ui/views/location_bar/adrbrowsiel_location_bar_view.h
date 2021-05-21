/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_VIEWS_LOCATION_BAR_adrbrowsiel_LOCATION_BAR_VIEW_H_
#define adrbrowsiel_BROWSER_UI_VIEWS_LOCATION_BAR_adrbrowsiel_LOCATION_BAR_VIEW_H_

#include <vector>

#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"
#include "adrbrowsiel/components/tor/buildflags/buildflags.h"
#include "chrome/browser/ui/views/location_bar/location_bar_view.h"

class adrbrowsielActionsContainer;
class adrbrowsielActionsContainerTest;
class RewardsBrowserTest;
class SkPath;

#if BUILDFLAG(ENABLE_TOR)
class OnionLocationView;
#endif

#if BUILDFLAG(IPFS_ENABLED)
class IPFSLocationView;
#endif

// The purposes of this subclass are to:
// - Add the adrbrowsielActionsContainer to the location bar
class adrbrowsielLocationBarView : public LocationBarView {
 public:
  using LocationBarView::LocationBarView;
  void Init() override;
  void Update(content::WebContents* contents) override;
  void OnChanged() override;
  adrbrowsielActionsContainer* GetadrbrowsielActionsContainer() { return adrbrowsiel_actions_; }
#if BUILDFLAG(ENABLE_TOR)
  OnionLocationView* GetOnionLocationView() { return onion_location_view_; }
#endif

#if BUILDFLAG(IPFS_ENABLED)
  IPFSLocationView* GetIPFSLocationView() { return ipfs_location_view_; }
#endif
  // LocationBarView:
  std::vector<views::View*> GetTrailingViews() override;

  ui::ImageModel GetLocationIcon(LocationIconView::Delegate::IconFetchedCallback
                                     on_icon_fetched) const override;

  // views::View:
  gfx::Size CalculatePreferredSize() const override;
  void OnThemeChanged() override;
  void ChildPreferredSizeChanged(views::View* child) override;

  int GetBorderRadius() const override;

  SkPath GetFocusRingHighlightPath() const;
  ContentSettingImageView* GetContentSettingsImageViewForTesting(size_t idx);
  bool ShouldShowIPFSLocationView() const;

 private:
  friend class ::adrbrowsielActionsContainerTest;
  friend class ::RewardsBrowserTest;
  adrbrowsielActionsContainer* adrbrowsiel_actions_ = nullptr;
#if BUILDFLAG(ENABLE_TOR)
  OnionLocationView* onion_location_view_ = nullptr;
#endif
#if BUILDFLAG(IPFS_ENABLED)
  IPFSLocationView* ipfs_location_view_ = nullptr;
#endif

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielLocationBarView);
};

#endif  // adrbrowsiel_BROWSER_UI_VIEWS_LOCATION_BAR_adrbrowsiel_LOCATION_BAR_VIEW_H_
