/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 3.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "adrbrowsiel/components/cosmetic_filters/browser/cosmetic_filters_resources.h"

#include <utility>

#include "base/json/json_reader.h"
#include "base/optional.h"
#include "base/values.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/ad_block_service.h"
#include "adrbrowsiel/components/adrbrowsiel_shields/browser/adrbrowsiel_shields_util.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"

namespace cosmetic_filters {

CosmeticFiltersResources::CosmeticFiltersResources(
    HostContentSettingsMap* settings_map,
    adrbrowsiel_shields::AdBlockService* ad_block_service)
    : settings_map_(settings_map),
      ad_block_service_(ad_block_service),
      weak_factory_(this) {}

CosmeticFiltersResources::~CosmeticFiltersResources() {}

void CosmeticFiltersResources::HiddenClassIdSelectors(
    const std::string& input,
    const std::vector<std::string>& exceptions,
    HiddenClassIdSelectorsCallback callback) {
  base::Optional<base::Value> input_value = base::JSONReader::Read(input);
  if (!input_value || !input_value->is_dict()) {
    // Nothing to work with
    std::move(callback).Run(base::Value());

    return;
  }
  base::DictionaryValue* input_dict;
  if (!input_value->GetAsDictionary(&input_dict)) {
    std::move(callback).Run(base::Value());

    return;
  }
  std::vector<std::string> classes;
  base::ListValue* classes_list;
  if (input_dict->GetList("classes", &classes_list)) {
    for (size_t i = 0; i < classes_list->GetSize(); i++) {
      if (!classes_list->GetList()[i].is_string()) {
        continue;
      }
      classes.push_back(classes_list->GetList()[i].GetString());
    }
  }
  std::vector<std::string> ids;
  base::ListValue* ids_list;
  if (input_dict->GetList("ids", &ids_list)) {
    for (size_t i = 0; i < ids_list->GetSize(); i++) {
      if (!ids_list->GetList()[i].is_string()) {
        continue;
      }
      ids.push_back(ids_list->GetList()[i].GetString());
    }
  }

  ad_block_service_->GetTaskRunner()->PostTaskAndReplyWithResult(
      FROM_HERE,
      base::BindOnce(&adrbrowsiel_shields::AdBlockService::HiddenClassIdSelectors,
                     base::Unretained(ad_block_service_), classes, ids,
                     exceptions),
      base::BindOnce(&CosmeticFiltersResources::HiddenClassIdSelectorsOnUI,
                     weak_factory_.GetWeakPtr(), std::move(callback)));
}

void CosmeticFiltersResources::HiddenClassIdSelectorsOnUI(
    HiddenClassIdSelectorsCallback callback,
    base::Optional<base::Value> resources) {
  std::move(callback).Run(resources ? std::move(resources.value())
                                    : base::Value());
}

void CosmeticFiltersResources::UrlCosmeticResourcesOnUI(
    UrlCosmeticResourcesCallback callback,
    base::Optional<base::Value> resources) {
  std::move(callback).Run(resources ? std::move(resources.value())
                                    : base::Value());
}

void CosmeticFiltersResources::ShouldDoCosmeticFiltering(
    const std::string& url,
    ShouldDoCosmeticFilteringCallback callback) {
  bool enabled =
      adrbrowsiel_shields::ShouldDoCosmeticFiltering(settings_map_, GURL(url));
  bool first_party_enabled =
      adrbrowsiel_shields::IsFirstPartyCosmeticFilteringEnabled(settings_map_,
                                                          GURL(url));
  std::move(callback).Run(enabled, first_party_enabled);
}

void CosmeticFiltersResources::UrlCosmeticResources(
    const std::string& url,
    UrlCosmeticResourcesCallback callback) {
  ad_block_service_->GetTaskRunner()->PostTaskAndReplyWithResult(
      FROM_HERE,
      base::BindOnce(&adrbrowsiel_shields::AdBlockService::UrlCosmeticResources,
                     base::Unretained(ad_block_service_), url),
      base::BindOnce(&CosmeticFiltersResources::UrlCosmeticResourcesOnUI,
                     weak_factory_.GetWeakPtr(), std::move(callback)));
}

}  // namespace cosmetic_filters
