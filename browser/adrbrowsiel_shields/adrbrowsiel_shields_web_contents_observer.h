/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_adrbrowsiel_SHIELDS_adrbrowsiel_SHIELDS_WEB_CONTENTS_OBSERVER_H_
#define adrbrowsiel_BROWSER_adrbrowsiel_SHIELDS_adrbrowsiel_SHIELDS_WEB_CONTENTS_OBSERVER_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/containers/flat_map.h"
#include "base/macros.h"
#include "base/synchronization/lock.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/common/adrbrowsiel_shields.mojom.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_receiver_set.h"
#include "content/public/browser/web_contents_user_data.h"

namespace content {
class WebContents;
}

class PrefRegistrySimple;

namespace adrbrowsiel_shields {

class adrbrowsielShieldsWebContentsObserver
    : public content::WebContentsObserver,
      public content::WebContentsUserData<adrbrowsielShieldsWebContentsObserver>,
      public adrbrowsiel_shields::mojom::adrbrowsielShieldsHost {
 public:
  explicit adrbrowsielShieldsWebContentsObserver(content::WebContents*);
  ~adrbrowsielShieldsWebContentsObserver() override;

  static void RegisterProfilePrefs(PrefRegistrySimple* registry);
  static void DispatchBlockedEventForWebContents(
      const std::string& block_type,
      const std::string& subresource,
      content::WebContents* web_contents);
  static void DispatchBlockedEvent(const GURL& request_url,
                                   int frame_tree_node_id,
                                   const std::string& block_type);
  static GURL GetTabURLFromRenderFrameInfo(int render_frame_tree_node_id);
  void AllowScriptsOnce(const std::vector<std::string>& origins,
                        content::WebContents* web_contents);
  bool IsBlockedSubresource(const std::string& subresource);
  void AddBlockedSubresource(const std::string& subresource);

 protected:
  // content::WebContentsObserver overrides.
  void RenderFrameCreated(content::RenderFrameHost* host) override;
  void RenderFrameDeleted(content::RenderFrameHost* render_frame_host) override;
  void RenderFrameHostChanged(content::RenderFrameHost* old_host,
                              content::RenderFrameHost* new_host) override;
  void ReadyToCommitNavigation(
      content::NavigationHandle* navigation_handle) override;
  void DidFinishNavigation(
      content::NavigationHandle* navigation_handle) override;

  // adrbrowsiel_shields::mojom::adrbrowsielShieldsHost.
  void OnJavaScriptBlocked(const std::u16string& details) override;

 private:
  friend class content::WebContentsUserData<adrbrowsielShieldsWebContentsObserver>;

  using adrbrowsielShieldsRemotesMap = base::flat_map<
      content::RenderFrameHost*,
      mojo::AssociatedRemote<adrbrowsiel_shields::mojom::adrbrowsielShields>>;

  // Return an already bound remote for the adrbrowsiel_shields::mojom::adrbrowsielShields
  // mojo interface. It is an error to call this method with an invalid |rfh|.
  mojo::AssociatedRemote<adrbrowsiel_shields::mojom::adrbrowsielShields>&
  GetadrbrowsielShieldsRemote(content::RenderFrameHost* rfh);

  std::vector<std::string> allowed_script_origins_;
  // We keep a set of the current page's blocked URLs in case the page
  // continually tries to load the same blocked URLs.
  std::set<std::string> blocked_url_paths_;

  content::WebContentsFrameReceiverSet<adrbrowsiel_shields::mojom::adrbrowsielShieldsHost>
      adrbrowsiel_shields_receivers_;

  // Map of remote endpoints for the adrbrowsiel_shields::mojom::adrbrowsielShields mojo
  // interface, to prevent binding a new remote each time it's used.
  adrbrowsielShieldsRemotesMap adrbrowsiel_shields_remotes_;

  WEB_CONTENTS_USER_DATA_KEY_DECL();
  DISALLOW_COPY_AND_ASSIGN(adrbrowsielShieldsWebContentsObserver);
};

}  // namespace adrbrowsiel_shields

#endif  // adrbrowsiel_BROWSER_adrbrowsiel_SHIELDS_adrbrowsiel_SHIELDS_WEB_CONTENTS_OBSERVER_H_
