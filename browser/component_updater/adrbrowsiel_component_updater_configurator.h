/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_COMPONENT_UPDATER_adrbrowsiel_COMPONENT_UPDATER_CONFIGURATOR_H_
#define adrbrowsiel_BROWSER_COMPONENT_UPDATER_adrbrowsiel_COMPONENT_UPDATER_CONFIGURATOR_H_

#include <memory>
#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "components/component_updater/configurator_impl.h"
#include "components/update_client/configurator.h"

class PrefRegistrySimple;
class PrefService;

namespace base {
class CommandLine;
}

namespace net {
class URLRequestContextGetter;
}

namespace component_updater {

class adrbrowsielConfigurator : public update_client::Configurator {
 public:
  adrbrowsielConfigurator(const base::CommandLine* cmdline,
                    PrefService* pref_service);

  // update_client::Configurator overrides.
  double InitialDelay() const override;
  int NextCheckDelay() const override;
  int OnDemandDelay() const override;
  int UpdateDelay() const override;
  std::vector<GURL> UpdateUrl() const override;
  std::vector<GURL> PingUrl() const override;
  std::string GetProdId() const override;
  base::Version GetBrowserVersion() const override;
  std::string GetChannel() const override;
  std::string GetBrand() const override;
  std::string GetLang() const override;
  std::string GetOSLongName() const override;
  base::flat_map<std::string, std::string> ExtraRequestParams() const override;
  std::string GetDownloadPreference() const override;
  scoped_refptr<update_client::NetworkFetcherFactory> GetNetworkFetcherFactory()
      override;
  scoped_refptr<update_client::CrxDownloaderFactory> GetCrxDownloaderFactory()
      override;
  scoped_refptr<update_client::UnzipperFactory> GetUnzipperFactory() override;
  scoped_refptr<update_client::PatcherFactory> GetPatcherFactory() override;
  bool EnabledDeltas() const override;
  bool EnabledComponentUpdates() const override;
  bool EnabledBackgroundDownloader() const override;
  bool EnabledCupSigning() const override;
  PrefService* GetPrefService() const override;
  update_client::ActivityDataService* GetActivityDataService() const override;
  bool IsPerUserInstall() const override;
  std::unique_ptr<update_client::ProtocolHandlerFactory>
  GetProtocolHandlerFactory() const override;

 private:
  friend class base::RefCountedThreadSafe<adrbrowsielConfigurator>;

  ConfiguratorImpl configurator_impl_;
  PrefService* pref_service_;  // This member is not owned by this class.
  scoped_refptr<update_client::NetworkFetcherFactory> network_fetcher_factory_;
  scoped_refptr<update_client::CrxDownloaderFactory> crx_downloader_factory_;
  scoped_refptr<update_client::UnzipperFactory> unzip_factory_;
  scoped_refptr<update_client::PatcherFactory> patch_factory_;

  ~adrbrowsielConfigurator() override;
};

}  // namespace component_updater

#endif  // adrbrowsiel_BROWSER_COMPONENT_UPDATER_adrbrowsiel_COMPONENT_UPDATER_CONFIGURATOR_H_
