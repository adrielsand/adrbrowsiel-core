/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/browser/ui/views/location_bar/adrbrowsiel_location_bar_view.h"

#include <memory>
#include <utility>

#include "adrbrowsiel/app/vector_icons/vector_icons.h"
#include "adrbrowsiel/browser/profiles/profile_util.h"
#include "adrbrowsiel/browser/themes/adrbrowsiel_theme_service.h"
#include "adrbrowsiel/browser/ui/views/adrbrowsiel_actions/adrbrowsiel_actions_container.h"
#include "adrbrowsiel/browser/ui/views/toolbar/adrbrowsiel_toolbar_view.h"
#include "adrbrowsiel/grit/adrbrowsiel_theme_resources.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/themes/theme_service_factory.h"
#include "chrome/browser/ui/layout_constants.h"
#include "chrome/browser/ui/omnibox/omnibox_theme.h"
#include "chrome/browser/ui/views/chrome_layout_provider.h"
#include "chrome/browser/ui/views/location_bar/location_bar_view.h"
#include "chrome/grit/chromium_strings.h"
#include "components/grit/adrbrowsiel_components_strings.h"
#include "components/omnibox/browser/omnibox_edit_model.h"
#include "components/version_info/channel.h"
#include "content/public/browser/navigation_entry.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/paint_vector_icon.h"
#include "ui/views/controls/highlight_path_generator.h"

#if BUILDFLAG(ENABLE_TOR)
#include "adrbrowsiel/browser/ui/views/location_bar/onion_location_view.h"
#endif
#if BUILDFLAG(IPFS_ENABLED)
#include "adrbrowsiel/browser/ui/views/location_bar/ipfs_location_view.h"
#include "adrbrowsiel/components/ipfs/ipfs_constants.h"
#include "adrbrowsiel/components/ipfs/ipfs_utils.h"
#endif

