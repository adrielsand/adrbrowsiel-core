/* Copyright (c) 2019 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/ads_history_info.h"

#include "bat/ads/ad_history_info.h"
#include "bat/ads/internal/json_helper.h"
#include "bat/ads/internal/logging.h"

namespace ads {

AdsHistoryInfo::AdsHistoryInfo() = default;

AdsHistoryInfo::AdsHistoryInfo(const AdsHistoryInfo& info) = default;

AdsHistoryInfo::~AdsHistoryInfo() = default;

std::string AdsHistoryInfo::ToJson() const {
  std::string json;
  SaveToJson(*this, &json);
  return json;
}

Result AdsHistoryInfo::FromJson(const std::string& json) {
  rapidjson::Document document;
  document.Parse(json.c_str());

  if (document.HasParseError()) {
    BLOG(1, helper::JSON::GetLastError(&document));
    return FAILED;
  }

  if (document.HasMember("ads_history")) {
    for (const auto& item : document["ads_history"].GetArray()) {
      AdHistoryInfo ad_history;
      rapidjson::StringBuffer buffer;
      rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
      if (item.Accept(writer) &&
          ad_history.FromJson(buffer.GetString()) == SUCCESS) {
        items.push_back(ad_history);
      }
    }
  }

  return SUCCESS;
}

void SaveToJson(JsonWriter* writer, const AdsHistoryInfo& ads_history) {
  writer->StartObject();

  writer->String("ads_history");
  writer->StartArray();
  for (const auto& item : ads_history.items) {
    SaveToJson(writer, item);
  }
  writer->EndArray();

  writer->EndObject();
}

}  // namespace ads
