/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_adrbrowsiel_DRM_TAB_HELPER_H_
#define adrbrowsiel_BROWSER_adrbrowsiel_DRM_TAB_HELPER_H_

#include <string>

#include "base/scoped_observer.h"
#include "adrbrowsiel/components/adrbrowsiel_drm/adrbrowsiel_drm.mojom.h"
#include "components/component_updater/component_updater_service.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_receiver_set.h"
#include "content/public/browser/web_contents_user_data.h"

// Reacts to DRM content detected on the renderer side.
class adrbrowsielDrmTabHelper final
    : public content::WebContentsObserver,
      public content::WebContentsUserData<adrbrowsielDrmTabHelper>,
      public adrbrowsiel_drm::mojom::adrbrowsielDRM,
      public component_updater::ComponentUpdateService::Observer {
 public:
  // Copied from widevine_cdm_component_installer.cc.
  // There is no shared constant value.
  static const char kWidevineComponentId[];

  explicit adrbrowsielDrmTabHelper(content::WebContents* contents);
  ~adrbrowsielDrmTabHelper() override;

  bool ShouldShowWidevineOptIn() const;

  // content::WebContentsObserver
  void DidStartNavigation(
      content::NavigationHandle* navigation_handle) override;

  // blink::mojom::adrbrowsielDRM
  void OnWidevineKeySystemAccessRequest() override;

  // component_updater::ComponentUpdateService::Observer
  void OnEvent(Events event, const std::string& id) override;

  WEB_CONTENTS_USER_DATA_KEY_DECL();

 private:
  content::WebContentsFrameReceiverSet<adrbrowsiel_drm::mojom::adrbrowsielDRM> receivers_;

  // Permission request is done only once during the navigation. If user
  // chooses dismiss/deny, additional request is added again only when new
  // main frame navigation is started.
  bool is_permission_requested_ = false;

  // True if we are notified that a page requested widevine availability.
  bool is_widevine_requested_ = false;

  ScopedObserver<component_updater::ComponentUpdateService,
                 component_updater::ComponentUpdateService::Observer> observer_;
};

#endif  // adrbrowsiel_BROWSER_adrbrowsiel_DRM_TAB_HELPER_H_