namespace {

class adrbrowsielLocationBarViewFocusRingHighlightPathGenerator
    : public views::HighlightPathGenerator {
 public:
  adrbrowsielLocationBarViewFocusRingHighlightPathGenerator() = default;

  // HighlightPathGenerator
  SkPath GetHighlightPath(const views::View* view) override {
    return static_cast<const adrbrowsielLocationBarView*>(view)
        ->GetFocusRingHighlightPath();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(adrbrowsielLocationBarViewFocusRingHighlightPathGenerator);
};

base::Optional<SkColor> GetFocusRingColor(Profile* profile) {
  constexpr SkColor kPrivateFocusRingColor = SkColorSetRGB(0xC6, 0xB3, 0xFF);
  constexpr SkColor kTorPrivateFocusRingColor = SkColorSetRGB(0xCF, 0xAB, 0xE2);
  if (adrbrowsiel::IsRegularProfile(profile) || profile->IsGuestSession()) {
    // Don't update color.
    return base::nullopt;
  }
  if (profile->IsTor())
    return kTorPrivateFocusRingColor;

  // Private window.
  return kPrivateFocusRingColor;
}

}  // namespace

void adrbrowsielLocationBarView::Init() {
  // base method calls Update and Layout
  LocationBarView::Init();
  // Change focus ring highlight path
  if (focus_ring_) {
    focus_ring_->SetPathGenerator(
        std::make_unique<
            adrbrowsielLocationBarViewFocusRingHighlightPathGenerator>());
    if (const auto color = GetFocusRingColor(profile()))
      focus_ring_->SetColor(color.value());
  }
#if BUILDFLAG(ENABLE_TOR)
  onion_location_view_ = new OnionLocationView(browser_->profile());
  AddChildView(onion_location_view_);
#endif
#if BUILDFLAG(IPFS_ENABLED)
  ipfs_location_view_ = new IPFSLocationView(browser_->profile());
  AddChildView(ipfs_location_view_);
#endif

  // adrbrowsiel action buttons
  adrbrowsiel_actions_ = new adrbrowsielActionsContainer(browser_, profile());
  adrbrowsiel_actions_->Init();
  AddChildView(adrbrowsiel_actions_);
  // Call Update again to cause a Layout
  Update(nullptr);

  // Stop slide animation for all content settings views icon.
  for (auto* content_setting_view : content_setting_views_)
    content_setting_view->disable_animation();
}

bool adrbrowsielLocationBarView::ShouldShowIPFSLocationView() const {
#if BUILDFLAG(IPFS_ENABLED)
  const GURL& url = GetLocationBarModel()->GetURL();
  if (!ipfs::IsIPFSScheme(url) || !ipfs::IsIpfsEnabled(profile_) ||
      !ipfs::IsLocalGatewayConfigured(profile_))
    return false;

  return true;
#else
  return false;
#endif
}

void adrbrowsielLocationBarView::Update(content::WebContents* contents) {
  // base Init calls update before our Init is run, so our children
  // may not be initialized yet
  if (adrbrowsiel_actions_) {
    adrbrowsiel_actions_->Update();
  }
#if BUILDFLAG(ENABLE_TOR)
  if (onion_location_view_)
    onion_location_view_->Update(contents);
#endif
#if BUILDFLAG(IPFS_ENABLED)
  if (ipfs_location_view_)
    ipfs_location_view_->Update(contents);
#endif

  LocationBarView::Update(contents);

  if (!ShouldShowIPFSLocationView())
    return;
  // Secure display text for a page was set by chromium.
  // We do not want to override this.
  if (!GetLocationBarModel()->GetSecureDisplayText().empty())
    return;
  auto badge_text = l10n_util::GetStringUTF16(IDS_IPFS_BADGE_TITLE);
  location_icon_view()->SetLabel(badge_text);
}

ui::ImageModel adrbrowsielLocationBarView::GetLocationIcon(
    LocationIconView::Delegate::IconFetchedCallback on_icon_fetched) const {
  if (!ShouldShowIPFSLocationView() ||
      !omnibox_view_->model()->ShouldShowCurrentPageIcon())
    return LocationBarView::GetLocationIcon(std::move(on_icon_fetched));

  auto& bundle = ui::ResourceBundle::GetSharedInstance();
  const auto& ipfs_logo = *bundle.GetImageSkiaNamed(IDR_adrbrowsiel_IPFS_LOGO);
  return ui::ImageModel::FromImageSkia(ipfs_logo);
}

void adrbrowsielLocationBarView::OnChanged() {
  if (adrbrowsiel_actions_) {
    // Do not show actions whilst omnibar is open or url is being edited
    const bool should_hide =
        ShouldHidePageActionIcons() && !omnibox_view_->GetText().empty();
    adrbrowsiel_actions_->SetShouldHide(should_hide);
  }
#if BUILDFLAG(ENABLE_TOR)
  if (onion_location_view_)
    onion_location_view_->Update(
        browser_->tab_strip_model()->GetActiveWebContents());
#endif
#if BUILDFLAG(IPFS_ENABLED)
  if (ipfs_location_view_)
    ipfs_location_view_->Update(
        browser_->tab_strip_model()->GetActiveWebContents());
#endif

  // OnChanged calls Layout
  LocationBarView::OnChanged();
}

std::vector<views::View*> adrbrowsielLocationBarView::GetTrailingViews() {
  std::vector<views::View*> views;
#if BUILDFLAG(ENABLE_TOR)
  if (onion_location_view_)
    views.push_back(onion_location_view_);
#endif
#if BUILDFLAG(IPFS_ENABLED)
  if (ipfs_location_view_)
    views.push_back(ipfs_location_view_);
#endif

  if (adrbrowsiel_actions_)
    views.push_back(adrbrowsiel_actions_);

  return views;
}

gfx::Size adrbrowsielLocationBarView::CalculatePreferredSize() const {
  gfx::Size min_size = LocationBarView::CalculatePreferredSize();
  if (adrbrowsiel_actions_ && adrbrowsiel_actions_->GetVisible()) {
    const int adrbrowsiel_actions_min = adrbrowsiel_actions_->GetMinimumSize().width();
    const int extra_width = adrbrowsiel_actions_min +
                              GetLayoutConstant(LOCATION_BAR_ELEMENT_PADDING);
    min_size.Enlarge(extra_width, 0);
  }
#if BUILDFLAG(ENABLE_TOR)
  if (onion_location_view_ && onion_location_view_->GetVisible()) {
    const int extra_width = GetLayoutConstant(LOCATION_BAR_ELEMENT_PADDING) +
        onion_location_view_->GetMinimumSize().width();
    min_size.Enlarge(extra_width, 0);
  }
#endif
#if BUILDFLAG(IPFS_ENABLED)
  if (ipfs_location_view_ && ipfs_location_view_->GetVisible()) {
    const int extra_width = GetLayoutConstant(LOCATION_BAR_ELEMENT_PADDING) +
                            ipfs_location_view_->GetMinimumSize().width();
    min_size.Enlarge(extra_width, 0);
  }
#endif

  return min_size;
}

void adrbrowsielLocationBarView::OnThemeChanged() {
  LocationBarView::OnThemeChanged();

  if (!IsInitialized())
    return;

  Update(nullptr);
  RefreshBackground();
}

void adrbrowsielLocationBarView::ChildPreferredSizeChanged(views::View* child) {
  LocationBarView::ChildPreferredSizeChanged(child);

  if (child != adrbrowsiel_actions_)
    return;

  Layout();
}

int adrbrowsielLocationBarView::GetBorderRadius() const {
  return ChromeLayoutProvider::Get()->GetCornerRadiusMetric(
      views::Emphasis::kHigh, size());
}

SkPath adrbrowsielLocationBarView::GetFocusRingHighlightPath() const {
  const SkScalar radius = GetBorderRadius();
  return SkPath().addRoundRect(gfx::RectToSkRect(GetLocalBounds()),
                               radius, radius);
}

ContentSettingImageView*
adrbrowsielLocationBarView::GetContentSettingsImageViewForTesting(size_t idx) {
  DCHECK(idx < content_setting_views_.size());
  return content_setting_views_[idx];
}
