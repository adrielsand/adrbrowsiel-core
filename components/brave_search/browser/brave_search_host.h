/* Copyright (c) 2021 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_SEARCH_BROWSER_adrbrowsiel_SEARCH_HOST_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_SEARCH_BROWSER_adrbrowsiel_SEARCH_HOST_H_

#include <list>
#include <map>
#include <memory>
#include <string>

#include "base/memory/weak_ptr.h"
#include "adrbrowsiel/components/adrbrowsiel_search/common/adrbrowsiel_search.mojom.h"
#include "url/gurl.h"

namespace network {
class SharedURLLoaderFactory;
class SimpleURLLoader;
}  // namespace network

namespace adrbrowsiel_search {

class adrbrowsielSearchHost final : public adrbrowsiel_search::mojom::adrbrowsielSearchFallback {
 public:
  adrbrowsielSearchHost(const adrbrowsielSearchHost&) = delete;
  adrbrowsielSearchHost& operator=(const adrbrowsielSearchHost&) = delete;
  explicit adrbrowsielSearchHost(
      scoped_refptr<network::SharedURLLoaderFactory> factory);
  ~adrbrowsielSearchHost() override;

  void FetchBackupResults(const std::string& query_string,
                          const std::string& lang,
                          const std::string& country,
                          const std::string& geo,
                          bool filter_explicit_results,
                          FetchBackupResultsCallback callback) override;

  static GURL GetBackupResultURL(const GURL& baseURL,
                                 const std::string& query,
                                 const std::string& lang,
                                 const std::string& country,
                                 const std::string& geo,
                                 bool filter_explicit_results);
  static void SetBackupProviderForTest(const GURL&);

 private:
  using SimpleURLLoaderList =
      std::list<std::unique_ptr<network::SimpleURLLoader>>;
  using URLRequestCallback =
      base::OnceCallback<void(const int,
                              const std::string&,
                              const std::map<std::string, std::string>&)>;

  void OnURLLoaderComplete(SimpleURLLoaderList::iterator iter,
                           adrbrowsielSearchHost::FetchBackupResultsCallback callback,
                           const std::unique_ptr<std::string> response_body);
  SimpleURLLoaderList url_loaders_;
  scoped_refptr<network::SharedURLLoaderFactory> shared_url_loader_factory_;
  base::WeakPtrFactory<adrbrowsielSearchHost> weak_factory_;
};

}  // namespace adrbrowsiel_search

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_SEARCH_BROWSER_adrbrowsiel_SEARCH_HOST_H_
