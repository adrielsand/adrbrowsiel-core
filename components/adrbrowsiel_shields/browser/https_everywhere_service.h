/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_SHIELDS_BROWSER_HTTPS_EVERYWHERE_SERVICE_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_SHIELDS_BROWSER_HTTPS_EVERYWHERE_SERVICE_H_

#include <memory>
#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/memory/weak_ptr.h"
#include "base/sequence_checker.h"
#include "base/synchronization/lock.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/base_adrbrowsiel_shields_service.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/https_everywhere_recently_used_cache.h"

namespace leveldb {
class DB;
}

class HTTPSEverywhereServiceTest;

using adrbrowsiel_component_updater::adrbrowsielComponent;

namespace adrbrowsiel_shields {

extern const char kHTTPSEverywhereComponentName[];
extern const char kHTTPSEverywhereComponentId[];
extern const char kHTTPSEverywhereComponentBase64PublicKey[];

struct HTTPSE_REDIRECTS_COUNT_ST {
 public:
  HTTPSE_REDIRECTS_COUNT_ST(uint64_t request_identifier,
                            unsigned int redirects):
    request_identifier_(request_identifier),
    redirects_(redirects) {
  }

  uint64_t request_identifier_;
  unsigned int redirects_;
};

class HTTPSEverywhereService : public BaseadrbrowsielShieldsService,
                         public base::SupportsWeakPtr<HTTPSEverywhereService> {
 public:
  explicit HTTPSEverywhereService(adrbrowsielComponent::Delegate* delegate);
  ~HTTPSEverywhereService() override;
  bool GetHTTPSURL(const GURL* url,
                   const uint64_t& request_id,
                   std::string* new_url);
  bool GetHTTPSURLFromCacheOnly(const GURL* url,
                                const uint64_t& request_id,
                                std::string* cached_url);

 protected:
  bool Init() override;
  void OnComponentReady(const std::string& component_id,
      const base::FilePath& install_dir,
      const std::string& manifest) override;

  void AddHTTPSEUrlToRedirectList(const uint64_t& request_id);
  bool ShouldHTTPSERedirect(const uint64_t& request_id);
  std::string ApplyHTTPSRule(const std::string& originalUrl,
      const std::string& rule);
  std::string CorrecttoRuleToRE2Engine(const std::string& to);

 private:
  friend class ::HTTPSEverywhereServiceTest;
  static bool g_ignore_port_for_test_;
  static std::string g_https_everywhere_component_id_;
  static std::string g_https_everywhere_component_base64_public_key_;
  static void SetIgnorePortForTest(bool ignore);
  static void SetComponentIdAndBase64PublicKeyForTest(
      const std::string& component_id,
      const std::string& component_base64_public_key);

  void CloseDatabase();

  void InitDB(const base::FilePath& install_dir);

  base::Lock httpse_get_urls_redirects_count_mutex_;
  std::vector<HTTPSE_REDIRECTS_COUNT_ST> httpse_urls_redirects_count_;
  HTTPSERecentlyUsedCache<std::string> recently_used_cache_;
  leveldb::DB* level_db_;

  SEQUENCE_CHECKER(sequence_checker_);
  DISALLOW_COPY_AND_ASSIGN(HTTPSEverywhereService);
};

// Creates the HTTPSEverywhereService
std::unique_ptr<HTTPSEverywhereService> HTTPSEverywhereServiceFactory(
    adrbrowsielComponent::Delegate* delegate);

}  // namespace adrbrowsiel_shields

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_SHIELDS_BROWSER_HTTPS_EVERYWHERE_SERVICE_H_
