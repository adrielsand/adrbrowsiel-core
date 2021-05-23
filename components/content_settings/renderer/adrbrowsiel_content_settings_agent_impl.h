/* Copyright 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_CONTENT_SETTINGS_RENDERER_adrbrowsiel_CONTENT_SETTINGS_AGENT_IMPL_H_
#define adrbrowsiel_COMPONENTS_CONTENT_SETTINGS_RENDERER_adrbrowsiel_CONTENT_SETTINGS_AGENT_IMPL_H_

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/containers/flat_map.h"
#include "base/containers/flat_set.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/common/adrbrowsiel_shields.mojom.h"
#include "adrbrowsiel/third_party/blink/renderer/adrbrowsiel_farbling_constants.h"
#include "components/content_settings/core/common/content_settings.h"
#include "components/content_settings/core/common/content_settings_types.h"
#include "components/content_settings/renderer/content_settings_agent_impl.h"
#include "mojo/public/cpp/bindings/associated_receiver_set.h"
#include "mojo/public/cpp/bindings/associated_remote.h"
#include "mojo/public/cpp/bindings/pending_associated_receiver.h"

#include "url/gurl.h"

namespace blink {
class WebLocalFrame;
}

namespace content_settings {

// Handles blocking content per content settings for each RenderFrame.
class adrbrowsielContentSettingsAgentImpl
    : public ContentSettingsAgentImpl,
      public adrbrowsiel_shields::mojom::adrbrowsielShields {
 public:
  adrbrowsielContentSettingsAgentImpl(content::RenderFrame* render_frame,
                                bool should_whitelist,
                                std::unique_ptr<Delegate> delegate);
  ~adrbrowsielContentSettingsAgentImpl() override;

 protected:
  bool AllowScript(bool enabled_per_settings) override;
  bool AllowScriptFromSource(bool enabled_per_settings,
                             const blink::WebURL& script_url) override;
  void DidNotAllowScript() override;

  bool UseEphemeralStorageSync(StorageType storage_type) override;
  bool AllowStorageAccessSync(StorageType storage_type) override;

  void adrbrowsielSpecificDidBlockJavaScript(const std::u16string& details);

  bool AllowAutoplay(bool play_requested) override;

  bool AllowFingerprinting(bool enabled_per_settings) override;

  adrbrowsielFarblingLevel GetadrbrowsielFarblingLevel() override;

 private:
  FRIEND_TEST_ALL_PREFIXES(adrbrowsielContentSettingsAgentImplAutoplayBrowserTest,
                           AutoplayBlockedByDefault);
  FRIEND_TEST_ALL_PREFIXES(adrbrowsielContentSettingsAgentImplAutoplayBrowserTest,
                           AutoplayAllowedByDefault);

  bool IsadrbrowsielShieldsDown(
      const blink::WebFrame* frame,
      const GURL& secondary_url);

  // RenderFrameObserver
  void DidCommitProvisionalLoad(ui::PageTransition transition) override;

  bool IsScriptTemporilyAllowed(const GURL& script_url);
  bool AllowStorageAccessForMainFrameSync(StorageType storage_type);

  // adrbrowsiel_shields::mojom::adrbrowsielShields.
  void SetAllowScriptsFromOriginsOnce(
      const std::vector<std::string>& origins) override;

  void BindadrbrowsielShieldsReceiver(
      mojo::PendingAssociatedReceiver<adrbrowsiel_shields::mojom::adrbrowsielShields>
          pending_receiver);

  // Returns the associated remote used to send messages to the browser process,
  // lazily initializing it the first time it's used.
  mojo::AssociatedRemote<adrbrowsiel_shields::mojom::adrbrowsielShieldsHost>&
  GetOrCreateadrbrowsielShieldsRemote();

  // Origins of scripts which are temporary allowed for this frame in the
  // current load
  base::flat_set<std::string> temporarily_allowed_scripts_;

  // cache blocked script url which will later be used in `DidNotAllowScript()`
  GURL blocked_script_url_;

  // temporary allowed script origins we preloaded for the next load
  base::flat_set<std::string> preloaded_temporarily_allowed_scripts_;

  using StoragePermissionsKey = std::pair<url::Origin, StorageType>;
  base::flat_map<StoragePermissionsKey, bool> cached_storage_permissions_;

  mojo::AssociatedRemote<adrbrowsiel_shields::mojom::adrbrowsielShieldsHost>
      adrbrowsiel_shields_remote_;

  mojo::AssociatedReceiverSet<adrbrowsiel_shields::mojom::adrbrowsielShields>
      adrbrowsiel_shields_receivers_;

  DISALLOW_COPY_AND_ASSIGN(adrbrowsielContentSettingsAgentImpl);
};

}  // namespace content_settings

#endif  // adrbrowsiel_COMPONENTS_CONTENT_SETTINGS_RENDERER_adrbrowsiel_CONTENT_SETTINGS_AGENT_IMPL_H_