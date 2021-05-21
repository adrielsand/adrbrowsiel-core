/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_COMPONENTS_adrbrowsiel_SHIELDS_BROWSER_AD_BLOCK_BASE_SERVICE_H_
#define adrbrowsiel_COMPONENTS_adrbrowsiel_SHIELDS_BROWSER_AD_BLOCK_BASE_SERVICE_H_

#include <stdint.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/files/file_path.h"
#include "base/memory/weak_ptr.h"
#include "base/sequence_checker.h"
#include "base/values.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/base_adrbrowsiel_shields_service.h"
#include "adrbrowsiel/components/adrbrowsiel_component_updater/browser/dat_file_util.h"
#include "third_party/blink/public/mojom/loader/resource_load_info.mojom-shared.h"

class AdBlockServiceTest;

using adrbrowsiel_component_updater::adrbrowsielComponent;
namespace adblock {
class Engine;
}

namespace adrbrowsiel_shields {

// The base class of the adrbrowsiel shields service in charge of ad-block
// checking and init.
class AdBlockBaseService : public BaseadrbrowsielShieldsService {
 public:
  using GetDATFileDataResult =
      adrbrowsiel_component_updater::LoadDATFileDataResult<adblock::Engine>;

  explicit AdBlockBaseService(adrbrowsielComponent::Delegate* delegate);
  ~AdBlockBaseService() override;

  void ShouldStartRequest(const GURL& url,
                          blink::mojom::ResourceType resource_type,
                          const std::string& tab_host,
                          bool* did_match_rule,
                          bool* did_match_exception,
                          bool* did_match_important,
                          std::string* mock_data_url) override;
  base::Optional<std::string> GetCspDirectives(
      const GURL& url,
      blink::mojom::ResourceType resource_type,
      const std::string& tab_host);
  void AddResources(const std::string& resources);
  void EnableTag(const std::string& tag, bool enabled);
  bool TagExists(const std::string& tag);

  virtual base::Optional<base::Value> UrlCosmeticResources(
      const std::string& url);
  virtual base::Optional<base::Value> HiddenClassIdSelectors(
      const std::vector<std::string>& classes,
      const std::vector<std::string>& ids,
      const std::vector<std::string>& exceptions);

 protected:
  friend class ::AdBlockServiceTest;
  bool Init() override;

  void GetDATFileData(const base::FilePath& dat_file_path);
  void AddKnownTagsToAdBlockInstance();
  void AddKnownResourcesToAdBlockInstance();
  void ResetForTest(const std::string& rules, const std::string& resources);

  std::unique_ptr<adblock::Engine> ad_block_client_;

 private:
  void UpdateAdBlockClient(
      std::unique_ptr<adblock::Engine> ad_block_client);
  void OnGetDATFileData(GetDATFileDataResult result);
  void OnPreferenceChanges(const std::string& pref_name);

  std::vector<std::string> tags_;
  std::string resources_;
  base::WeakPtrFactory<AdBlockBaseService> weak_factory_;
  DISALLOW_COPY_AND_ASSIGN(AdBlockBaseService);
};

}  // namespace adrbrowsiel_shields

#endif  // adrbrowsiel_COMPONENTS_adrbrowsiel_SHIELDS_BROWSER_AD_BLOCK_BASE_SERVICE_H_
