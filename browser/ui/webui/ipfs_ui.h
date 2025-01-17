/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_UI_WEBUI_IPFS_UI_H_
#define adrbrowsiel_BROWSER_UI_WEBUI_IPFS_UI_H_

#include <memory>
#include <string>
#include <vector>

#include "base/scoped_observation.h"
#include "adrbrowsiel/components/ipfs/ipfs_service.h"
#include "adrbrowsiel/components/ipfs/ipfs_service_observer.h"
#include "content/public/browser/web_ui_controller.h"
#include "content/public/browser/web_ui_message_handler.h"

namespace ipfs {
struct AddressesConfig;
struct RepoStats;
struct NodeInfo;
}  // namespace ipfs

class IPFSDOMHandler : public content::WebUIMessageHandler,
                       public ipfs::IpfsServiceObserver {
 public:
  IPFSDOMHandler();
  ~IPFSDOMHandler() override;

  // WebUIMessageHandler implementation.
  void RegisterMessages() override;

  // ipfs::IpfsServiceObserver overrides:
  void OnIpfsLaunched(bool result, int64_t pid) override;
  void OnIpfsShutdown() override;
  void OnGetConnectedPeers(bool success,
                           const std::vector<std::string>& peers) override;
  void OnInstallationEvent(ipfs::ComponentUpdaterEvents event) override;

 private:
  void HandleGetConnectedPeers(const base::ListValue* args);
  void HandleGetAddressesConfig(const base::ListValue* args);
  void OnGetAddressesConfig(bool success,
                            const ipfs::AddressesConfig& config);
  void HandleGetDaemonStatus(const base::ListValue* args);
  void HandleLaunchDaemon(const base::ListValue* args);
  void LaunchDaemon();
  void OnLaunchDaemon(bool success);
  void HandleShutdownDaemon(const base::ListValue* args);
  void HandleRestartDaemon(const base::ListValue* args);
  void OnShutdownDaemon(bool success);
  void HandleGetRepoStats(const base::ListValue* args);
  void OnGetRepoStats(bool success, const ipfs::RepoStats& stats);
  void HandleGetNodeInfo(const base::ListValue* args);
  void OnGetNodeInfo(bool success, const ipfs::NodeInfo& info);

  void HandleGarbageCollection(const base::ListValue* args);
  void OnGarbageCollection(bool success, const std::string& error);

  base::ScopedObservation<ipfs::IpfsService, ipfs::IpfsServiceObserver>
      service_observer_{this};
  base::WeakPtrFactory<IPFSDOMHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(IPFSDOMHandler);
};

// The WebUI for adrbrowsiel://ipfs
class IPFSUI : public content::WebUIController {
 public:
  IPFSUI(content::WebUI* web_ui, const std::string& host);
  ~IPFSUI() override;
  IPFSUI(const IPFSUI&) = delete;
  IPFSUI& operator=(const IPFSUI&) = delete;
};

#endif  // adrbrowsiel_BROWSER_UI_WEBUI_IPFS_UI_H_
