/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/legacy_migration/legacy_conversion_migration.h"

#include <cstdint>
#include <string>

#include "base/json/json_reader.h"
#include "base/optional.h"
#include "base/strings/string_number_conversions.h"
#include "base/values.h"
#include "bat/ads/internal/ads_client_helper.h"
#include "bat/ads/internal/conversions/conversion_queue_item_info.h"
#include "bat/ads/internal/database/tables/conversion_queue_database_table.h"
#include "bat/ads/internal/logging.h"
#include "bat/ads/pref_names.h"
#include "bat/ads/result.h"

namespace ads {
namespace conversions {

namespace {

const char kFilename[] = "ad_conversions.json";

const char kListKey[] = "ad_conversions";
const char kTimestampKey[] = "timestamp_in_seconds";
const char kCreativeSetIdKey[] = "creative_set_id";
const char kCreativeInstanceIdKey[] = "uuid";

base::Optional<ConversionQueueItemInfo> GetFromDictionary(
    const base::DictionaryValue* dictionary) {
  DCHECK(dictionary);

  // Timestamp
  const std::string* timestamp_value = dictionary->FindStringKey(kTimestampKey);
  if (!timestamp_value) {
    return base::nullopt;
  }

  int64_t timestamp;
  if (!base::StringToInt64(*timestamp_value, &timestamp)) {
    return base::nullopt;
  }

  // Creative set id
  const std::string* creative_set_id_value =
      dictionary->FindStringKey(kCreativeSetIdKey);
  if (!creative_set_id_value) {
    return base::nullopt;
  }

  // Creative instance id
  const auto* creative_instance_id_value =
      dictionary->FindStringKey(kCreativeInstanceIdKey);
  if (!creative_instance_id_value) {
    return base::nullopt;
  }

  ConversionQueueItemInfo conversion_queue_item;
  conversion_queue_item.creative_set_id = *creative_set_id_value;
  conversion_queue_item.creative_instance_id = *creative_instance_id_value;
  conversion_queue_item.timestamp = base::Time::FromDoubleT(timestamp);

  return conversion_queue_item;
}

base::Optional<ConversionQueueItemList> GetFromList(const base::Value* list) {
  DCHECK(list);
  DCHECK(list->is_list());

  ConversionQueueItemList conversion_queue_items;

  for (const auto& value : list->GetList()) {
    if (!value.is_dict()) {
      return base::nullopt;
    }

    const base::DictionaryValue* dictionary = nullptr;
    value.GetAsDictionary(&dictionary);
    if (!dictionary) {
      return base::nullopt;
    }

    const base::Optional<ConversionQueueItemInfo> conversion_queue_item =
        GetFromDictionary(dictionary);
    if (!conversion_queue_item) {
      return base::nullopt;
    }

    conversion_queue_items.push_back(conversion_queue_item.value());
  }

  return conversion_queue_items;
}

base::Optional<ConversionQueueItemList> FromJson(const std::string& json) {
  const base::Optional<base::Value> value = base::JSONReader::Read(json);
  if (!value || !value->is_dict()) {
    return base::nullopt;
  }

  const base::DictionaryValue* dictionary = nullptr;
  if (!value->GetAsDictionary(&dictionary)) {
    return base::nullopt;
  }

  const base::Value* list = dictionary->FindListKey(kListKey);
  if (!list) {
    return base::nullopt;
  }

  return GetFromList(list);
}

}  // namespace

void Migrate(InitializeCallback callback) {
  if (AdsClientHelper::Get()->GetBooleanPref(
          prefs::kHasMigratedConversionState)) {
    callback(SUCCESS);
    return;
  }

  BLOG(3, "Loading conversion state");

  AdsClientHelper::Get()->Load(
      kFilename, [=](const Result result, const std::string& json) {
        if (result != SUCCESS) {
          // Conversion state does not exist
          BLOG(3, "Successfully migrated conversion state");

          AdsClientHelper::Get()->SetBooleanPref(
              prefs::kHasMigratedConversionState, true);

          callback(SUCCESS);
          return;
        }

        const base::Optional<ConversionQueueItemList> conversion_queue_items =
            FromJson(json);

        if (!conversion_queue_items) {
          BLOG(0, "Failed to migrate conversions");
          callback(FAILED);
          return;
        }

        BLOG(3, "Successfully loaded conversion state");

        BLOG(1, "Migrating conversion state");

        database::table::ConversionQueue conversion_queue;
        conversion_queue.Save(
            conversion_queue_items.value(), [=](const Result result) {
              if (result != SUCCESS) {
                BLOG(0, "Failed to migrate conversion state");
                callback(FAILED);
                return;
              }

              AdsClientHelper::Get()->SetBooleanPref(
                  prefs::kHasMigratedConversionState, true);

              BLOG(3, "Successfully migrated conversion state");
              callback(SUCCESS);
            });
      });
}

}  // namespace conversions
}  // namespace ads
